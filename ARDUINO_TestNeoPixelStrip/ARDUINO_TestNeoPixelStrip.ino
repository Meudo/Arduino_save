
#include <Adafruit_NeoPixel.h>
// https://learn.adafruit.com/adafruit-neopixel-uberguide/neomatrix-library?view=all

#define STRIP1_PIN 6
#define STRIP2_PIN 7
#define STRIP1_NB_PIEXELS 16
#define STRIP2_NB_PIEXELS 150

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(STRIP1_NB_PIEXELS, STRIP1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(STRIP2_NB_PIEXELS, STRIP2_PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip1.Color(255, 0, 255);
uint32_t red = strip1.Color(255, 0, 0);
uint32_t green = strip1.Color(0, 255, 0);
uint32_t blue = strip1.Color(0, 0, 255);
uint32_t white = strip1.Color(255, 255, 255);

int Brightness=250;
int Increment=-1;


void setup()
{
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
}

void loop()
{
/*  strip.setBrightness(Brightness);                  // set brightness to all pixels 0-255
  strip.show();
  delay (5);
 
  if ((Brightness<=2) || (Brightness>254) ) {Increment=-Increment;}

  Brightness+=Increment;*/

  //uint32_t color = strip.getPixelColor(11); // get color of a pixel
  //uint16_t n = strip.numPixels();           // get number of pixels in the strip
  //strip.setBrightness(64);                  // set brightness to all pixels 0-255

  rainbow(20);
  rainbowCycle(20);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip1.numPixels(); i++) {
      strip1.setPixelColor(i, Wheel1((i+j) & 255));
    }
    strip1.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip2.numPixels(); i++) {
      strip2.setPixelColor(i, Wheel2(((i * 256 / strip2.numPixels()) + j) & 255));
    }
    strip2.show();
    delay(wait);
  }
}




// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel1(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel2(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}



