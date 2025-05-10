#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <MadgwickAHRS.h>  // Include the Madgwick library

// Create objects for sensors
SparkFun_ISM330DHCX myISM;
SFE_MMC5983MA myMag;
Madgwick filter;  // Create an instance of the Madgwick filter

// Define raw data structs
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

// Magnetometer Calibration Parameters
double A_mag[3][3] = {
  {1.168384, 0.004008, -0.001287},
  {0.004008, 1.229647, -0.087849},
  {-0.001287, -0.087849, 1.265104}
};
double b_mag[3] = {-2.508501, 32.722110, -8.102990};

// Accelerometer Calibration Parameters
double A_acc[3][3] = {
  {0.996986, 0.000024, -0.000549},
  {0.000024, 1.001778, 0.000595},
  {-0.000549, 0.000595, 1.003680}
};
double b_acc[3] = {0.005380, -0.014230, 0.009798};


void setup() {
  // Start communication
  Wire.begin();
  Serial.begin(115200);

  // Initialize the accelerometer and magnetometer
  if (!myISM.begin()) {
    Serial.println("Failed to initialize ISM330DHCX.");
    while (1);
  }

  if (!myMag.begin()) {
    Serial.println("Failed to initialize MMC5983MA.");
    while (1);
  }

  // Reset devices
  myISM.deviceReset();
  while (!myISM.getDeviceReset()) {
    delay(1);
  }
  
  // Apply settings for accelerometer and gyroscope
  myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
  myISM.setAccelFullScale(ISM_2g);
  myISM.setAccelFilterLP2();
  myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
  myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
  myISM.setGyroFullScale(ISM_125dps);
  myISM.setGyroFilterLP1();
  myISM.setGyroLP1Bandwidth(ISM_MEDIUM);

  // Initialize Madgwick filter
  filter.begin(1);  // Set data rate for Madgwick filter

}

void loop() {
  // Get raw accelerometer data
  if (myISM.getRawAccel(&rawAccelData)) {
    // Apply accelerometer calibration              // lsb to g . 
    double rawX = (rawAccelData.xData * 2.0) / 32768.0;
    double rawY = (rawAccelData.yData * 2.0) / 32768.0;
    double rawZ = (rawAccelData.zData * 2.0) / 32768.0;

    double calibAccel[3];
    calibAccel[0] = A_acc[0][0] * (rawX - b_acc[0]) + A_acc[0][1] * (rawY - b_acc[1]) + A_acc[0][2] * (rawZ - b_acc[2]);
    calibAccel[1] = A_acc[1][0] * (rawX - b_acc[0]) + A_acc[1][1] * (rawY - b_acc[1]) + A_acc[1][2] * (rawZ - b_acc[2]);
    calibAccel[2] = A_acc[2][0] * (rawX - b_acc[0]) + A_acc[2][1] * (rawY - b_acc[1]) + A_acc[2][2] * (rawZ - b_acc[2]);

    // Get raw magnetometer data
    double magX = static_cast<double>(myMag.getMeasurementX());
    double magY = static_cast<double>(myMag.getMeasurementY());
    double magZ = static_cast<double>(myMag.getMeasurementZ());

    // Scale magnetometer data   // lsb to microTesla
    magX = (magX - 131072.0) * 8.0 / 131072.0 * 100.0;
    magY = (magY - 131072.0) * 8.0 / 131072.0 * 100.0;
    magZ = (magZ - 131072.0) * 8.0 / 131072.0 * 100.0;

    // Invert Z-axis
    magZ = -magZ;

    // Apply magnetometer calibration
    double calibMag[3];
    calibMag[0] = A_mag[0][0] * (magX - b_mag[0]) + A_mag[0][1] * (magY - b_mag[1]) + A_mag[0][2] * (magZ - b_mag[2]);
    calibMag[1] = A_mag[1][0] * (magX - b_mag[0]) + A_mag[1][1] * (magY - b_mag[1]) + A_mag[1][2] * (magZ - b_mag[2]);
    calibMag[2] = A_mag[2][0] * (magX - b_mag[0]) + A_mag[2][1] * (magY - b_mag[1]) + A_mag[2][2] * (magZ - b_mag[2]);

    // Get raw gyroscope data
    if (myISM.getRawGyro(&rawGyroData)) {
      // Scale the raw gyroscope data to degree per second
      double gyroX = (rawGyroData.xData * 125.0)/32768.0 ;
      double gyroY = (rawGyroData.yData * 125.0)/32768.0 ;
      double gyroZ = (rawGyroData.zData * 125.0)/32768.0 ;

      //calib gyro .....

      // Pass the sensor data to the Madgwick filter

       filter.update(gyroX, gyroY, gyroZ, calibAccel[0], calibAccel[1], calibAccel[2],calibMag[0], calibMag[1], calibMag[2]);

      // Print the RPY values
      Serial.print(filter.getRoll(),5);
      Serial.print(" ");
      Serial.print(filter.getPitch(),5);
      Serial.print(" ");
      Serial.println(filter.getYaw(),5);



    } else {
      Serial.println("Failed to read raw gyroscope data.");
    }
  } else {
    Serial.println("Failed to read raw accelerometer data.");
  }

  //delay(100); // Adjust delay for desired output rate
}






































#include <Wire.h>
#include "SparkFun_ISM330DHCX.h"
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <MadgwickAHRS.h>  // Include the Madgwick library

// Create objects for sensors
SparkFun_ISM330DHCX myISM;
SFE_MMC5983MA myMag;
Madgwick filter;  // Create an instance of the Madgwick filter

unsigned long microsPerReading, microsPrevious;

// Define raw data structs
sfe_ism_raw_data_t rawAccelData;
sfe_ism_raw_data_t rawGyroData;

// Magnetometer Calibration Parameters
double A_mag[3][3] = {
  {1.168384, 0.004008, -0.001287},
  {0.004008, 1.229647, -0.087849},
  {-0.001287, -0.087849, 1.265104}
};
double b_mag[3] = {-2.508501, 32.722110, -8.102990};

// Accelerometer Calibration Parameters
double A_acc[3][3] = {
  {0.996986, 0.000024, -0.000549},
  {0.000024, 1.001778, 0.000595},
  {-0.000549, 0.000595, 1.003680}
};
double b_acc[3] = {0.005380, -0.014230, 0.009798};



void setup() {
  // Start communication
  Wire.begin();
  Serial.begin(2000000);

  // Initialize the accelerometer and magnetometer
  if (!myISM.begin()) {
    Serial.println("Failed to initialize ISM330DHCX.");
    while (1);
  }

  if (!myMag.begin()) {
    Serial.println("Failed to initialize MMC5983MA.");
    while (1);
  }

  // Reset devices
  myISM.deviceReset();
  while (!myISM.getDeviceReset()) {
    delay(1);
  }

  // Apply settings for accelerometer and gyroscope
  myISM.setAccelDataRate(ISM_XL_ODR_12Hz5);
  myISM.setAccelFullScale(ISM_2g);
  //myISM.setAccelFilterLP2();
 // myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);
  myISM.setGyroDataRate(ISM_GY_ODR_12Hz);
  myISM.setGyroFullScale(ISM_125dps);
  //myISM.setGyroFilterLP1();
  //myISM.setGyroLP1Bandwidth(ISM_MEDIUM);

  // Initialize Madgwick filter
  //filter.begin(100);  // Set data rate for Madgwick filter

  // initialize variables to pace updates to correct rate
  //microsPerReading = 1000000 / 900000;
  microsPrevious = micros();
}

void normalize(double& x, double& y, double& z) {
  double magnitude = sqrt(x * x + y * y + z * z);
  if (magnitude > 0.001) {    // >0 , >0.001 , etc 
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
  }
}

void loop() {
  // Get current time in microseconds
 // unsigned long microsNow;

  // Check if it's time to read data and update the filter
   // microsNow = micros();
  //if (microsNow - microsPrevious >= microsPerReading) {


    // Get raw accelerometer data
    if (myISM.getRawAccel(&rawAccelData)) {
      // Convert raw accelerometer data to 'g'
      double rawX = (rawAccelData.xData * 2.0) / 32768.0;
      double rawY = (rawAccelData.yData * 2.0) / 32768.0;
      double rawZ = (rawAccelData.zData * 2.0) / 32768.0;

      // Apply accelerometer calibration
      double calibAccel[3];
      calibAccel[0] = A_acc[0][0] * (rawX - b_acc[0]) + A_acc[0][1] * (rawY - b_acc[1]) + A_acc[0][2] * (rawZ - b_acc[2]);
      calibAccel[1] = A_acc[1][0] * (rawX - b_acc[0]) + A_acc[1][1] * (rawY - b_acc[1]) + A_acc[1][2] * (rawZ - b_acc[2]);
      calibAccel[2] = A_acc[2][0] * (rawX - b_acc[0]) + A_acc[2][1] * (rawY - b_acc[1]) + A_acc[2][2] * (rawZ - b_acc[2]);

      // Get raw magnetometer data
      double magX = static_cast<double>(myMag.getMeasurementX());
      double magY = static_cast<double>(myMag.getMeasurementY());
      double magZ = static_cast<double>(myMag.getMeasurementZ());

      // Scale magnetometer data
      magX = (magX - 131072.0) * 8.0 / 131072.0 * 100.0;
      magY = (magY - 131072.0) * 8.0 / 131072.0 * 100.0;
      magZ = (magZ - 131072.0) * 8.0 / 131072.0 * 100.0;

      // Invert Z-axis
      magZ = -magZ;

      // Apply magnetometer calibration
      double calibMag[3];
      calibMag[0] = A_mag[0][0] * (magX - b_mag[0]) + A_mag[0][1] * (magY - b_mag[1]) + A_mag[0][2] * (magZ - b_mag[2]);
      calibMag[1] = A_mag[1][0] * (magX - b_mag[0]) + A_mag[1][1] * (magY - b_mag[1]) + A_mag[1][2] * (magZ - b_mag[2]);
      calibMag[2] = A_mag[2][0] * (magX - b_mag[0]) + A_mag[2][1] * (magY - b_mag[1]) + A_mag[2][2] * (magZ - b_mag[2]);

      // Get raw gyroscope data
      if (myISM.getRawGyro(&rawGyroData)) {
        // Scale raw gyroscope data to degrees per second
        double gyroX = (rawGyroData.xData * 125.0) / 32768.0;
        double gyroY = (rawGyroData.yData * 125.0) / 32768.0;
        double gyroZ = (rawGyroData.zData * 125.0) / 32768.0;

        // Pass the sensor data to the Madgwick filter
        filter.update(gyroX, gyroY, gyroZ, calibAccel[0], calibAccel[1], calibAccel[2], calibMag[0], calibMag[1], calibMag[2]);

        // Print the RPY values
        Serial.print("Roll: ");
        Serial.print(filter.getRoll(), 5);
        Serial.print(" Pitch: ");
        Serial.print(filter.getPitch(), 5);
        Serial.print(" Yaw: ");
        Serial.print(filter.getYaw(), 5);
        Serial.println(" ");
      } else {
        Serial.println("Failed to read raw gyroscope data.");
      }
    } else {
      Serial.println("Failed to read raw accelerometer data.");
    }

      // microsPrevious = microsPrevious + microsPerReading;
  }

