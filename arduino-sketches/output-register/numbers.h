#define MINUS 0b00000010
#define DECIM 0b00000001
#define EXPON 0b10011110

//Digits
//0 - 9
//Binary goes a - dp

const byte DIG_MAP[10] = {
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110
};

const byte HEX_MAP[16] = {
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110,
  0b11101110,
  0b00111110,
  0b10011100,
  0b01111010,
  0b10011110,
  0b10001110
};

//Unsigned integer conversion
void ui_to_sci(unsigned long int data, byte segments[]) {
  unsigned long int divisor = 1;
  byte e_factor = 5;
  //Exponent > 9
  if (data > 999999) {
    //Truncate number
    while (data > 99999) {
      data /= 10;
      e_factor++;
    }
    segments[2] = EXPON;
    //write digits
    for (byte i=0; i<2; i++) {
      segments[i] = DIG_MAP[(e_factor / divisor) % 10];
      divisor *= 10;
    }
    divisor = 1;
    for (byte i=3; i<8; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
    segments[7] |= DECIM;
  }
  //Exponent < 10
  else {
    //Truncate number
    while (data > 999999) {
      data /= 10;
      e_factor++;
    }
    segments[1] = EXPON;
    //Include 5 decimal points already shifted
    segments[0] = DIG_MAP[e_factor];
    //write digits
    for (byte i=2; i<8; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
    segments[7] |= DECIM;
  }
}

void ui_to_dec(unsigned long int data, byte segments[]) {
  unsigned long int divisor = 1;
  
  //Convert to sci notation if too large
  if (data > 99999999) {
    ui_to_sci(data, segments);
  }
  else {
    for (byte i=0; i<8; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
  }
}

//Signed integer conversion
void si_to_sci(long int data, byte segments[]) {
  unsigned long int divisor = 1;
  byte e_factor = 5;
  //Exponent > 9
  if (data > 999999) {
    //Truncate number
    while (data > 9999) {
      data /= 10;
      e_factor++;
    }
    segments[2] = EXPON;
    //write digits
    for (byte i=0; i<2; i++) {
      segments[i] = DIG_MAP[(e_factor / divisor) % 10];
      divisor *= 10;
    }
    divisor = 1;
    for (byte i=2; i<7; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
    segments[6] |= DECIM;
  }
  //Exponent < 10
  else {
    //Truncate number
    while (data > 99999) {
      data /= 10;
      e_factor++;
    }
    segments[1] = EXPON;
    segments[0] = e_factor;
    //write digits
    for (byte i=2; i<8; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
    segments[6] |= DECIM;
  }
}

void si_to_dec(long int data, byte segments[]) {
  unsigned long int divisor = 1;
  
  //Add negative sign
  if (data < 0) {
    segments[7] = MINUS;
    data = abs(data);
  }
  
  //Convert to sci notation if too large
  if (data > 9999999) {
    si_to_sci(data, segments);
  }
  else {
    for (byte i=0; i<7; i++) {
      segments[i] = DIG_MAP[(data / divisor) % 10];
      divisor *= 10;
    }
  }
}

void float_to_dec(float data, byte segments[]) {
  unsigned long int divisor = 1;
  byte point = 0;

  //Add negative sign
  if (data < 0) {
    segments[7] = MINUS;
    data = abs(data);
  }
  
  if (log(data) < -6) {
    data *= pow(10, 14);
    si_to_dec(data, segments);
  }
  else if (log(data) > 6) {
    si_to_dec(data, segments);
  }

  else {
    point = ceil(log10(data));
    data *= pow(10, 7-point);
    long int data_2 = data;
  
    for (byte i=0; i<7; i++) {
      segments[i] = DIG_MAP[(data_2 / divisor) % 10];
      divisor *= 10;
    }
    segments[7 - point] |= DECIM; 
  }
}

//hexdecimal
void ui_to_hex(unsigned long int data, byte segments[]) {
  unsigned long int divisor = 1;
  for (byte i=0; i<8; i++) {
      segments[i] = HEX_MAP[(data / divisor) % 16];
      divisor *= 16;
  }
}

void si_to_hex(long int data, byte segments[]) {
  unsigned long int divisor = 1;
  
  if (data < 0) {
    segments[7] = MINUS;
    data = abs(data);
  }
  
  for (byte i=0; i<7; i++) {
      segments[i] = HEX_MAP[(data / divisor) % 16];
      divisor *= 16;
  }
}

//binary
void data_to_bin(byte data, byte segments[]) {
  for (byte i=0; i<8; i++) {
    Serial.println((data >> i) & 0b00000001, BIN);
    segments[i] = DIG_MAP[(data >> i) & 0b00000001];
  }
}
