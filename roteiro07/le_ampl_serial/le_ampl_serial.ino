/* Programa que mede um sinal senoidal a 19225Hz, calcula a amplitude 
 *  pico-a-pico do sinal e envia o valor calculado pela porta serial.
 */
typedef union {
    float floatingPoint;
    byte binary[4];
} binaryFloat;

float piE[] = {0.000000, 0.176777, 0.250000, 0.176777, -0.000000, -0.176777, -0.250000, -0.176777, 0.250000, 0.176777, -0.000000, -0.176777, -0.250000, -0.176777, 0.000000, 0.176777};
int numSamples = 0;
byte medida[8];

void setup(){
   // Configura a serial: baud rate de 115200, 8-bit, sem paridade, 1 stop bit
   Serial.begin(115200, SERIAL_8N1);
   ADCSRA = 0;             // clear ADCSRA register
   ADCSRB = 0;             // clear ADCSRB register
   ADMUX |= (0 & 0x07);    // set A0 analog input pin
   ADMUX |= (1 << REFS0);  // set reference voltage
   ADMUX |= (1 << ADLAR);  // left align ADC value to 8 bits from ADCH register
   ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    // 8 prescaler for 153.8 KHz
   ADCSRA |= (1 << ADATE); // enable auto trigger
   ADCSRA |= (1 << ADIE);  // enable interrupts when measurement complete
   ADCSRA |= (1 << ADEN);  // enable ADC
   ADCSRA |= (1 << ADSC);  // start ADC measurements
}

ISR(ADC_vect)
{
  medida[numSamples] = ADCH;  // read 8 bit value from ADC
  numSamples++;
  if( numSamples == 8 ) {
    ADCSRA &= ~(1 << ADSC);  // stop ADC measurements
    ADCSRA &= ~(1 << ADEN);  // disable ADC
  }
}

void loop(){
  int i;
  float seno, cosseno;
  binaryFloat amplitude;
  if( numSamples == 8 ) {    // fim da coleta
    numSamples = 0;
    seno = 0;
    cosseno = 0;
    for (i = 0; i < 8; i++){
      seno += (float)medida[i]*(float)piE[i];
      cosseno += (float)medida[i]*(float)piE[i+8];
    }
    amplitude.floatingPoint = 2*sqrt(sq(seno)+sq(cosseno))*5.0/255;
    Serial.write(amplitude.binary,4);
    delay(100); // aguarda 100ms e inicia nova coleta
    ADCSRA |= (1 << ADEN);  // enable ADC
    ADCSRA |= (1 << ADSC);  // start ADC measurements
  }
}
