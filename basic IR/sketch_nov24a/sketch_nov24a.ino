/*
Source: https://github.com/DrGFreeman/SharpDistSensor
*/

/*
This example shows how to use the SharpDistSensor library to continuously
read the sensor and display the distance in mm using a pre-defined sensor model.
See the library README page for the list of available models and corresponding
constants or for how to use custom defined fit functions.
*/

#include <SharpDistSensor.h>

// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 20;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, medianFilterWindowSize);

void setup() {
  Serial.begin(9600);

  // Set sensor model
  sensor.setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);
}

void loop() {
  // Get distance from sensor
  unsigned int distance = sensor.getDist();

  // Print distance to Serial
  Serial.println(distance);

  // Wait some time
  delay(10);
}
