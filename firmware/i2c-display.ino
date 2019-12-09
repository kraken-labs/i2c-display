#include <TinyWireS.h>
#include <math.h>

#define I2C_SLAVE_ADDRESS 0x5

#define DATA_PIN PB3
#define CLOCK_PIN PB4
#define LATCH_PIN 8 // NO_USAR // PIN_A5 // MISO

#define DIGIT_SWITCH PB1

const byte digit_pattern[16] =
{
  B10111111,  // 0
  B10000110,  // 1
  B11011011,  // 2
  B11001111,  // 3
  B11100110,  // 4
  B11101101,  // 5
  B11111101,  // 6
  B10000111,  // 7
  B11111111,  // 8
  B11101111,  // 9
  B11110111,  // A
  B11111100,  // b
  B10111001,  // C
  B11011110,  // d
  B11111001,  // E
  B11110001   // F
};

volatile byte currentValue = 13;

void receiveEvent(uint8_t howMany)
{
  /* // Sanity checks, enable when working
  if (TinyWireS.available()){  
    if (howMany < 1) {   // Sanity-check
      return;
    }
    if (howMany > TWI_RX_BUFFER_SIZE) {   // Also insane number
      return;
    }
  }*/
  
  /* // 1st attempt
  if (howMany == 1) {
    if (STATE == 0) {
      STATE = 1;
    }
  }
  */

  // 2nd attempt
  if (TinyWireS.available()) {
    byte newValue = TinyWireS.read();

    if (newValue >= 0 && newValue < 100) {
      currentValue = newValue;
    }
  }
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DIGIT_SWITCH, OUTPUT);

  renderDigits(currentValue);

  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onReceive(receiveEvent);
}

void renderDigits(byte n) {
  byte pattern;
  byte firstDigit = n % 10;
  byte secondDigit = round(n / 10);

  for (int i = 0; i < 100; i++) {
    pattern = digit_pattern[firstDigit];
    //digitalWrite(LATCH_PIN, LOW);
    digitalWrite(DIGIT_SWITCH, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ~(pattern | 0b00000000));
    //digitalWrite(LATCH_PIN, HIGH);
    delay(5);

    pattern = digit_pattern[secondDigit];
    //digitalWrite(LATCH_PIN, LOW);
    digitalWrite(DIGIT_SWITCH, HIGH);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ~(pattern | 0b10000000));
    //digitalWrite(LATCH_PIN, HIGH);
    delay(5);
  }
}

void loop() {
  TinyWireS_stop_check();

  renderDigits(currentValue);
}
