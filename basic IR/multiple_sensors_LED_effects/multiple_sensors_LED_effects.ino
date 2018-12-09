/*
SharpDistSensorArray.ino

Source: https://github.com/DrGFreeman/SharpDistSensor

*/

#include <SharpDistSensor.h>

#include <Adafruit_NeoPixel.h>

/* LED BEGIN */ 
#define PIN 3
#define PIN_2 9
#define NUM_LEDS 108
#define BRIGHTNESS 50
#define BLINK_WAIT 10

uint32_t c_yellow, c_red, c_blue, c_green, c_white, c_magenta, c_lightblue;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN_2, NEO_GRB + NEO_KHZ800);

/* LED END */ 

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
uint16_t oldArray[nbSensors];
int checker = 0;
int timer = 0;

/* IR Sensor END */


void setup() {
  // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }

  Serial.begin(9600);

  c_red     = strip.Color(255,0,0);
  c_blue    = strip.Color(0, 0, 255);
  c_lightblue = strip.Color(0, 0, 100);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip2.begin();
  strip2.show();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
    // Read distance for each sensor in array into an array of distances
    for (byte i = 0; i < nbSensors; i++) {
      distArray[i] = sensorArray[i].getDist();
//      if (distArray[i] != oldArray[i]) {
//        oldArray[i] = distArray[i];
//        checker++;
//      }
    }

//    if (checker >= 2) {
     /*
    Serial.print(distArray[0]); // Print A0 distance to Serial
    Serial.print(",");
    Serial.print(distArray[1]); // Print A1 distance to Serial
    Serial.print(",");
    Serial.print(distArray[2]); // Print A2 distance to Serial
    Serial.print(",");
    Serial.println(distArray[3]); // Print A3 distance to Serial  
 */
    int a = map(distArray[0], 1002, 2500, 0, NUM_LEDS);
    a = constrain(a, 0, NUM_LEDS);
    
    Serial.println(a);
    
    
     
   //  colorWipe(round10(a)-10, 0, round10(a), 0, c_blue, BLINK_WAIT);
   
   
//    checker = 0;
//    }
    //allow for same not to be played every half-second
//    if ((millis() - timer) > 500) {
//       for (byte i = 0; i < nbSensors; i++) {
//          oldArray[i] = 0;
//      }
//      timer+=500;
//    }
    // Wait some time
   /* delay(50);*/
}
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
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
   
    showStrip();
  
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}





void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    
    setAll(0,0,0);
    showStrip();
    
  }
 

}


void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
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
  showStrip();
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}
