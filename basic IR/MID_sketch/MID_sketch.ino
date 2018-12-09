/*
SharpDistSensorArray.ino

Source: https://github.com/DrGFreeman/SharpDistSensor

*/

//#include "MIDIUSB.h"
//#include "PitchToNote.h"
// #include <MIDI.h>

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
uint16_t oldArray[nbSensors];
int checker = 0;
int timer = 0;
uint16_t oldNote1[3];
uint16_t oldNote2[3];
char hexval[5];

//MIDI stuff
//const byte notePitches[] = {C3, D3, E3, F3, G3, A3, B3};
uint8_t intensity;
uint8_t intensity2;
uint8_t note;
uint8_t note2;

/* IR Sensor END */

void setup() {
  // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }
//   MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
   Serial.begin(115200);
   delay(1000);
//  c_red     = strip.Color(255,0,0);
//  c_blue    = strip.Color(0, 0, 255);
//  c_lightblue = strip.Color(0, 0, 100);
//  strip.setBrightness(BRIGHTNESS);
//  strip.begin();
//  strip2.begin();
//  strip2.show();
//  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
    // Read distance for each sensor in array into an array of distances
    for (byte i = 0; i < nbSensors; i++) {
      distArray[i] = sensorArray[i].getDist();
//      if (distArray[i] != oldArray[i]) {
//        oldArray[i] = distArray[i];/
//        checker++;
//      }
    }
      readIntensity(distArray[2], distArray[3]);
      readNotes(distArray[0], distArray[1]);
      playNotes();

//    if (checker >= 2) {
//     
//     Serial.print(distArray[0]); // Print A0 distance to Serial
//    Serial.print(",");
//    Serial.print(distArray[1]); // Print A1 distance to Serial
//    Serial.print(",");
//    Serial.print(distArray[2]); // Print A2 distance to Serial
//    Serial.print(",");
//    Serial.println(distArray[3]); // Print A3 distance to Serial  
//  
//    int a = map(distArray[2], 1500, 2100, 0, 108);
//    constrain(a, 0, 108);
//    int b = map(distArray[3], 1000, 1500, 0, 108);
//    constrain(b, 0, 108);
//    colorWipe(round10(a)-10, round10(b)-10, round10(a), round10(b), c_blue, BLINK_WAIT);
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
    delay(50);
}


void colorWipe(uint32_t from1, uint32_t from2, uint32_t to1, uint32_t to2, uint32_t c, uint8_t wait) {
  for(uint16_t i=0;i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c_lightblue); 
    strip2.setPixelColor(i, c_lightblue);
  }
  for(uint16_t i=from1; i<to1; i++) {
    strip.setPixelColor(i, c);
    strip.show();

    //delay(50);
  }
//    for(uint16_t i=from2; i<to2; i++) {
//    strip2.setPixelColor(i, c);
//    strip2.show();
//
//    //delay(50);
//  }
}

void readIntensity(int val, int val2)
{
  intensity = (uint8_t) (map(val, 1000, 5000, 0x00, 0x90));
  intensity2 = (uint8_t) (map(val, 1000, 5000, 0x00, 0x90));
}

void readNotes(int val, int val2)
{
  note = (uint8_t) (map(val, 1000, 4000, 48, 59));
  note2 = (uint8_t) (map(val2, 1000, 4000, 48, 59));
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}
//
//void controlChange(byte channel, byte control, byte value) {
//  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
//  MidiUSB.sendMIDI(event);
//}

void playNotes() {
     noteOn(0x90, note, intensity);
 //    noteOff(oldNote1[0], oldNote1[1], oldNote1[2]);
     oldNote1[0] = 4;
     oldNote1[1] = note;
     oldNote1[2] = intensity;
  //   delay(100);
//     noteOn(2, note2, intensity2);
//     oldNote2[0] = 2;
//     oldNote1[1] = note2;
//     oldNote1[2] = intensity2;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  Serial.write(channel);
  Serial.write(pitch);
  Serial.write(velocity);
//   MIDI.read();
}

void noteOff(byte channel, byte pitch, byte velocity) {
//  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
//  MidiUSB.sendMIDI(noteOff);
}
