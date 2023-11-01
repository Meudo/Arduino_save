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

/******** TemperatureSonde ************/
//#include <OneWire.h>
/******** TemperatureSonde ************/

/******** Tranmission *******************/
#include <VirtualWire.h>
/******** Tranmission *******************/

/******** Ecran ************/
//#include <Tiny4kOLED.h>
//#include <TinyWireM.h>
/******** Ecran ************/

/********** Led Blink ************/
int ledPin=3;
/********** Led Blink ************/

/********** Général ************/
#define TEMPS_PAUSE 100
/********** Général ************/



/*********** TemperatureSonde ******************/
//#define ONEWIRE_BUSS 4
//OneWire TemperatureSensor(ONEWIRE_BUSS);
/*********** Temperature ******************/

/*********** Transmission ******************/
#define NO_MODULE 13
#define TX_PIN 1  //pin where your transmitter is connected
typedef struct {
  char NoModule=NO_MODULE;
  float Temperature=0.0;
} MaStructure;
int cpt=0;
/*********** Transmission ******************/


/*********** Niveau Batterie ******************/
/*uint16_t readVcc(void) {
  uint16_t result;
  // Read 1.1V reference against Vcc
  ADMUX = (0<<REFS0) | (12<<MUX0);
  delay(2); // Wait for Vref to settle
  ADCSRA |= (1<<ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCW;
  return 1018500L / result; // Back-calculate AVcc in mV
}*/
/*********** Niveau Batterie ******************/


void setup()  ///////////////////// Setup ///////////////////////
{
/******** Ecran ************/  
/*  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT16X32); // 1 raw of 8 characters exactly fills 128x32

  oled.setFont(FONT8X16); // 2 raw of 16 characters exactly fills 128x32
  oled.setCursor(0, 0);
  oled.print(F("Temperature")); //wrap strings in F() to save RAM!
  oled.setCursor(0, 2);
  oled.print(F("Celsus")); //wrap strings in F() to save RAM!*/
/******** Ecran ************/

/******** Led Blink ************/
  pinMode(ledPin, OUTPUT); 
  digitalWrite(ledPin, LOW); 
/******** Led Blink ************/

/******** Transmission ************/
  vw_set_tx_pin(TX_PIN);
  vw_setup(2000);
/******** Transmission ************/

} ///////////////////// End Setup ///////////////////////

void loop()  ///////////////////// End Loop ///////////////////////
{
//    byte i;
 
/******** Temperature ************/
/*    byte DataTemperature[12];
    int16_t rawTemperature;
    float Celsius;
    //float Fahrenheit;*/
/******** Temperature ************/

/******** Transmission ************/
  MaStructure Message;
/******** Transmission ************/

/******** Led Blink ************/
  digitalWrite(ledPin, HIGH); 
  delay(10);
  digitalWrite(ledPin, LOW);
/******** Led Blink ************/

/******** Temperature ************/
/*    TemperatureSensor.reset();       // reset one wire buss
    TemperatureSensor.skip();        // select only device
    TemperatureSensor.write(0x44, 1);   // start conversion
 
    delay(1000);                     // wait for the conversion
 
    TemperatureSensor.reset();
    TemperatureSensor.skip();
    TemperatureSensor.write(0xBE);   // Read Scratchpad
    for ( i = 0; i < 9; i++) {       // 9 bytes
      DataTemperature[i] = TemperatureSensor.read();
    }
 
    // Convert the data to actual temperature
    rawTemperature = (DataTemperature[1] << 8) | DataTemperature[0];
    Celsius = (float)rawTemperature / 16.0;
    //Fahrenheit = Celsius * 1.8 + 32.0;*/
/******** Temperature ************/


/********** Ecran **************/
//oled.clear();
/*  oled.setFont(FONT16X32);
  oled.setCursor(0, 0);
  if (Celsius>0) {  oled.print(F(","));}
  else           {  oled.print(F("-"));}
  oled.print(abs(Celsius)); //wrap strings in F() to save RAM!
  oled.print(F("/:"));  
  delay(1000);

// Affiche la tension de la batterie
  oled.clear();
  oled.setFont(FONT16X32);
  oled.setCursor(0, 0);
  oled.print(readVcc());
  delay(10);*/
/********** Ecran **************/


/************ Transmission ************/
//  Message.Temperature = Celsius;
//  vw_send((byte*) &Message, sizeof(Message)); // On envoie le message
  vw_send((byte*) cpt++, sizeof(cpt)); // On envoie le message
  vw_wait_tx(); // On attend la fin de l'envoi
  delay(TEMPS_PAUSE);
/************ Transmission ************/

} ///////////////////// End Loop ///////////////////////

