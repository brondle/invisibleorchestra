#include <Adafruit_NeoPixel.h>
#include <SharpDistSensor.h>

/* LED BEGIN */ 
#define PIN_3 3
#define PIN_5 5
#define PIN_9 9
#define PIN_11 11

#define NUM_LEDS 70 
int a, b, c, d;

// Pattern types supported:
enum  pattern { NONE, BLINK, METEOR, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };


class NeoPatterns : public Adafruit_NeoPixel
{
    public:
 
    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    
    void (*OnComplete)();  // Callback on completion of pattern
    
    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }
    
    // Update the pattern
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            
            switch(ActivePattern)
            {
                case BLINK:
                  BlinkUpdate();
                  break;
                case METEOR:
                  meteorRainUpdate();
                  break; 
                case COLOR_WIPE:
                  ColorWipeUpdate();
                  break;                 
                default:
                    break;
            }
        }
    }
  
    // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }
    
    // Reverse pattern direction
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

    void Blink(){
        ActivePattern = BLINK;
     
    }

    void BlinkUpdate() {

      
      int Pixel = random(NUM_LEDS);

      
      setPixel(Pixel,0xff,0xff,0xff);
      show();
      setPixel(Pixel,0,0,0);
      
      /*
      setPixel(Pixel,255,0,0);
      show();
      setPixel(Pixel,0,255,0);
*/

      
      Increment();
    }


 // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = COLOR_WIPE;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color;
        Index = 0;
        Direction = dir;
    }
    
    //- Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {      
        show();
        Increment();        
    }

    
    void meteorRain(uint8_t interval) {
      ActivePattern = METEOR;
      Direction = FORWARD;
      Interval = interval;
    }
 
   void meteorRainUpdate() {  

    byte red = 0xff;
    byte green = 0xff;
    byte blue = 0xff;
    byte meteorSize = 10;
    byte meteorTrailDecay = 64;
    boolean meteorRandomDecay = true;
    int SpeedDelay = 30;
    setAll(0,0,0);
    
    for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
      
      
      // fade brightness all LEDs one step
      for(int j=0; j<NUM_LEDS; j++) {

        if( (!meteorRandomDecay) || (random(10)>5) ) {
          fadeToBlack(j, meteorTrailDecay );      
            
        }
      }
      
      // draw meteor
      for(int j = 0; j < meteorSize; j++) {
        if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
          setPixel(i-j, red, green, blue);
        } 
      }
    
      show();
      Increment();
    }
  }
  
  void fadeToBlack(int ledNo, byte fadeValue) {
   #ifdef ADAFRUIT_NEOPIXEL_H 
      // NeoPixel
      uint32_t oldColor;
      uint8_t r, g, b;
      int value;
      
      oldColor = getPixelColor(ledNo);
      r = (oldColor & 0x00ff0000UL) >> 16;
      g = (oldColor & 0x0000ff00UL) >> 8;
      b = (oldColor & 0x000000ffUL);
  
      r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
      g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
      b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
      
      setPixelColor(ledNo, r,g,b);
   #endif 
  }
    
   void setPixel(int Pixel, byte red, byte green, byte blue) {
       #ifdef ADAFRUIT_NEOPIXEL_H 
         // NeoPixel
         setPixelColor(Pixel, Color(red, green, blue));
       #endif
       #ifndef ADAFRUIT_NEOPIXEL_H 
         // FastLED
         leds[Pixel].r = red;
         leds[Pixel].g = green;
         leds[Pixel].b = blue;
       #endif
    }

    void setAll(byte red, byte green, byte blue) {
      for(int i = 0; i < NUM_LEDS; i++ ) {
        setPixel(i, red, green, blue); 
      }
      show();
    }
    
    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
        // Shift R, G and B components one bit to the right
        uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
        return dimColor;
    }
 
    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
        for (int i = 0; i < numPixels(); i++)
        {
            setPixelColor(i, color);
        }
        show();
    }
 
    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
        return (color >> 16) & 0xFF;
    }
 
    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
        return (color >> 8) & 0xFF;
    }
 
    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
        return color & 0xFF;
    }
    
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
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

void Ring1Complete();
void Ring2Complete();
void Ring3Complete();
void Ring4Complete();
NeoPatterns Ring1(NUM_LEDS, PIN_3, NEO_GRB + NEO_KHZ800, &Ring1Complete);
NeoPatterns Ring2(NUM_LEDS, PIN_5, NEO_GRB + NEO_KHZ800, &Ring2Complete);
NeoPatterns Ring3(NUM_LEDS, PIN_9, NEO_GRB + NEO_KHZ800, &Ring3Complete);
NeoPatterns Ring4(NUM_LEDS, PIN_11, NEO_GRB + NEO_KHZ800, &Ring4Complete);



/* IR Sensor BEGIN */
// Define the number of sensors in the array as a constant
const byte nbSensors = 4;



// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 21;

// Define the array of SharpDistSensor objects
SharpDistSensor sensorArray[] = {
  SharpDistSensor(A0, medianFilterWindowSize), // First sensor using pin A0
  SharpDistSensor(A1, medianFilterWindowSize), // Second sensor using pin A1
  SharpDistSensor(A2, medianFilterWindowSize), // Third sensor using pin A2
  SharpDistSensor(A3, medianFilterWindowSize), // Fourth sensor using pin A3
  // Add as many sensors as required
};

// Define an array of integers that will store the measured distances
uint16_t distArray[nbSensors];
uint16_t oldNote1[3];
uint16_t oldNote2[3];
char hexval[5];

uint8_t intensity;
uint8_t intensity2;
uint8_t oldIntensity1;
uint8_t oldIntensity2;
uint8_t note;
uint8_t note2;

int timer = 0;

/* IR Sensor END */

#define midiChannel (char)0

void setup()
{
  Serial.begin(112500);
   // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }
 
//   pinMode(PIN_3, INPUT_PULLUP);
//   pinMode(PIN_5, INPUT_PULLUP);
//   pinMode(PIN_9, INPUT_PULLUP);
//   
   Ring1.begin();
   Ring1.clear();
   Ring1.show();
   Ring1.Blink();
  
   Ring2.begin();
   Ring2.clear();
   Ring2.show();
   Ring2.Blink();

   
   Ring3.begin();
   Ring3.clear();
   Ring3.show();
   Ring3.Blink();

   Ring4.begin();
   Ring4.clear();
   Ring4.show();
   Ring4.Blink();


}


void loop()
{

  for (byte i = 0; i < nbSensors; i++) {
      distArray[i] = sensorArray[i].getDist();
  }
  
  a = map(distArray[0], 1002, 5000, 0, 300);
  b = map(distArray[1], 1002, 5000, 0, 300);
  c = map(distArray[2], 1002, 5000, 0, 300);
  d = map(distArray[3], 1002, 5000, 0, 300);
    
  
  /*  
    Serial.println(distArray[0]);
    //a = constrain(a, 0, NUM_LEDS);
  */  
//     Serial.print(distArray[0]); // Print A0 distance to Serial
//    Serial.print(",");
//    Serial.print(distArray[1]); // Print A1 distance to Serial
//    Serial.print(",");
// Serial.println(distArray[2]); // Print A2 distance to Serial
//    Serial.print(",");
//    Serial.println(distArray[3]); // Print A3 distance to Serial 

   readIntensity(distArray[0], distArray[3]);
   readNotes(distArray[2], distArray[1]);
   playNotes();
   delay(10);



  // Update the rings.
    Ring1.Update();
    Ring2.Update(); 
    Ring3.Update();
    Ring4.Update();
}

void Ring1Complete()
{   
  
  Ring1.Interval = a;

  /*
   for(uint16_t i=0;i<Ring1.numPixels(); i++) {
    Ring1.setPixelColor(i, Ring1.Color(0,0,255)); 
    }
      
    for(uint16_t i=round10(a)-10; i<round10(a); i++) {
      Ring1.setPixelColor(i,  Ring1.Color(255,0,0));
    }
    Ring1.Interval = 10;
    */
}


void Ring2Complete()
{   
  Ring2.Interval = b;
}


void Ring3Complete()
{   
  Ring3.Interval = c;
}

void Ring4Complete()
{   
  Ring4.Interval = d;
}
void readIntensity(int val, int val2)
{
  intensity = (uint8_t) (map(val, 1000, 5000, 0x00, 0x90));
  intensity2 = (uint8_t) (map(val2, 1000, 5000, 0x00, 0x90));
}

void readNotes(int val, int val2)
{
  note = (uint8_t) (map(val, 1000, 4000, 48, 59));
  note2 = (uint8_t) (map(val2, 1000, 4000, 48, 59));
}

void playNotes() {
//     Serial.println(1);
     noteOn(0x90, note, intensity);
 //    noteOff(oldNote1[0], oldNote1[1], oldNote1[2]);
//     oldNote1[0] = 4;
//     oldNote1[1] = note;
//     oldNote1[2] = intensity;
 //   delay(50);
//    Serial.println(2);
     noteOn(0x91, note2, intensity2);
//     oldNote2[0] = 2;
//     oldNote1[1] = note2;
//     oldNote1[2] = intensity2;
}

void noteOn(byte channel, byte pitch, byte velocity) {
//  Serial.print(channel);
//  Serial.print(",");
//  Serial.print(pitch);
//  Serial.print(",");
//  Serial.println(velocity);


  Serial.write(channel);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOff(byte channel, byte pitch, byte velocity) {
}



int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}
