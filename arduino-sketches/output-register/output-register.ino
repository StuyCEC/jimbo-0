#include "config.h"
#include "numbers.h"

//Bus buffer (connected to data bus);
byte DATA_BUF = 0;

//0 = accept data, 1 = accept mode
byte MODE = 0;

/* OUT MODE
 * 0 = decimal
 * 1 = hex
 * 2 = binary
 */
byte OUT_MODE = 0;

/* DATA TYPE
 * 0 = unsigned char
 * 1 = char
 * 2 = unsigned int
 * 3 = int
 * 4 = unsigned long int 
 * 5 = long int
 * 6 = float
 */
byte DATA_TYPE = 0;

// XXXX-YYYY, 
// XXXX = Output type hex, decimal, scientific not, binary(only works for chars)
// YYYY = Data type
//Data holders
unsigned int FLOAT = 0;

long int LONG_INT = 0;
int INT = 0;
char CHAR = 0;

unsigned long int ULONG_INT = 0;
unsigned int UINT = 0;
unsigned short USHORT = 0;
unsigned char UCHAR = 0;

//each digit register Lowest to highest
byte DIG_REG[8] = {0};

void clr() {
  FLOAT = 0;
  LONG_INT = 0;
  INT = 0;
  CHAR = 0;
  ULONG_INT = 0;
  UINT = 0;
  UCHAR = 0;

  DATA_BUF = 0;
  DATA_TYPE = 0;
  OUT_MODE = 0;
  memset(DIG_REG, 0, 8*sizeof(byte));
  MODE = 1;
  return;
}

void get_mode() {
  OUT_MODE = DATA_BUF >> 4;
  DATA_TYPE = DATA_BUF & 0b00001111;
}

void read_data() {
  for (byte i=0; i<8; i++) {
      bitWrite(DATA_BUF, i, digitalRead(BUS_PINS[i]));
  }
  if (MODE == 1) {
    get_mode();
    MODE = 0;
  }
  else {
    switch (DATA_TYPE) {
      case 0:
        UCHAR = DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            ui_to_dec(UCHAR, DIG_REG);
            break;
          case 1:
            ui_to_hex(UCHAR, DIG_REG);
            break;
          case 2:
            data_to_bin(UCHAR, DIG_REG);
            break;
        }
       break;
       
      case 1:
        CHAR = DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            si_to_dec(CHAR, DIG_REG);
            break;
          case 1:
            si_to_hex(CHAR, DIG_REG);
            break;
          case 2:
            data_to_bin(CHAR, DIG_REG);
            break;
        }
        break;

      case 2:
        UINT = (UINT << 8) | DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            ui_to_dec(UINT, DIG_REG);
            break;
          case 1:
            ui_to_hex(UINT, DIG_REG);
            break;
        }
        break;
        
      case 3:
        INT = (INT << 8) | DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            si_to_dec(INT, DIG_REG);
            break;
          case 1:
            si_to_hex(INT, DIG_REG);
            break;
        }
        break;
        
      case 4:
        ULONG_INT = (ULONG_INT << 8) | DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            ui_to_dec(ULONG_INT, DIG_REG);
            break;
          case 1:
            ui_to_hex(ULONG_INT, DIG_REG);
            break;
        }
        break;
        
      case 5:
        LONG_INT = (LONG_INT << 8) | DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            si_to_dec(ULONG_INT, DIG_REG);
            break;
          case 1:
            si_to_hex(ULONG_INT, DIG_REG);
            break;
        }
        break;
        
      case 6:
        FLOAT = (FLOAT << 8) | DATA_BUF;
        switch (OUT_MODE) {
          case 0:
            float_to_dec(FLOAT, DIG_REG);
            break;
          case 1:
            si_to_hex(FLOAT, DIG_REG);
            break;
        }
        break;
    }
  }
  return;
}

bool CLR_PREV_STATE = LOW;
bool IN_PREV_STATE = LOW;

bool CLR_EDGE() {
  bool buttonstate = digitalRead(OUT_CLR);
  if (buttonstate != CLR_PREV_STATE) {
    CLR_PREV_STATE = buttonstate;
    if (buttonstate == HIGH) {
      return true;
    }
  }
  return false;
}

bool IN_EDGE() {
  bool buttonstate = digitalRead(OUT_IN);
  if (buttonstate != IN_PREV_STATE) {
    IN_PREV_STATE = buttonstate;
    if (buttonstate == HIGH) {
      return true;
    }
  }
  return false;
}


void setup() { 
  //pin setup
  for (byte i=0; i<8; i++) {
    pinMode(BUS_PINS[i], INPUT);
    
    pinMode(SEG_SEL[i], OUTPUT);
    digitalWrite(SEG_SEL[i], HIGH);
  }
  pinMode(OUT_IN, INPUT);
  pinMode(OUT_CLR, INPUT);
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);

  /*
  attachInterrupt(digitalPinToInterrupt(OUT_IN), read_data, RISING);
  attachInterrupt(digitalPinToInterrupt(OUT_CLR), clr, RISING);
  */

}

void loop() {
  for (byte i=0; i<8; i++) {
    if (CLR_EDGE()) {
      clr();
    }
    if (IN_EDGE()) {
      read_data();
    }
    shiftOut(SER, SRCLK, LSBFIRST, DIG_REG[i]);
    digitalWrite(SRCLK, LOW);
    digitalWrite(SRCLK, HIGH);
    digitalWrite(SRCLK, LOW);
    digitalWrite(SEG_SEL[i], LOW);
    delay(1);
    digitalWrite(SEG_SEL[i], HIGH);
  }
}
