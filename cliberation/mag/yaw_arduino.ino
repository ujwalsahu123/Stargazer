#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <math.h>

SFE_MMC5983MA myMag;

/////////////////////////////////  update the offset data , after each calib
////////////////////////////////    and note that setup should not change (headphone , pen , pencil , table..) . 

// Magneto Calibration Parameters
double A[3][3] = {
  {1.168384, 0.004008, -0.001287},
  {0.004008, 1.229647, -0.087849},
  {-0.001287, -0.087849, 1.265104}};
double b[3] = {-2.508501, 32.722110, -8.102990};

// Scaling Factor (LSB to microtesla for this sensor)
const double SCALE_FACTOR = 8.0 / 131072.0 * 100.0;  // From LSB to microtesla

// Averaging Parameters
const int numSamples = 50;  // Number of samples for moving average
double yawSum = 0.0;        // Sum of yaw values
int sampleCount = 0;        // Current sample count
double lastYaw = 0.0;       // Last yaw value for low-pass filtering

void setup() {
  Serial.begin(115200);

  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
    while (1);
  }
  Serial.println("Magnetometer Initialized.");
  delay(500);
}

void loop() {
  // Read raw magnetometer values (LSB)
  double magX = static_cast<double>(myMag.getMeasurementX());
  double magY = static_cast<double>(myMag.getMeasurementY());
  double magZ = static_cast<double>(myMag.getMeasurementZ());

  // Scale to microtesla
  magX = (magX - 131072.0) * SCALE_FACTOR;
  magY = (magY - 131072.0) * SCALE_FACTOR;
  magZ = (magZ - 131072.0) * SCALE_FACTOR;

  // Invert Z-axis
  magZ = -magZ;

  // Apply calibration
  double calibX = (A[0][0] * (magX - b[0])) + (A[0][1] * (magY - b[1])) + (A[0][2] * (magZ - b[2]));
  double calibY = (A[1][0] * (magX - b[0])) + (A[1][1] * (magY - b[1])) + (A[1][2] * (magZ - b[2]));
  double calibZ = (A[2][0] * (magX - b[0])) + (A[2][1] * (magY - b[1])) + (A[2][2] * (magZ - b[2]));

  // Calculate yaw
  double yaw = atan2(calibY, calibX) * 180.0 / PI;
  if (yaw < 0.0) yaw += 360.0;  // Normalize to 0-360 degrees

  // Update moving average
  if (sampleCount < numSamples) {
    yawSum += yaw;
    sampleCount++;
  } else {
    yawSum -= yawSum / numSamples;  // Remove the oldest sample
    yawSum += yaw;                  // Add the newest sample
  }

  // Calculate the averaged yaw
  double averagedYaw = yawSum / sampleCount;

  // Low-pass filter for smoother yaw
  double filteredYaw = lastYaw * 0.8 + yaw * 0.2;  // Filter strength: 0.8 for previous, 0.2 for current
  lastYaw = filteredYaw;

  // Output the averaged and filtered yaw
  Serial.print("Filtered Yaw: ");
  Serial.print(filteredYaw, 3);  // Print filtered yaw with 3 decimal places
  Serial.print(" | Averaged Yaw: ");
  Serial.println(averagedYaw, 3);  // Print averaged yaw with 3 decimal places

  delay(10);  // Faster update rate (100 Hz)
}
