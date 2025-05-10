#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include "SparkFun_ISM330DHCX.h"

// Create objects for the magnetometer and IMU
SFE_MMC5983MA myMag;
SparkFun_ISM330DHCX myISM;

// Define the structure for raw accelerometer and gyroscope data
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize the magnetometer
  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
    while (1);
  }
  
  // Initialize the ISM330DHCX (accelerometer and gyroscope)
  Wire.begin();
  if (!myISM.begin()) {
    Serial.println("Could not initialize ISM330DHCX. Check connections.");
    while (1);
  }

  // Reset the device to default settings
  myISM.deviceReset();
  while (!myISM.getDeviceReset()) {
    delay(1);
  }

  // Apply accelerometer and gyroscope settings
  myISM.setDeviceConfig();
  myISM.setBlockDataUpdate(); // Prevent overwriting of unread data

  // Set accelerometer configuration
  myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // 104Hz data rate
  myISM.setAccelFullScale(ISM_2g);          // ±2g range
  myISM.setAccelFilterLP2();                // Enable low-pass filter
  myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); // Slope filter setting

  // Set gyroscope configuration
  myISM.setGyroDataRate(ISM_GY_ODR_104Hz);  // 104Hz data rate
  myISM.setGyroFullScale(ISM_125dps);        // ±125dps range
  myISM.setGyroFilterLP1();                  // Enable low-pass filter for gyroscope
  myISM.setGyroLP1Bandwidth(ISM_MEDIUM);     // Medium bandwidth

}

void loop() {
  // Read raw magnetometer data

  // Read raw accelerometer data
  if (myISM.getRawAccel(&rawAccelData)) {
    // Read raw gyroscope data
    if (myISM.getRawGyro(&rawGyroData)) {
      // Print all raw sensor values in the requested format
      Serial.print(myMag.getMeasurementX()); Serial.print(",");
      Serial.print(myMag.getMeasurementY()); Serial.print(",");
      Serial.print(myMag.getMeasurementZ()); Serial.print(",");

      Serial.print(rawAccelData.xData); Serial.print(",");
      Serial.print(rawAccelData.yData); Serial.print(",");
      Serial.print(rawAccelData.zData); Serial.print(",");

      Serial.print(rawGyroData.xData); Serial.print(",");
      Serial.print(rawGyroData.yData); Serial.print(",");
      Serial.println(rawGyroData.zData);
    } else {
      Serial.println("Failed to read raw gyroscope data.");
    }
  } else {
    Serial.println("Failed to read raw accelerometer data.");
  }

  //delay(50);  // Adjust delay as needed for desired data output rate
}
