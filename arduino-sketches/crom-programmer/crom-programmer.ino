#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13
//0b00000000000000000000000000000000 
//XOR 0b00000011111111111000111101111111
//01111111
//10001111
#define INVERTS   0b000011111111111000111101111111
#define A_IN      0b000000000000000000000000000001 //INV 1
#define A_OUT     0b000000000000000000000000000010 //INV 2
#define B_IN      0b000000000000000000000000000100 //INV 3
#define B_OUT     0b000000000000000000000000001000 //INV 4
#define C_IN      0b000000000000000000000000010000 //INV 5
#define C_OUT     0b000000000000000000000000100000 //INV 6
#define F_IN      0b000000000000000000000001000000 //INV 7
#define F_OUT     0b000000000000000000000010000000
#define ALU_A_IN  0b000000000000000000000100000000 //INV 9
#define ALU_B_IN  0b000000000000000000001000000000 //INV 10
#define ARI_OUT   0b000000000000000000010000000000 //INV 11
#define NAND_OUT  0b000000000000000000100000000000 //INV 12
#define CARRY_EN  0b000000000000000001000000000000 
#define SUB_EN    0b000000000000000010000000000000
#define PC_EN     0b000000000000000100000000000000 
#define PC_OUT    0b000000000000001000000000000000 //INV 16
#define PC_IN     0b000000000000010000000000000000 //INV 17
#define PC_RST    0b000000000000100000000000000000 //INV 18
#define RAM_WE    0b000000000001000000000000000000 //INV 19
#define RAM_CE    0b000000000010000000000000000000 //INV 20
#define MH_IN     0b000000000100000000000000000000 //INV 21
#define ML_IN     0b000000001000000000000000000000 //INV 22
#define M_OUT     0b000000010000000000000000000000 //INV 23
#define IR_IN     0b000000100000000000000000000000 //INV 24
#define NIL_W     0b000001000000000000000000000000
#define STPC_RST  0b000010000000000000000000000000 //INV 26
#define OUT_IN    0b000100000000000000000000000000
#define OUT_RST   0b001000000000000000000000000000
#define HLT       0b010000000000000000000000000000 
#define FETCH     0b100000000000000000000000000000 
#define ROMSEL 3

uint32_t fetchc[8] = {
  PC_OUT|RAM_CE|IR_IN|FETCH, FETCH, PC_EN|FETCH, STPC_RST, 0, 0, 0, 0
};

/*  
uint32_t UCODE_TEMPLATE_Z0[16][8] = {
  {PC_EN, PC_OUT|R10001111AM_CE|RESEL, 0, STPC_RST|FETCH,                                                         0, 0, 0, 0}, //0x00 MOV 
  {M_OUT|RAM_CE, STPC_RST|FETCH,                                                                     0, 0, 0, 0, 0, 0}, //0x01 LDR
  {M_OUT|RAM_CE, M_OUT|RAM_WE|RAM_CE, STPC_RST|FETCH,                                                  0, 0, 0, 0 ,0}, //0x02 STR
  {PC_EN, PC_OUT|RAM_CE|RESEL, ALU_A_IN, ALU_B_IN, ARI_OUT|F_IN, STPC_RST|FETCH,                                0, 0}, //0x03 ADD
  {PC_EN, PC_OUT|RAM_CE|RESEL, ALU_A_IN, ALU_B_IN, ARI_OUT|CARRY_EN, F_IN|ARI_OUT|CARRY_EN, STPC_RST|FETCH,        0}, //0x04 ADC
  {PC_EN, PC_OUT|RAM_CE|RESEL, ALU_A_IN, ALU_B_IN, SUB_EN|ARI_OUT|F_IN, STPC_RST|FETCH,                         0, 0}, //0x05 SUB
  {PC_EN, PC_OUT|RAM_CE|RESEL, ALU_A_IN, ALU_B_IN, SUB_EN|ARI_OUT|CARRY_EN, F_IN|ARI_OUT|CARRY_EN, STPC_RST|FETCH, 0}, //0x06 SBB
  {PC_EN, PC_OUT|RAM_CE|RESEL, ALU_A_IN, ALU_B_IN, NAND_OUT|F_IN, STPC_RST|FETCH,                               0, 0}, //0x07 NAND
  {M_OUT|PC_IN, STPC_RST|FETCH,                                                                     0, 0, 0, 0, 0, 0}, //0x08 JMP
  {F_OUT, STPC_RST|FETCH, STPC_RST|FETCH,                                                              0, 0, 0, 0, 0}, //0x09 JNZ
  {F_OUT, STPC_RST|FETCH, STPC_RST|FETCH,                                                              0, 0, 0, 0, 0}, //0x0A JNC
  {OUT_IN, STPC_RST|FETCH,                                                                          0, 0, 0, 0, 0, 0}, //0x0B OUT
  {OUT_RST, STPC_RST|FETCH,                                                                         0, 0, 0, 0, 0, 0}, //0x0C COT
  {HLT,                                                                                          0, 0, 0, 0, 0, 0, 0}, //0x0D HLT
  {ML_IN, STPC_RST|FETCH,                                                                           0, 0, 0, 0, 0, 0}, //0x0E MIL
  {MH_IN, STPC_RST|FETCH,                                                                           0, 0, 0, 0, 0, 0} //0x0F MIH
};
*/ 

uint32_t UCODE_TEMPLATE[16][8] = {
  {PC_OUT|RAM_CE|C_IN, PC_EN, STPC_RST|FETCH,                                                                 0, 0, 0, 0, 0}, //0x00 MOV R1R2
  {PC_OUT|RAM_CE|MH_IN, PC_EN, PC_O , M_OUT|RAM_CE|C_IN, PC_EN, STPC_RST|FETCH,                          0, 0},//0x01 LDR M->R1
  {PC_OUT|RAM_CE|MH_IN, PC_EN, PC_OUT|RAM_CE|ML_IN, M_OUT|RAM_CE, M_OUT|RAM_CE|RAM_WE|C_OUT, PC_EN, STPC_RST|FETCH,       0}, //0x02 STR R1->M                             
  {PC_OUT|RAM_CE|ALU_B_IN, ALU_A_IN|C_OUT, ARI_OUT|F_IN|C_IN, PC_EN, STPC_RST|FETCH,                                0, 0, 0}, //0x03 ADD R1R2
  {PC_OUT|RAM_CE|ALU_B_IN, ALU_A_IN|C_OUT, ARI_OUT|CARRY_EN|C_IN, F_IN|ARI_OUT|CARRY_EN, PC_EN,  STPC_RST|FETCH,       0, 0}, //0x04 ADC R1R2
  {PC_OUT|RAM_CE|ALU_B_IN|SUB_EN, ALU_A_IN|C_OUT, ARI_OUT|F_IN|C_IN, PC_EN, STPC_RST|FETCH,                         0, 0, 0}, //0x05 SUB R1R2
  {PC_OUT|RAM_CE|ALU_B_IN|SUB_EN, ALU_A_IN|C_OUT, ARI_OUT|CARRY_EN|C_IN, F_IN|ARI_OUT|CARRY_EN, PC_EN, STPC_RST|FETCH, 0, 0}, //0x06 SBB R1R2
  {PC_OUT|RAM_CE|ALU_B_IN, ALU_A_IN|C_OUT, NAND_OUT|F_IN|C_IN, PC_EN, STPC_RST|FETCH,                               0, 0, 0}, //0x07 NAND R1R2
  {PC_OUT|RAM_CE|MH_IN, PC_EN, PC_OUT|RAM_CE|ML_IN, M_OUT|PC_IN, STPC_RST|FETCH,                                    0, 0, 0}, //0x08 JMP M                                            
  {PC_OUT|RAM_CE|MH_IN, PC_EN, PC_OUT|RAM_CE|ML_IN, F_OUT, M_OUT|PC_IN|F_OUT, STPC_RST|FETCH,                          0, 0}, //0x09 JNZ M                                                       
  {PC_OUT|RAM_CE|MH_IN, PC_EN, PC_OUT|RAM_CE|ML_IN, F_OUT, M_OUT|PC_IN|F_OUT, STPC_RST|FETCH,                          0, 0}, //0x0A JNC M
  {OUT_IN, PC_OUT|RAM_CE|OUT_IN, PC_EN, STPC_RST|FETCH,                                                          0, 0, 0, 0}, //0x0B OUT R1
  {OUT_RST, STPC_RST|FETCH,                                                                                0, 0, 0, 0, 0, 0}, //0x0C COT
  {HLT,                                                                                                 0, 0, 0, 0, 0, 0, 0}, //0x0D HLT
  {PC_OUT|RAM_CE|ML_IN, PC_EN, STPC_RST|FETCH,                                                                0, 0, 0, 0, 0}, //0x0E MIL R1
  {PC_OUT|RAM_CE|MH_IN, PC_EN, STPC_RST|FETCH,                                                                0, 0, 0, 0, 0}, //0x0F MIH R1
};

uint32_t set_invert(uint32_t ucode[16][8]) {
  for (byte i=0; i<16; i++) {
    for (byte y=0; y<8; y++) {
      ucode[i][y] ^= INVERTS;
    }
  }
}

void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}
/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents(int start, int length) {
  for (int base = start; base < length; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[160];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

void writeEEPROM(int address, uint32_t longdata) {
  byte data = (longdata >> (8*ROMSEL));
  setAddress(address, false);
  int x = (address & 0b0011110000000) >> 7;
  int y = (address & 0b0000001111000) >> 3;
  if ((x == 0x0b) && (y == 0x00)){
    Serial.println(data, BIN);
  }
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


void setup() {
  set_invert(UCODE_TEMPLATE);
  
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  
  Serial.begin(57600);

  // Program data bytes
  Serial.print("Programming EEPROM");
  
  // Program the 8 high-order bits of microcode into the first 128 bytes of EEPROM
  for (unsigned int address = 0; address < 8192; address+= 8) {
    int flag        = (address & 0b1100000000000) >> 11;
    int x           = (address & 0b0011110000000) >> 7;
    int y           = (address & 0b0000001111000) >> 3;
    int step        = (address & 0b0000000000111);

  
    for (byte i = 0; i<8; i++) {
      writeEEPROM(address + i, UCODE_TEMPLATE[x][i]);
    }
    switch (x) {
      //MV
      case 0x00:
        switch (y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (C_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (C_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (C_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 0, (PC_OUT|RAM_CE|A_IN)^INVERTS); 
            break;
          case 0x0a:
            writeEEPROM((address & 0b1111111111000) + 0, (PC_OUT|RAM_CE|B_IN)^INVERTS);
            break;
        }
        break;
      //LDR
      case 0x01:
        switch (y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);  
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS); 
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS); 
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 3, (M_OUT|RAM_CE|A_IN)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 3, (M_OUT|RAM_CE|B_IN)^INVERTS);
            break;
        }
        break;
      //STR
      case 0x02:
        switch (y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (M_OUT|RAM_CE|RAM_WE|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (STPC_RST|FETCH)^INVERTS); 
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (M_OUT|RAM_CE|RAM_WE|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (STPC_RST|FETCH)^INVERTS); 
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|RAM_CE)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (M_OUT|RAM_CE|RAM_WE|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (STPC_RST|FETCH)^INVERTS); 
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 4, (M_OUT|RAM_CE|RAM_WE|A_OUT)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 4, (M_OUT|RAM_CE|RAM_WE|B_OUT)^INVERTS);
            break;
        }
        break;
      //ADD
      case 0x03:
        switch (y) {
          case 0x00: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|A_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|A_IN)^INVERTS);
            break;
          case 0x0A:
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|B_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|B_IN)^INVERTS);
            break;
        }
        break;
      //ADC
      case 0x04:
        switch (y) {
          case 0x00: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (F_IN|ARI_OUT|CARRY_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|A_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            break;
          case 0x0A:
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|B_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            break;
        }
        break;
      //SUB
      case 0x05:
        switch (y) { 
          case 0x00: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|A_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|A_IN)^INVERTS);
            break;
          case 0x0A:
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|B_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|F_IN|B_IN)^INVERTS);
            break;
        }
        break;
      case 0x06:
      switch (y) {
          case 0x00: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT|SUB_EN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (ARI_OUT|CARRY_EN|F_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 4, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|A_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|CARRY_EN|A_IN)^INVERTS);
            break;
          case 0x0A:
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|B_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|CARRY_EN|B_IN)^INVERTS);
            break;
        }
        break;
      //NAND
      case 0x07:
        switch (y) {
          case 0x00: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|A_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x03:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x04:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (ARI_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x05: 
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|B_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x06:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x07:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x08:
            writeEEPROM((address & 0b1111111111000) + 0, (ALU_A_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_B_IN|C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (NAND_OUT|F_IN|C_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x09: 
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|A_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|A_IN)^INVERTS);
            break;
          case 0x0A:
            writeEEPROM((address & 0b1111111111000) + 1, (ALU_A_IN|B_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 2, (ALU_A_IN|B_IN)^INVERTS);
            break;
        }
        break;
      //JMP
      case 0x08:
        switch (y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (M_OUT|PC_IN)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
        }
        break;
      //JNZ
      case 0x09:
        switch (y) {   
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (F_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 1, (M_OUT|PC_IN|F_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (STPC_RST|FETCH)^INVERTS);
            break;
        }
        break;
      //JNC
      case 0x0a:
        switch (y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (F_OUT)^INVERTS); 
            writeEEPROM((address & 0b1111111111000) + 1, (M_OUT|PC_IN|F_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (STPC_RST|FETCH)^INVERTS);
            break;
        }
        break;
      //OUT
      case 0x0b:
        switch(y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (A_OUT|OUT_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (A_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (B_OUT|OUT_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (B_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (C_OUT|OUT_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 2, (C_OUT)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 3, (STPC_RST|FETCH)^INVERTS);
            break;          
        }
        break;
      //MIL
      case 0x0e:
        switch(y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (A_OUT|ML_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (B_OUT|ML_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (C_OUT|ML_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;          
        }
        break;
      //MIH
      case 0x0f:
        switch(y) {
          case 0x00:
            writeEEPROM((address & 0b1111111111000) + 0, (A_OUT|MH_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x01:
            writeEEPROM((address & 0b1111111111000) + 0, (B_OUT|MH_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;
          case 0x02:
            writeEEPROM((address & 0b1111111111000) + 0, (C_OUT|MH_IN)^INVERTS);
            writeEEPROM((address & 0b1111111111000) + 1, (STPC_RST|FETCH)^INVERTS);
            break;        
        }
        break;
    }

    switch (flag) {
      //Z0F0
      case 0x01:
        writeEEPROM(((address & 0b1100000000000) | 0b0010010000000) + 1, (STPC_RST|FETCH)^INVERTS);
        writeEEPROM(((address & 0b1100000000000) | 0b0010010000000) + 4, (STPC_RST|FETCH)^INVERTS);
        break;
      //Z0F1
      case 0x02:
        writeEEPROM(((address & 0b1100000000000) | 0b0010100000000) + 1, (STPC_RST|FETCH)^INVERTS);
        writeEEPROM(((address & 0b1100000000000) | 0b0010100000000) + 4, (STPC_RST|FETCH)^INVERTS);
        break;
      //Z1F0
      case 0x03:
        writeEEPROM(((address & 0b1100000000000) | 0b0010010000000) + 1, (STPC_RST|FETCH)^INVERTS);
        writeEEPROM(((address & 0b1100000000000) | 0b0010010000000) + 4, (STPC_RST|FETCH)^INVERTS);
        writeEEPROM(((address & 0b1100000000000) | 0b0010100000000) + 1, (STPC_RST|FETCH)^INVERTS);
        writeEEPROM(((address & 0b1100000000000) | 0b0010100000000) + 4, (STPC_RST|FETCH)^INVERTS);
        break;
    }
    
    if (address % 256 == 0) {
      Serial.print(".");
    }
  }
  
  for (byte i=0; i<8; i++) {
    writeEEPROM(0b0000001111000 + i, fetchc[i]^INVERTS); 
  }
  Serial.println(" done");

  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents(0, 8192);
}

void loop() {
  // put your main code here, to run repeatedly:

}
