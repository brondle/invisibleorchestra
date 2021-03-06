/*
SharpDistSensorArray.ino

Source: https://github.com/DrGFreeman/SharpDistSensor

*/

#include <SharpDistSensor.h>

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

void setup() {
  // Set some parameters for each sensor in array
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A710K0F_5V_DS);  // Set sensor model
  }
  Serial.begin(9600);
  Serial.setTimeout(50); /* Reduce latency */ 
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
    
    // Wait some time
    delay(50);
}
