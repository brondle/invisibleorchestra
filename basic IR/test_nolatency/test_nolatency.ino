#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <SharpDistSensor.h>


#define PIN 6
#define PIN2 5
#define NUM_LEDS 50
#define BRIGHTNESS 50
#define BLINK_WAIT 10
int currentWandPos = 0;

uint32_t c_yellow, c_red, c_blue, c_green, c_white, c_magenta, c_lightblue;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);


// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 19;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, medianFilterWindowSize);
const int threshold = 20;
int oldValue = 0;
int oldTime = 0;

void setup() {
  // put your setup code here, to run once:
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
  sensor.setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);

}

void loop() {
  // put your main code here, to run repeatedly:
          // Get distance from sensor
   // strip.setPixelColor(1, c_red); // Moderately bright green color.

    strip.show(); // 
  // Print distance to Serial
  //compare to threshold to prevent too many values being written
   unsigned int distance = sensor.getDist();
   distance = 1000;
//   if (distance < 2500 && (oldValue >= distance+threshold || oldValue <= distance-threshold /*|| millis() - oldTime >= 750*/)) {
//    Serial.println(distance);
//    oldValue = distance;
//    oldTime = millis();
    int a = map(distance, 1100, 2500, 0, 100);
    constrain(a, 0, 50);
    // strip.clear();
    colorWipe(round15(a)-10, round15(a), c_blue, BLINK_WAIT); 
    // chase(c_red, a);
    delay(50);
//   }
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
    delay(wait);
  }
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}

    static void chase(uint32_t c, uint16_t i) {
      for(i; i<strip.numPixels()+4; i++) {
          strip.setPixelColor(i, c); // Draw new pixel
          strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
          strip.show();
      }
    }
