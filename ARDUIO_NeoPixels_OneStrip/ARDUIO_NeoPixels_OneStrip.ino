#include <Adafruit_NeoPixel.h>
#define INTERVAL_RAINBOW 3    //1
#define INTERVAL_THEATER 200  //1
#define INTERVAL_WIPE 50      //100
#define INTERVAL_SCANNER 100  //10
#define INTERVAL_FADE 10      //1
#define INTERVAL_STARS 100      //1
#define STEPS_FADE 255
#define STEPS_STARS 1


// https://learn.adafruit.com/multi-tasking-the-arduino-part-3/put-it-all-together-dot-dot-dot?view=all

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE, STARS };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

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
                case RAINBOW_CYCLE: RainbowCycleUpdate(); break;
                case THEATER_CHASE: TheaterChaseUpdate(); break;
                case COLOR_WIPE:    ColorWipeUpdate();    break;
                case SCANNER:       ScannerUpdate();      break;
                case FADE:          FadeUpdate();         break;
                case STARS:         StarsUpdate();        break;
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
   
    // Initialize for a Stars ======================================================================================================================================================
    void Stars(uint32_t color1, uint16_t steps, uint8_t interval)
    {
        ActivePattern = STARS;
        Interval = interval;
        TotalSteps = steps;
        Color1 = color1;
        Index = 0;
        
        setPixelColor(random(numPixels()), Color1);
    }

    // Update the Scanner Pattern
    void StarsUpdate()
    { 
        uint32_t color;
        
        for (int i = 0; i < numPixels(); i++)
        {
            color = getPixelColor(i);
            if (color != 0) { setPixelColor(i, DimColor(getPixelColor(i))); } // Fading
        }
        show();
        Increment();
    }
    
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
        if(WheelPos < 85) { return Color(255 - WheelPos * 3, 0, WheelPos * 3); }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }
};


/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
int ledPin=13;                    // LED connected to digital pin 13
int AlimPowered = 2;              // Tells if Ailm is powered-up
int PowerUpAlim = 3;              // Powered-up the Alim
int Button = 4;                   // Button to change animation


void Stick1Complete();

//NeoPatterns Stick1(300, 6, NEO_GRB + NEO_KHZ800, &Stick1Complete);
NeoPatterns Stick1(16, 6, NEO_GRB + NEO_KHZ800, &Stick1Complete);

// Initialize everything and prepare to start *****************************************************************************************
void setup()
{
  Serial.begin(115200);

   pinMode(AlimPowered, INPUT);             // High if Alim powered-up
   pinMode(Button,      INPUT_PULLUP);      // Button to change animation
   pinMode(ledPin,      OUTPUT);            // sets the digital pin as output
   pinMode(PowerUpAlim, OUTPUT);            // sets the digital pin as output
 
    // Initialize all the pixelStrips
    Stick1.begin();
    
    //Stick1.RainbowCycle   (INTERVAL_RAINBOW);
    //Stick1.TheaterChase (Stick1.Wheel(random(255)), ~Stick1.Color1, INTERVAL_THEATER); //Stick1.TheaterChase (Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), INTERVAL_THEATER);
    //Stick1.ColorWipe    (Stick1.Wheel(random(255)), INTERVAL_WIPE);
    //Stick1.Scanner      (Stick1.Wheel(random(255)), INTERVAL_SCANNER);
    //Stick1.Fade         (Stick1.Wheel(random(255)), Stick1.Wheel(random(255)), STEPS_FADE, INTERVAL_FADE);
    Stick1.Stars         (Stick1.Wheel(random(255)), STEPS_STARS, INTERVAL_STARS);//Stick1.Stars         (Stick1.Color(255, 255, 255), STEPS_STARS, INTERVAL_STARS);
    //Stick1.setBrightness(20); // 64
}

// Main loop **********************************************************************************************************************************************************************************
void loop()
{
  digitalWrite(ledPin, digitalRead(AlimPowered));       // Sets the LED on if Alim is Powered-up
  digitalWrite(PowerUpAlim, LOW);                       // Power up the Alim

  Stick1.Update();
    
    // Switch patterns on a button press:
    if (digitalRead(Button) == LOW) // Button #1 pressed
    {
        switch(Stick1.ActivePattern)
        {
          case RAINBOW_CYCLE:
            Stick1.ActivePattern = STARS;
            Stick1.Interval = INTERVAL_FADE; //1
            break;

          case THEATER_CHASE:
            Stick1.ActivePattern = RAINBOW_CYCLE;
            Stick1.Interval = INTERVAL_RAINBOW; //1
            //Stick1.TotalSteps = 255;
            break;
            
          case COLOR_WIPE:
            Stick1.ActivePattern = THEATER_CHASE;
            Stick1.Interval = INTERVAL_THEATER; //100
            break;

          case SCANNER:
            Stick1.ActivePattern = COLOR_WIPE;
            Stick1.Interval = INTERVAL_WIPE; //100
            break;

          case FADE:
            Stick1.ActivePattern = SCANNER;
            Stick1.Interval = INTERVAL_SCANNER; //1
            break;

          case STARS:
            Stick1.ActivePattern = FADE;
            Stick1.Interval = INTERVAL_SCANNER; //1
            break;

          default: break;
      }
    }
}

//------------------------------------------------------------
//Completion Routines - get called on completion of a pattern
//------------------------------------------------------------

// Stick1 Completion Callback *****************************************************************************************
void Stick1Complete()
{
    // Random color change for next scan
        switch(Stick1.ActivePattern)
        {
          case RAINBOW_CYCLE:
            break;

          case THEATER_CHASE:
              Stick1.Color1 = Stick1.Wheel(random(255));
              Stick1.Color2 = ~Stick1.Color1;
            break;
            
          case COLOR_WIPE:
              Stick1.Color1 = Stick1.Wheel(random(255));
            break;

          case SCANNER:
              Stick1.Color1 = Stick1.Wheel(random(255));
            break;

          case FADE:
              Stick1.Color1 = Stick1.Color2;
              Stick1.Color2 = Stick1.Wheel(random(255));
            break;

          case STARS:
              Stick1.setPixelColor(random(Stick1.numPixels()), Stick1.Color1);
              Stick1.Color1 = Stick1.Wheel(random(255)); //Stick1.Color1 = Stick1.Color(255, 255, 255);
            break;

          default: break;
      }
}

