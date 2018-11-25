#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUM_LEDS 60
#define BRIGHTNESS 50
#define BLINK_WAIT 10
int currentWandPos = 0;

uint32_t c_yellow, c_red, c_blue, c_green, c_white, c_magenta;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


void setup() {
   Serial.begin(9600);
   delay(1000);
   c_red     = strip.Color(255,0,0);
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop() {
  // most code unabashedly stolen from dan shiffman/danny rozin/tom igoe. big thanks.
  // put your main code here, to run repeatedly:
    if (Serial.available()) {
    String fromSerial = Serial.readStringUntil('\n');               // read until you see a \n 
    int firstValueEnd = fromSerial.indexOf(',');                    // find the first comma and tell me how deep into the string it is
    String firstValueString = fromSerial.substring(0,firstValueEnd);// give me a new string that includes everything till the first comma
    int firstValue= firstValueString.toInt();                       // give me the int interpretation of that string 
    int secondValueEnd = fromSerial.indexOf(',',firstValueEnd+1);                     // search for the second comma, start searching after the first one
    String secondValueString = fromSerial.substring(firstValueEnd+1,secondValueEnd);   //give me a new string with everything beween first and second comma
    int secondValue= secondValueString.toInt();     // give me the int interpretation of that string 
  
    //tried out mapping
    int remapValue =  map(secondValue, 0, 200, 0, 100);
    remapValue = constrain(remapValue, 0, 100);
    int mappedValue = 100 - remapValue; //numbers are reversed
    Serial.println(mappedValue);
    if (round10(mappedValue) != round10(currentWandPos)) {
        Serial.flush();
       currentWandPos = mappedValue;
       strip.clear();
       colorWipe(round15(mappedValue)-10, round15(mappedValue), c_red, BLINK_WAIT);
    }
  }
  delay(50);
}
void colorWipe(uint32_t from, uint32_t to, uint32_t c, uint8_t wait) {
  for(uint16_t i=from; i<to; i++) {
   
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

int round15(int n) {
  return (n/15 + (n%15>2)) * 10;
}
int round10(int n) {
  return (n/10 + (n%10>2)) * 10;
}
