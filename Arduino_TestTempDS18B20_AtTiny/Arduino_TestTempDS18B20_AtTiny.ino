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

#include <Tiny4kOLED.h>
#include <TinyWireM.h>
#include <OneWire.h>

// ============================================================================

int ledPin=3;
float temperature = 0;

#define ONEWIRE_BUSS 4
OneWire TemperatureSensor(ONEWIRE_BUSS);


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
    TemperatureSensor.write(0x44);   // start conversion
 
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


  oled.clear();
  oled.setFont(FONT16X32);
  oled.setCursor(0, 0);
  //temperature = random(-1600,3800) / 100;
  oled.print(celsius); //wrap strings in F() to save RAM!
  oled.setFont(FONT8X16);
  oled.setCursor((8*10), 2); 
  if (celsius>0) {  oled.print("+");}
  else           {  oled.print("-");}
  delay(10);
}
