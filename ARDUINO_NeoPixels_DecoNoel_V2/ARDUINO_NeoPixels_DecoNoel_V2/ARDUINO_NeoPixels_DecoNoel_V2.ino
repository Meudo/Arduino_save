#include <Adafruit_NeoPixel.h>

// https://learn.adafruit.com/multi-tasking-the-arduino-part-3/put-it-all-together-dot-dot-dot?view=all

// Pattern types supported:
//enum  pattern { NONE, RAINBOW_CYCLE, XMAS_RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, SCINTILLE };
enum  pattern { NONE, RAINBOW_CYCLE, XMAS_RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE};
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

#define INTENSITE 0.25

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
    public:

    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;      // total number of steps in the pattern
    uint16_t Index;           // current step within the pattern
    
    void (*OnComplete)();  // Callback on completion of pattern
    
    // Constructor - calls base-class constructor to initialize strip -----------------------------------------------------------------
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }
    
    // Update the pattern ----------------------------------------------------------------------------------------------------------------------------------
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            switch(ActivePattern)
            {
                case RAINBOW_CYCLE:       RainbowCycleUpdate();     break;
                case XMAS_RAINBOW_CYCLE:  XmasRainbowCycleUpdate(); break;
                case THEATER_CHASE:       TheaterChaseUpdate();     break;
                case COLOR_WIPE:          ColorWipeUpdate();        break;
                case SCANNER:             ScannerUpdate();          break;
                case FADE:                FadeUpdate();             break;
                //case SCINTILLE:           Scintille();              break;
                default:  break;
            }
        }
    }
  
    // Increment the Index and reset at the end -----------------------------------------------------------------
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL) { OnComplete();} // call the comlpetion callback
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL) { OnComplete();} // call the comlpetion callback
            }
        }
    }
    
    // Reverse pattern direction ----------------------------------------------------------------------------------------------------------------------------------
    void Reverse()
    {
        if (Direction == FORWARD)
        {
            Direction = REVERSE;
            Index = TotalSteps-1;
        }
        else
        {
            Direction = FORWARD;
            Index = 0;
        }
    }
    
    // Initialize for a RainbowCycle ======================================================================================================================================================
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = RAINBOW_CYCLE;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        Direction = dir;
    }
    
    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        }
        show();
        Increment();
    }
    
    // Initialize for a RainbowCycle ======================================================================================================================================================
    void XmasRainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = XMAS_RAINBOW_CYCLE;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        Direction = dir;
    }
    
    // Update the Rainbow Cycle Pattern
    void XmasRainbowCycleUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            setPixelColor(i, XmasWheel(((i * 256 / numPixels()) + Index) & 255));
        }
        show();
        Increment();
    }

    // Initialize for a Theater Chase ======================================================================================================================================================
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = THEATER_CHASE;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color1;
        Color2 = color2;
        Index = 0;
        Direction = dir;
   }
    
    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            if ((i + Index) % 3 == 0) { setPixelColor(i, Color1); }
            else                      { setPixelColor(i, Color2); }
        }
        show();
        Increment();
    }

    // Initialize for a ColorWipe ======================================================================================================================================================
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = COLOR_WIPE;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color;
        Index = 0;
        Direction = dir;
    }
    
    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
        setPixelColor(Index, Color1);
        show();
        Increment();
    }
    
    // Initialize for a SCANNNER ======================================================================================================================================================
    void Scanner(uint32_t color1, uint8_t interval)
    {
        ActivePattern = SCANNER;
        Interval = interval;
        TotalSteps = (numPixels() - 1) * 2;
        Color1 = color1;
        Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    { 
        for (int i = 0; i < numPixels(); i++)
        {
            if      (i == Index)              { setPixelColor(i, Color1); }           // Scan Pixel to the right
            else if (i == TotalSteps - Index) { setPixelColor(i, Color1); }           // Scan Pixel to the left
            else                              { setPixelColor(i, DimColor(getPixelColor(i))); }// Fading tail
        }
        show();
        Increment();
    }
    
    // Initialize for a Fade ======================================================================================================================================================
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = FADE;
        Interval = interval;
        TotalSteps = steps;
        Color1 = color1;
        Color2 = color2;
        Index = 0;
        Direction = dir;
    }
    
    // Update the Fade Pattern
    void FadeUpdate()
    {
        // Calculate linear interpolation between Color1 and Color2
        // Optimise order of operations to minimize truncation error
        uint8_t red =   ((Red(Color1)   * (TotalSteps - Index)) + (Red(Color2)   * Index)) / TotalSteps;
        uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
        uint8_t blue =  ((Blue(Color1)  * (TotalSteps - Index)) + (Blue(Color2)  * Index)) / TotalSteps;
        
        ColorSet(Color(red, green, blue));
        show();
        Increment();
    }

      // Initialize for a Fade ======================================================================================================================================================
/*    void Scintille(uint32_t color1, uint16_t steps, uint8_t interval)
    {
        ActivePattern = SCINTILLE;
        Interval = interval;
        TotalSteps = steps;
        Color1 = color1;
        Index = 0;
        Direction = dir;
    }*/
    
    // Update the Fade Pattern
/*    void ScintilleUpdate()
    {
        for (int i = 0; i < numPixels(); i++)
        {
            if    (random(steps) == 0) { setPixelColor(i, Color1); }           // Set full color
            else                       { setPixelColor(i, DimColor(getPixelColor(i))); }// Fade color
        }
        show();
        Increment();
    }*/
   
    // Calculate 50% dimmed version of a color (used by ScannerUpdate) *******************************************************************************************
    uint32_t DimColor(uint32_t color)
    {
        // Shift R, G and B components one bit to the right
        uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }

    // Set all pixels to a color (synchronously) *******************************************************************************************
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)     { setPixelColor(i, color); }
        show();
    }

    // Returns the Red component of a 32-bit color *******************************************************************************************
    uint8_t Red(uint32_t color) { return (color >> 16) & 0xFF; }

    // Returns the Green component of a 32-bit color *******************************************************************************************
    uint8_t Green(uint32_t color) { return (color >> 8) & 0xFF; }

    // Returns the Blue component of a 32-bit color *******************************************************************************************
    uint8_t Blue(uint32_t color) { return color & 0xFF; }
    
    // Input a value 0 to 255 to get a color value. *******************************************************************************************
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85) { return Color((255 - WheelPos * 3) * INTENSITE, 0, (WheelPos * 3) * INTENSITE); }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, (WheelPos * 3) * INTENSITE, (255 - WheelPos * 3) * INTENSITE);
        }
        else
        {
            WheelPos -= 170;
            return Color((WheelPos * 3) * INTENSITE, (255 - WheelPos * 3) * INTENSITE, 0);
        }
    }

    // Input a value 0 to 255 to get a color value. *******************************************************************************************
    // The colours are a transition r - g - b - back to r.
    uint32_t XmasWheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85) { return Color((255 - WheelPos * 3) * INTENSITE, 0, 0); }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, (WheelPos * 3) * INTENSITE, 03);
        }
        else
        {
            WheelPos -= 170;
            return Color((WheelPos * 3) * INTENSITE, (WheelPos * 3) * INTENSITE, (WheelPos * 3) * INTENSITE);
        }
    }
};


/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
int CurrentColor = 1;             // Couleur de la LedStrip pour le FADE (rouge, vert et blanc)
int NbIteration = 0;              // Nombre de boucles pour changer de couleurs


void Stick1Complete();
void Stick2Complete();
void Stick3Complete();
void Stick4Complete();
void Stick5Complete();
void Stick6Complete();

// Define some NeoPatterns for the two rings and the stick
//  as well as some completion routines
NeoPatterns Stick1(30,  6,  NEO_GRB + NEO_KHZ800, &Stick1Complete); // Sapin 30
NeoPatterns Stick4(150, 9,  NEO_GRB + NEO_KHZ800, &Stick4Complete); // Sapin 150
NeoPatterns Stick5(150, 10, NEO_GRB + NEO_KHZ800, &Stick5Complete); // Sapin 150
NeoPatterns Stick6(150, 5, NEO_GRB + NEO_KHZ800, &Stick6Complete); // Sapin 150
NeoPatterns Stick2(30,  7,  NEO_GRB + NEO_KHZ800, &Stick2Complete); // Sapin 30
NeoPatterns Stick3(30,  8,  NEO_GRB + NEO_KHZ800, &Stick3Complete); // Sapin 30

// Initialize everything and prepare to start *****************************************************************************************
void setup()
{
    pattern KickOffPatern = FADE; //NONE, XMAS_RAINBOW_CYCLE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, SCINTILLE
    Serial.begin(115200);

    // Initialize all the pixelStrips
    Stick1.begin();
    Stick2.begin();
    Stick3.begin();
    Stick4.begin();
    Stick5.begin();
    Stick6.begin();
    
    // Kick off a pattern
    switch (KickOffPatern)
    {
        case COLOR_WIPE:
            Stick1.ColorWipe(Stick1.Wheel(random(255)), 1);
            Stick2.ColorWipe(Stick1.Color1,             1);
            Stick3.ColorWipe(Stick1.Color1,             1);
            Stick4.ColorWipe(Stick1.Color1,             1);
            Stick5.ColorWipe(Stick1.Color1,             1);
            Stick6.ColorWipe(Stick1.Color1,             1);
        break;

        case FADE:
            Stick1.Fade(Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), 255, 1);
            Stick2.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick3.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick4.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick5.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick6.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
        break;

        case THEATER_CHASE:
            Stick1.TheaterChase(Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), 100);
            Stick2.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick3.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick4.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick5.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick6.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
        break;

        case RAINBOW_CYCLE:
            Stick1.RainbowCycle(3);
            Stick2.RainbowCycle(3);
            Stick3.RainbowCycle(3);
            Stick4.RainbowCycle(3);
            Stick5.RainbowCycle(3);
            Stick6.RainbowCycle(3);
        break;

        case XMAS_RAINBOW_CYCLE:
            Stick1.XmasRainbowCycle(3);
            Stick2.XmasRainbowCycle(3);
            Stick3.XmasRainbowCycle(3);
            Stick4.XmasRainbowCycle(3);
            Stick5.XmasRainbowCycle(3);
            Stick6.XmasRainbowCycle(3);
        break;

        case SCANNER:
            Stick1.Scanner(Stick1.Color(0,255 * INTENSITE,0), 70);
            Stick2.Scanner(Stick1.Color(255 * INTENSITE,255 * INTENSITE,255 * INTENSITE), 130);
            Stick3.Scanner(Stick1.Color(255 * INTENSITE,0,0), 100);
            Stick4.Scanner(Stick1.Color1,         1);
            Stick5.Scanner(Stick1.Color1,         1);
            Stick6.Scanner(Stick1.Color1,         1);
        break;

/*        case SCINTILLE:
            Stick1.Scintille(Stick1.Color(255,255,255), Stick1.numPixels/10);
            Stick2.Scintille(Stick1.Color(255,255,255), Stick2.numPixels/10);
            Stick3.Scintille(Stick1.Color(255,255,255), Stick3.numPixels/10);
            Stick4.Scintille(Stick1.Color(255,255,255), Stick4.numPixels/10);
            Stick5.Scintille(Stick1.Color(255,255,255), Stick5.numPixels/10);
            Stick6.Scintille(Stick1.Color(255,255,255), Stick6.numPixels/10);
        break;
*/
        default: break;
    }
  Stick4.RainbowCycle(3);
}

// Main loop **********************************************************************************************************************************************************************************
void loop()
{

  Stick1.Update();
  Stick2.Update();
  Stick3.Update();
  Stick4.Update();
  Stick5.Update();
  Stick6.Update();
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Stick1 Completion Callback *****************************************************************************************
void Stick1Complete()       // COLOR_WIPE, FADE, THEATER_CHASE, RAINBOW_CYCLE, XMAS_RAINBOW_CYCLE, SCANNER
{
    if (NbIteration++ < 6)  
    {
      switch (CurrentColor)
      {
        default:     CurrentColor=1;
        case 1: /*red*/   Stick1.Fade(Stick1.Color2, Stick1.Color(255 * INTENSITE,0,0),     255, 1); break;
        case 2: /*green*/ Stick1.Fade(Stick1.Color2, Stick1.Color(0,255 * INTENSITE,0),     255, 1); break;
        case 3: /*white*/ Stick1.Fade(Stick1.Color2, Stick1.Color(255 * INTENSITE,255 * INTENSITE,255 * INTENSITE), 255, 1); break;
      }
  
      CurrentColor++;
      
      // Random color change for next scan
      //Stick1.Fade(Stick1.Color2,             Stick1.Wheel(random(255)), 255, 1);
      Stick2.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
      Stick3.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
      Stick4.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
      Stick5.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
      Stick6.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
    }
    else if (NbIteration < 12)
    {
      Stick1.RainbowCycle(3);
      Stick2.RainbowCycle(3);
      Stick3.RainbowCycle(3);
      Stick4.RainbowCycle(3);
      Stick5.RainbowCycle(3);
      Stick6.RainbowCycle(3);
    }
    else if (NbIteration > 18) NbIteration=0;
    else
    {
      Stick1.XmasRainbowCycle(3);
      Stick2.XmasRainbowCycle(3);
      Stick3.XmasRainbowCycle(3);
      Stick4.XmasRainbowCycle(3);
      Stick5.XmasRainbowCycle(3);
      Stick6.XmasRainbowCycle(3);
    }

/*
         case COLOR_WIPE:
            Stick1.ColorWipe(Stick1.Wheel(random(255)), 1);
            Stick2.ColorWipe(Stick1.Color1,             1);
            Stick3.ColorWipe(Stick1.Color1,             1);
            Stick4.ColorWipe(Stick1.Color1,             1);
            Stick5.ColorWipe(Stick1.Color1,             1);
            Stick6.ColorWipe(Stick1.Color1,             1);
        break;

        case FADE:
            Stick1.Fade(Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), 255, 1);
            Stick2.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick3.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick4.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick5.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
            Stick6.Fade(Stick1.Color1,             Stick1.Color2,             255, 1);
        break;

        case THEATER_CHASE:
            Stick1.TheaterChase(Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), 100);
            Stick2.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick3.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick4.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick5.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
            Stick6.TheaterChase(Stick1.Color1,             Stick1.Color2,             100);
        break;

        case RAINBOW_CYCLE:
            Stick1.RainbowCycle(3);
            Stick2.RainbowCycle(3);
            Stick3.RainbowCycle(3);
            Stick4.RainbowCycle(3);
            Stick5.RainbowCycle(3);
            Stick6.RainbowCycle(3);
        break;

        case XMAS_RAINBOW_CYCLE:
            Stick1.XmasRainbowCycle(3);
            Stick2.XmasRainbowCycle(3);
            Stick3.XmasRainbowCycle(3);
            Stick4.XmasRainbowCycle(3);
            Stick5.XmasRainbowCycle(3);
            Stick6.XmasRainbowCycle(3);
        break;

        case SCANNER:
            Stick1.Scanner(Stick1.Color(0,255,0), 70);
            Stick2.Scanner(Stick1.Color(255,255,255), 130);
            Stick3.Scanner(Stick1.Color(255,0,0), 100);
            Stick4.Scanner(Stick1.Color1,         1);
            Stick5.Scanner(Stick1.Color1,         1);
            Stick6.Scanner(Stick1.Color1,         1);
        break;
 */



}

// Stick2 Completion Callback *****************************************************************************************
void Stick2Complete()
{
    // Random color change for next scan
}

// Stick3 Completion Callback *****************************************************************************************
void Stick3Complete()
{
    // Random color change for next scan
}

// Stick4 Completion Callback *****************************************************************************************
void Stick4Complete()
{
    // Random color change for next scan
}

// Stick5 Completion Callback *****************************************************************************************
void Stick5Complete()
{
    // Random color change for next scan
}

// Stick6 Completion Callback *****************************************************************************************
void Stick6Complete()
{
    // Random color change for next scan
}
