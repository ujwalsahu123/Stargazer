#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
SparkFun_ISM330DHCX myISM;
// Structs for X,Y,Z data
sfe_ism_raw_data_t rawGyroData; 

void setup(){
 Wire.begin();
 Serial.begin(115200);
 if( !myISM.begin() ){
 Serial.println("Did not begin.");
 while(1); }
 // Reset the device to default settings. This if helpful is you're doing multiple
 // uploads testing different settings.
 myISM.deviceReset();
 // Wait for it to finish reseting
 while( !myISM.getDeviceReset() ){
 delay(1);
 }
 Serial.println("Reset.");
 Serial.println("Applying settings.");
 delay(100);
 myISM.setDeviceConfig();
 myISM.setBlockDataUpdate();

 // Set the output data rate and precision of the gyroscope
 myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
 myISM.setGyroFullScale(ISM_125dps);        
 
 // Turn on the gyroscope's filter and apply settings.
 myISM.setGyroFilterLP1();
 myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
}
void loop(){
 // Check if both gyroscope and accelerometer data is available
  if(myISM.getRawGyro(&rawGyroData))
  {

    Serial.print(rawGyroData.xData);
    Serial.print(",");  // Separate values with a comma
    Serial.print(rawGyroData.yData);
    Serial.print(",");  // Separate values with a comma
    Serial.println(rawGyroData.zData);  // Print Z value and move to next line
  } else {
    Serial.println("Failed to read raw accelerometer data.");
  }

 delay(100);
}

