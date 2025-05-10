#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"

SparkFun_ISM330DHCX myISM;

// Define the structure for raw accelerometer data
sfe_ism_raw_data_t rawAccelData;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  if (!myISM.begin()) {
    Serial.println("Could not initialize ISM330DHCX. Check connections.");
    while (1);
  }

  // Reset the device to default settings
  myISM.deviceReset();
  while (!myISM.getDeviceReset()) {
    delay(1);
  }
  Serial.println("Reset complete.");

  // Apply device settings
  myISM.setDeviceConfig();
  myISM.setBlockDataUpdate(); // Prevent overwriting of unread data

  // Set accelerometer configuration
  myISM.setAccelDataRate(ISM_XL_ODR_104Hz); // 104Hz data rate
  myISM.setAccelFullScale(ISM_2g);          // ±2g range
  myISM.setAccelFilterLP2();                // Enable low-pass filter
  myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100); // Slope filter setting

  Serial.println("Settings applied.");
}

void loop() {
  // Get raw accelerometer data
  if (myISM.getRawAccel(&rawAccelData)) {
    // Print raw data for X, Y, Z axes separated by commas
    Serial.print(rawAccelData.xData);
    Serial.print(",");  // Separate values with a comma
    Serial.print(rawAccelData.yData);
    Serial.print(",");  // Separate values with a comma
    Serial.println(rawAccelData.zData);  // Print Z value and move to next line
  } else {
    Serial.println("Failed to read raw accelerometer data.");
  }

  delay(100); // Delay to control data output rate (adjust as necessary)
}
