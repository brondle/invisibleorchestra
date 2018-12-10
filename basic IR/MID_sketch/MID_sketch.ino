/*
SharpDistSensorArray.ino

Source: https://github.com/DrGFreeman/SharpDistSensor

*/


#include <SharpDistSensor.h>

#include <Adafruit_NeoPixel.h>
//NEOPIXEL Multi-pattern stuff
// Pattern types supported:


/* LED BEGIN */ 
#define PIN 3
#define PIN_2 5
#define PIN_3 9
#define PIN_4 11
#define NUM_LEDS 108
#define BRIGHTNESS 50


uint32_t c_yellow, c_red, c_blue, c_green, c_white, c_magenta, c_lightblue;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN_3, NEO_GRB + NEO_KHZ800);


/* LED END */ 

/* IR Sensor BEGIN */
// Define the number of sensors in the array as a constant
const byte nbSensors = 4; //4;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 21;

#define midiChannel (char)0

// Define the array of SharpDistSensor objects
SharpDistSensor sensorArray[] = {
  SharpDistSensor(A0, medianFilterWindowSize), // First sensor using pin A0
  SharpDistSensor(A1, medianFilterWindowSize), // Second sensor using pin A1
  SharpDistSensor(A2, medianFilterWindowSize), // Third sensor using pin A2
  SharpDistSensor(A3, medianFilterWindowSize), // Fourth sensor using pin A3
//  // Add as many sensors as required
};

// Define an array of integers that will store the measured distances
uint16_t distArray[nbSensors];
uint16_t oldNote1[3];
uint16_t oldNote2[3];
char hexval[5];

//MIDI stuff
//const byte notePitches[] = {C3, D3, E3, F3, G3, A3, B3};
uint8_t intensity;
uint8_t intensity2;
uint8_t oldIntensity1;
uint8_t oldIntensity2;
uint8_t note;
uint8_t note2;

uint16_t timer = 0;

/* IR Sensor END */

void setup() {
  // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }
   Serial.begin(115200);
   delay(1000);
  c_red     = strip.Color(255,0,0);
  c_blue    = strip.Color(0, 0, 255);
  c_lightblue = strip.Color(0, 0, 100);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip2.setBrightness(BRIGHTNESS);
  strip2.begin();
  strip2.show(); // Initialize all pixels to 'off'
    strip3.setBrightness(BRIGHTNESS);
  strip3.begin();
  strip3.show(); // Initialize all pixels to 'off'
  
}

void loop() {
    // Read distance for each sensor in array into an array of distances
    for (byte i = 0; i < nbSensors; i++) {
      distArray[i] = sensorArray[i].getDist();
    }
      readIntensity(distArray[2], distArray[3]);
      readNotes(distArray[0], distArray[1]);
      playNotes();

//     
     Serial.print(distArray[0]); // Print A0 distance to Serial
    Serial.print(",");
    Serial.print(distArray[1]); // Print A1 distance to Serial
    Serial.print(",");
 Serial.println(distArray[2]); // Print A2 distance to Serial
    Serial.print(",");
    Serial.println(distArray[3]); // Print A3 distance to Serial  
//  
    int a = map(distArray[1], 1000, 3000, 0, 108);
    constrain(a, 0, 108);
    int b = map(distArray[2], 1000, 3000, 0, 108);
    constrain(b, 0, 108);
    int c = map(distArray[3], 1000, 3000, 0, 108);
    colorWipe(round10(a)-10, round10(a),  c_blue);


}


void colorWipe(uint32_t from1, uint32_t to1, uint32_t c) {
  for(uint16_t i=0;i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c_lightblue); 
    strip2.setPixelColor(i, c_lightblue);
    strip3.setPixelColor(i, c_lightblue);
  }
  for(uint16_t i=from1; i<to1; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    strip2.setPixelColor(i, c);
    strip2.show();
    strip3.setPixelColor(i, c);
    strip3.show();
  }
  
}

void readIntensity(int val, int val2)
{
  intensity = (uint8_t) (map(val, 1000, 5000, 0x00, 0x90));
  intensity2 = (uint8_t) (map(val2, 1000, 5000, 0x00, 0x90));
}

void readNotes(int val, int val2)
{
  note = (uint8_t) (map(val, 1000, 3000, 37, 59));
  note2 = (uint8_t) (map(val2, 1000, 3000, 37, 59));
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
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


void Strip2Complete() {
  
}


// Define some NeoPatterns for the two rings and the stick
// Initialize everything and prepare to start
