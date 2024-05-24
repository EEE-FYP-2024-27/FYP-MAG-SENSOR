#include <Arduino.h>

int offset;

void setup() {
  REG_DAC_CTRLA |= DAC_CTRLA_ENABLE;
  analogReference(AR_INTERNAL1V1);
  REG_ADC_CTRLA &= !ADC_CTRLA_ENABLE;
  REG_ADC_CTRLB |= ADC_CTRLB_RESSEL_16BIT;
  REG_ADC_AVGCTRL |= ADC_AVGCTRL_SAMPLENUM_512 | ADC_AVGCTRL_ADJRES(10);
  REG_ADC_CTRLA |= ADC_CTRLA_ENABLE;


  Serial.begin(115200);
  offset = (int)analogRead(A2);
  
}

void loop() {

  Serial.println((int)analogRead(A1) - offset);

  delay(10);
}