#include <Wire.h>
#include <Ticker.h>
#include <PF.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

#include "config.h"

//Null function
void nullfunc() {
}

//SD
FATFS fs;

SSD1306AsciiWire oled;

//SPEED in Hz
uint32_t FREQ = 1;

//Menu Globals
//BT1 = Switch menu, BT2 = Scroll around, BT3 = select
byte MODE = 0;
byte BTN_PINS[] = {BT1, BT2, BT3};
bool BTN_PREV_STATES[] = {LOW, LOW, LOW}; 

bool buttonEdge(byte index) {
  bool buttonstate = digitalRead(BTN_PINS[index]);
  if (buttonstate != BTN_PREV_STATES[index]) {
    BTN_PREV_STATES[index] = buttonstate;
    if (buttonstate == HIGH) {
      return true;
    }
  }
  return false;
}

//Menu St
char MENU_CONTENT[3][4][25] = {
  {"Load Program", ">Slot: 0", " Load", " Name"},
  {"Clock Options", ">Freq:  1  Hz", " Run", " Step"},
  {"Options", ">CROMs: 1", " Probe RAM", " None"},
};

byte CURRENT_MENU = 0;
byte MENU_CURSOR = 0;

void print_menu_content() {
  oled.clear();
  oled.setCursor(0, 0);
  for (byte i=0; i<4; i++) {
    oled.println(MENU_CONTENT[CURRENT_MENU][i]);
  }
}

void update_menu_cursor() {
  for (byte i=0; i<3; i++) {
    MENU_CONTENT[CURRENT_MENU][i+1][0] = 32;
  }
  MENU_CONTENT[CURRENT_MENU][MENU_CURSOR+1][0] = 62;
}

//File stuff
byte CURRENT_FILE = 0;
const char FILE_PATHS[10][3] = {
  "B0", "B1", "B2", "B3", "B4",
  "B5", "B6", "B7", "B8", "B9"
};

void inc_CurrentFile() {
  CURRENT_FILE++;
  CURRENT_FILE %= 10;
  MENU_CONTENT[0][1][7] = CURRENT_FILE + '0';
  print_menu_content();
}

void get_filename() {
  oled.clear();
  
  char buf;
  char file_buf[3];
  for (byte i=0; i<3; i++) {
    file_buf[i] = FILE_PATHS[CURRENT_FILE][i];
  }
  if (PF.open(file_buf)) {
    oled.println("Program slot empty!");
    oled.print("Press SELECT to exit!");
    MODE = 2;
    return;
  }
  
  for (byte i=0; i<16; i++) {
    UINT nr;
    PF.readFile(&buf, 1, &nr);
    oled.print(buf);
    if (nr == 0) {
      oled.println("Slot corrupted!");
      oled.print("Press SELECT to exit!");
      MODE = 2;
      return;
    }
  }
  oled.println();
  oled.print("Press SELECT to exit!");
  MODE = 2;
  return;
}

//Clock function
bool CLOCKSTATE = LOW;
bool HLTSTATE = false;

uint32_t FREQLIST[6] = {1, 10, 100, 1000, 10000, 20000};
char FREQLABEL[6][7] = {
  "  1  Hz", " 10  Hz", "100  Hz", "  1 kHz", " 10 kHz", " 20 kHz"
};
byte CURRENT_FREQ = 0;

void toggleClock() {
  digitalWrite(CLOCK, CLOCKSTATE);
  CLOCKSTATE = !CLOCKSTATE && !digitalRead(HLT);
}

Ticker clockTimer(toggleClock, (1000000/FREQ)/2, 0, MICROS_MICROS);

void switchFreq() {
  CURRENT_FREQ++;
  CURRENT_FREQ %= 6;
  FREQ = FREQLIST[CURRENT_FREQ];
  clockTimer.interval((1000000/FREQ)/2);
  
  for (byte i=0; i<7; i++) {
    MENU_CONTENT[1][1][7+i] = FREQLABEL[CURRENT_FREQ][i];
  }
  print_menu_content();
}

void stepClock() {
  digitalWrite(CLOCK, LOW);
  digitalWrite(CLOCK, (HIGH && !digitalRead(HLT)));
  delay(100);
  digitalWrite(CLOCK, LOW);
}

void runClock() {
  MODE = 2;
  oled.clear();
  oled.println("Clock running.");
  oled.print("Freq:");
  for (byte i=0; i<7; i++) {
    oled.print(FREQLABEL[CURRENT_FREQ][i]);
  }
  oled.println();
  oled.print("Press SELECT to exit!");
  clockTimer.start();
}

//CROM
bool CROM_STATE = LOW;
void toggleCROM() {
  CROM_STATE = !CROM_STATE;
  if (CROM_STATE == LOW) {
    MENU_CONTENT[2][1][8] = '1';
  }
  else {
    MENU_CONTENT[2][1][8] = '0';
  }
  digitalWrite(ROM_OE, CROM_STATE);
  print_menu_content();
}

//RAM functions
void setAddress(int address) {
  shiftOut(SER, SRCLK, MSBFIRST, (address >> 8));
  shiftOut(SER, SRCLK, MSBFIRST, address);

  digitalWrite(A_RCLK, LOW);
  digitalWrite(A_RCLK, HIGH);
  digitalWrite(A_RCLK, LOW);
}

void setData(byte data) {
  shiftOut(SER, SRCLK, MSBFIRST, data);

  digitalWrite(D_RCLK, LOW);
  digitalWrite(D_RCLK, HIGH);
  digitalWrite(D_RCLK, LOW);
}

void loadRAM() {
  //Clear OLED;
  oled.clear();
  
  //Clear shift registers
  setAddress(0);
  setData(0);

  //Disable control logic ROM, which also resets the PC and Step counter
  digitalWrite(ROM_OE, HIGH);
  
  //Prepare RAM for writing
  pinMode(RAM_WE, OUTPUT);
  pinMode(RAM_CE, OUTPUT);

  //Open File
  byte buf;
  char file_buf[3];
  for (byte i=0; i<2; i++) {
    file_buf[i] = FILE_PATHS[CURRENT_FILE][i];
  }
  if (PF.open(file_buf)) {
    oled.println("Program slot empty!");
    oled.print("Press SELECT to exit!");
    MODE = 2;
    return;
  }

  oled.println("Loading program...");
  //Flush out name string
  for (byte i=0; i<16; i++) {
    UINT nr;
    PF.readFile(&buf, 1, &nr);
    if (nr == 0) {
      oled.println("Slot corrupted!");
      oled.print("Press SELECT to exit!");
      MODE = 2;
      return;
    }
  }
  uint16_t addr = 0;
  while (addr < 2048) {
    UINT nr;
    PF.readFile(&buf, 1, &nr);
    if (nr == 0) { break; }
    setAddress(addr);      
    setData(buf);
    delay(1);
    digitalWrite(RAM_WE, HIGH);
    digitalWrite(OE, HIGH);
    digitalWrite(RAM_CE, HIGH);
    
    digitalWrite(RAM_CE, LOW);
    digitalWrite(OE, LOW);
    digitalWrite(RAM_WE, LOW);
    
    digitalWrite(RAM_WE, HIGH);
    digitalWrite(OE, HIGH);
    digitalWrite(RAM_CE, HIGH);
    Serial.print(addr, BIN); Serial.print(" : ");
    Serial.println(buf, BIN);
    addr++;
    
  }

  //Return pins to Z state
  pinMode(RAM_WE, INPUT);
  pinMode(RAM_CE, INPUT);
  
  //Enable control logic ROM
  digitalWrite(ROM_OE, CROM_STATE);

  oled.clear();
  oled.println("Program loaded!");
  oled.print(addr);
  oled.println(" bytes used");
  oled.print("Press SELECT to exit!");
  MODE = 2;
}

void probeRAM() {
  //Clear OLED;
  oled.clear();

  //prepare ram for reading
  pinMode(RAM_CE, OUTPUT);

  //Display RAM contents to bus
  digitalWrite(RAM_CE, HIGH);
  digitalWrite(RAM_CE, LOW);
  oled.println("Address displayed!");
  oled.print("Press SELECT to exit!");
  MODE = 2; 
}
void (*menu_functions[3][3])() = {
  {&inc_CurrentFile, &loadRAM, &get_filename},
  {&switchFreq, &runClock, &stepClock},
  {&toggleCROM, &probeRAM, &nullfunc}
};

void halt() {
  if (CLOCKSTATE) {
    HLTSTATE = true;
  }
}

void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.clear();
  oled.setFont(Adafruit5x7);
  oled.setCursor(0, 0);
  oled.print("init...");
  

  //Init SD
  PF.begin(&fs);
  
  
  //Init pins
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(SRCLR, OUTPUT);
  pinMode(A_RCLK, OUTPUT);
  pinMode(D_RCLK, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
  pinMode(BT3, INPUT);
  pinMode(RAM_WE, INPUT);
  pinMode(RAM_CE, INPUT);
  pinMode(ROM_OE, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  
  pinMode(HLT, INPUT);
  attachInterrupt(digitalPinToInterrupt(HLT), halt, RISING);

  digitalWrite(SRCLR, HIGH);
  digitalWrite(ROM_OE, LOW);
  digitalWrite(OE, HIGH);

  print_menu_content();
}

void loop() {
  clockTimer.update();
  switch (MODE) {
    //All buttons enabled
    case 0:
      if (buttonEdge(0)) {
        CURRENT_MENU++;
        CURRENT_MENU %= 3;
        MENU_CURSOR = 0;
        update_menu_cursor();
        print_menu_content();
        delay(80);

      }
      if (buttonEdge(1)) {
        MENU_CURSOR++;
        MENU_CURSOR %= 3;
        update_menu_cursor();
        print_menu_content();
        delay(80);

      }
      if (buttonEdge(2)) {
        //Runs function from pointer, weird
        (*menu_functions[CURRENT_MENU][MENU_CURSOR])();
        delay(80);
      }
      break;
    case 1:
      if (buttonEdge(2)) {
        MODE = 0;
        MENU_CURSOR = 0;
        update_menu_cursor();
        print_menu_content();
        delay(200);
      }
      break;
    case 2:
      if (buttonEdge(2)) {
        clockTimer.stop();
        digitalWrite(RAM_CE, HIGH);
        pinMode(RAM_CE, INPUT);
        digitalWrite(CLOCK, LOW);
        MODE = 0;
        MENU_CURSOR = 0;
        update_menu_cursor();
        print_menu_content();
        delay(200);
      }
      break;
  }
  
}
