/********************************************************************************
 * Coleta 1000 pontos no ADC, calcula a taxa de amostragem e imprime os
 * resultados na serial.
 * Codigo baseado no exemplo disponibilizado no site:
 * http://yaab-arduino.blogspot.com/2015/02/fast-sampling-from-analog-input.html
 ********************************************************************************/
int numSamples=0;
long t, t0;
byte x;

void setup()
{
  Serial.begin(115200);

  ADCSRA = 0;             // clear ADCSRA register
  ADCSRB = 0;             // clear ADCSRB register
  ADMUX |= (0 & 0x07);    // set A0 analog input pin
  ADMUX |= (1 << REFS0);  // set reference voltage
  ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register

  // sampling rate is [clock] / [prescaler] / [conversion clock cycles]
  // for Arduino Uno clock is 16 MHz and a conversion takes 13 clock cycles
  //ADCSRA |= (1 << ADPS2) | (1 << ADPS0);    // 32 prescaler for 38.5 KHz
  //ADCSRA |= (1 << ADPS2);                   // 16 prescaler for 76.9 KHz
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
  //ADCSRA |= (1 << ADPS1);                   // 4 prescaler for 307 KHz
  //ADCSRA |= (1 << ADPS0);                   // 2 prescaler for 615 KHz

  ADCSRA |= (1 << ADATE); // enable auto trigger
  ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN);  // enable ADC
  ADCSRA |= (1 << ADSC);  // start ADC measurements
}

ISR(ADC_vect)
{
  x = ADCH;  // read 8 bit value from ADC
  numSamples++;
}
  
void loop()
{
  if (numSamples>=1000)
  {
    t = micros()-t0;  // calculate elapsed time

    Serial.print("Frequencia de amostragem: ");
    Serial.print((float)1000000/t);
    Serial.print(" KHz. Valor lido: ");
    Serial.println(x);
    delay(2000);
    
    // restart
    t0 = micros();
    numSamples=0;
  }
}
