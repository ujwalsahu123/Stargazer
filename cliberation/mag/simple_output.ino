// output mag xyz reading in lsb . 

#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

// Create magnetometer object
SFE_MMC5983MA myMag;

void setup() {
  Serial.begin(115200);  // Set baud rate

  // Initialize MMC5983MA magnetometer
  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
    while(1);
  }

  Serial.println("Magnetometer Initialized.");
  delay(500);
}

void loop() {
  
  // Output raw data as integers
  Serial.print(myMag.getMeasurementX());  Serial.print(",");
  Serial.print(myMag.getMeasurementY());  Serial.print(",");
  Serial.println(myMag.getMeasurementZ());

  delay(100);  // 10 Hz
}
