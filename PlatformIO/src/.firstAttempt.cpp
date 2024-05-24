#include <Arduino.h>

#define WINDOW_SIZE 25

int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;

int offset = 0;

void setup() {
  REG_DAC_CTRLA &= !DAC_CTRLA_ENABLE;
  REG_DAC_CTRLB = (REG_DAC_CTRLB & 0b00111111) | DAC_CTRLB_REFSEL_INT1V;
  REG_DAC_CTRLA |= DAC_CTRLA_ENABLE;
  analogReference(AR_INTERNAL1V1);
  REG_ADC_CTRLA &= !ADC_CTRLA_ENABLE;
  REG_ADC_CTRLB |= ADC_CTRLB_RESSEL_16BIT;
  REG_ADC_AVGCTRL |= ADC_AVGCTRL_SAMPLENUM_512 | ADC_AVGCTRL_ADJRES(10);
  REG_ADC_CTRLA |= ADC_CTRLA_ENABLE;
  analogWriteResolution(10);
  analogWrite(A0, 512);
  Serial.begin(115200);
  offset = (int)analogRead(A2);

  uint8_t bit = 10;
  uint16_t val, newval = 0;

  while (bit > 0U) {
    bit --;
    newval = val | 1 << bit;
    analogWrite(A0, newval);

    delay(10);
    if (((int)analogRead(A1) - offset) < 0) {
      val = newval;
    }
    delay(10);
  }
  delay(1000);


  while (INDEX < 4) {
    SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
    VALUE = (int)analogRead(A1) - offset;        // Read the next sensor value
    READINGS[INDEX] = VALUE;           // Add the newest reading to the window
    SUM = SUM + VALUE;                 // Add the newest reading to the sum
    INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

    AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  }

  offset += AVERAGED;
  
}

void loop() {

  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
  VALUE = (int)analogRead(A1) - offset;        // Read the next sensor value
  READINGS[INDEX] = VALUE;           // Add the newest reading to the window
  SUM = SUM + VALUE;                 // Add the newest reading to the sum
  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size

  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result


  //Serial.println((int)analogRead(A1) - offset);
  if(INDEX%5 == 1) {
    Serial.println(AVERAGED);
  }

  delayMicroseconds(10);
}