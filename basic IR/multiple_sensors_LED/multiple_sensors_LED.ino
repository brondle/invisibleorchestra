/*
SharpDistSensorArray.ino

Source: https://github.com/DrGFreeman/SharpDistSensor

*/

#include <SharpDistSensor.h>

#include <Adafruit_NeoPixel.h>

/* LED BEGIN */ 
#define PIN 3
#define PIN_2 9
#define NUM_LEDS 100
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

/* IR Sensor END */


void setup() {
  // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }

  Serial.begin(9600);
  delay(1000);
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
    }
     
    Serial.print(distArray[0]); // Print A0 distance to Serial
    Serial.print(",");
    Serial.print(distArray[1]); // Print A1 distance to Serial
    Serial.print(",");
    Serial.print(distArray[2]); // Print A2 distance to Serial
    Serial.print(",");
    Serial.println(distArray[3]); // Print A3 distance to Serial  
  
    int a = 65; //map(distArray[1], 1100, 2500, 0, 100);
    constrain(a, 0, 100);
    colorWipe(round15(a)-10, round15(a), c_blue, BLINK_WAIT); 

    // Wait some time
    delay(50);
}


void colorWipe(uint32_t from, uint32_t to, uint32_t c, uint8_t wait) {
  for(uint16_t i=0;i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c_lightblue); 
    strip2.setPixelColor(i, c_lightblue);
  }
  for(uint16_t i=from; i<to; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    strip2.setPixelColor(i, c);
    strip2.show();
    delay(50);
  }
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}
