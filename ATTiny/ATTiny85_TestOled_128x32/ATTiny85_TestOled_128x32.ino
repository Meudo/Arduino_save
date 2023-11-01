/* Digispark ATtiny85 to small OLED 128x32 by I2C
 *  https://github.com/datacute/Tiny4kOLED 
 *  Tiny4kOLED.h: SSD1306xLED-Drivers for OLED 128x32 displays
 * @created: 2014-08-12  @author: Neven Boyanov
 * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled
 * @ modified for 16x32 fonts: 2017-01-12 @author: M. V. Predoi 
 * Fits into Digispark  ATtiny85 (6012 bytes). 52 bytes of dynamic memory.
 *  ATtiny85   OLED
 *    <<------>>
 *  PB0        SDA
 *  PB2        SCL
 *  5V         VCC
 *  GND        GND
 *  
 */

/******** Ecran ************/
#include <Tiny4kOLED.h>
#include <TinyWireM.h>
#include <OneWire.h>
//#include <Manchester.h>
/******** Ecran ************/

// ============================================================================

int ledPin=3;
float temperature = 0;

#define ONEWIRE_BUSS 4
OneWire TemperatureSensor(ONEWIRE_BUSS);

//#define BUFFER_SIZE 7
#define NO_MODULE 13
#define TX_PIN 1  //pin where your transmitter is connected
//uint8_t data2send[BUFFER_SIZE]; // 00 (debut), Id, Signe, Partie Entiere, Partie Decimale, CheckSum, 0xFF (Fin)

typedef struct {
  char NoModule;
  float Valeur;
} MaStructure;


uint16_t readVcc(void) {
  uint16_t result;
  // Read 1.1V reference against Vcc
  ADMUX = (0<<REFS0) | (12<<MUX0);
  delay(2); // Wait for Vref to settle
  ADCSRA |= (1<<ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCW;
  return 1018500L / result; // Back-calculate AVcc in mV
}


void setup()
{
  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT16X32); // 1 raw of 8 characters exactly fills 128x32

/*  oled.clear(); //all black
  oled.setCursor(0, 0);
  oled.print(F("Temp")); //wrap strings in F() to save RAM!
  delay(1000);*/
  oled.setFont(FONT8X16); // 2 raw of 16 characters exactly fills 128x32
  oled.setCursor(0, 0);
  oled.print(F("Temperature")); //wrap strings in F() to save RAM!
  oled.setCursor(0, 2);
  oled.print(F("Celsus")); //wrap strings in F() to save RAM!*/
//----------------------------------
   pinMode(ledPin, OUTPUT); 
   digitalWrite(ledPin, LOW); 

  //man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
//  man.setupTransmit(TX_PIN, MAN_1200);
  data2send[0] = 0xFE;
//  data2send[1] = ID_MODULE;
  data2send[2] = 0x00; //Signe
  data2send[3] = 0x00; //Partie Entiere
  data2send[4] = 0x00; //Partie Decimale
  data2send[5] = 0x00; //CheckSum
  data2send[6] = 0xFF;
}

void loop()
{
    byte i;
    byte data[12];
    int16_t raw;
    float celsius;
    //float fahrenheit;

  digitalWrite(ledPin, HIGH); 
  delay(10);
  digitalWrite(ledPin, LOW);

    TemperatureSensor.reset();       // reset one wire buss
    TemperatureSensor.skip();        // select only device
    TemperatureSensor.write(0x44, 1);   // start conversion
 
    delay(1000);                     // wait for the conversion
 
    TemperatureSensor.reset();
    TemperatureSensor.skip();
    TemperatureSensor.write(0xBE);   // Read Scratchpad
    for ( i = 0; i < 9; i++) {       // 9 bytes
      data[i] = TemperatureSensor.read();
    }
 
    // Convert the data to actual temperature
    raw = (data[1] << 8) | data[0];
    celsius = (float)raw / 16.0;
    //fahrenheit = celsius * 1.8 + 32.0;


  //oled.clear();
  oled.setFont(FONT16X32);
  oled.setCursor(0, 0);
  //temperature = random(-1600,3800) / 100;
  if (celsius>0) {  oled.print(F(","));}
  else           {  oled.print(F("-"));}
  oled.print(abs(celsius)); //wrap strings in F() to save RAM!
  oled.print(F("/:"));  
  delay(10);


// Affiche la tension de la batterie
/*  oled.clear();
  oled.setFont(FONT16X32);
  oled.setCursor(0, 0);
  oled.print(readVcc());
  delay(1000);
*/


  data2send[2] = (celsius==abs(celsius) ? 0x01 : 0x00);
  data2send[3] = (int)celsius;
  data2send[4] = (celsius-(int)celsius)*100;
  data2send[5] = data2send[3]+data2send[4];
  //man.transmitArray(BUFFER_SIZE, data2send);

/*
  for(int i=0; i<BUFFER_SIZE; i++)
  {
      man.transmit(data2send[i]);
      delay(100);
  }
*/
}



