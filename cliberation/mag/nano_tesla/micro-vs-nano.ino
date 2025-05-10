#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>
#include <math.h>

SFE_MMC5983MA myMag;

// Magneto Calibration Parameters
double microA[3][3] = {
  {1.168384, 0.004008, -0.001287},
  {0.004008, 1.229647, -0.087849},
  {-0.001287, -0.087849, 1.265104}};
double microb[3] = {-2.508501, 32.722110, -8.102990};

double nanoA[3][3] = {
  {1.168410, 0.004008, -0.001287},
  {0.004008, 1.229675, -0.087851},
  {-0.001287, -0.087851, 1.265133}};
double nanob[3] = {-2508.501393, 32722.109719, -8102.989934};

double picoA[3][3] = {
  {1.164961, 0.002885, -0.000955},
  {0.002885, 1.241208, -0.092292},
  {-0.000955, -0.092292, 1.263595}};
double picob[3] = {-2524661.313118, 32898901.751168, -8157212.264480};

// Scaling Factors
const double SCALE_FACTOR_micro = 8.0 / 131072.0 * 100.0;
const double SCALE_FACTOR_nano = 8.0 / 131072.0 * 100000.0;
const double SCALE_FACTOR_pico = 8.0 / 131072.0 * 100000000.0;

// Previous yaw values for calculating change
double lastMicroYaw = 0.0;
double lastNanoYaw = 0.0;
double lastPicoYaw = 0.0;

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
  double magmicroX, magmicroY, magmicroZ;
  double magnanoX, magnanoY, magnanoZ;
  double magpicoX, magpicoY, magpicoZ;

  magpicoX = magnanoX = magmicroX = static_cast<double>(myMag.getMeasurementX());
  magpicoY = magnanoY = magmicroY = static_cast<double>(myMag.getMeasurementY());
  magpicoZ = magnanoZ = magmicroZ = static_cast<double>(myMag.getMeasurementZ());

  // Scale and calibrate magnetometer values
  // Micro
  magmicroX = (magmicroX - 131072.0) * SCALE_FACTOR_micro;
  magmicroY = (magmicroY - 131072.0) * SCALE_FACTOR_micro;
  magmicroZ = -(magmicroZ - 131072.0) * SCALE_FACTOR_micro;  // Invert Z-axis

  double calibmicroX = (microA[0][0] * (magmicroX - microb[0])) + (microA[0][1] * (magmicroY - microb[1])) + (microA[0][2] * (magmicroZ - microb[2]));
  double calibmicroY = (microA[1][0] * (magmicroX - microb[0])) + (microA[1][1] * (magmicroY - microb[1])) + (microA[1][2] * (magmicroZ - microb[2]));

  double microyaw = atan2(calibmicroY, calibmicroX) * 180.0 / PI;
  if (microyaw < 0.0) microyaw += 360.0;  // Normalize to 0-360 degrees

  // Nano
  magnanoX = (magnanoX - 131072.0) * SCALE_FACTOR_nano;
  magnanoY = (magnanoY - 131072.0) * SCALE_FACTOR_nano;
  magnanoZ = -(magnanoZ - 131072.0) * SCALE_FACTOR_nano;

  double calibnanoX = (nanoA[0][0] * (magnanoX - nanob[0])) + (nanoA[0][1] * (magnanoY - nanob[1])) + (nanoA[0][2] * (magnanoZ - nanob[2]));
  double calibnanoY = (nanoA[1][0] * (magnanoX - nanob[0])) + (nanoA[1][1] * (magnanoY - nanob[1])) + (nanoA[1][2] * (magnanoZ - nanob[2]));

  double nanoyaw = atan2(calibnanoY, calibnanoX) * 180.0 / PI;
  if (nanoyaw < 0.0) nanoyaw += 360.0;  // Normalize to 0-360 degrees

  // Pico
  magpicoX = (magpicoX - 131072.0) * SCALE_FACTOR_pico;
  magpicoY = (magpicoY - 131072.0) * SCALE_FACTOR_pico;
  magpicoZ = -(magpicoZ - 131072.0) * SCALE_FACTOR_pico;

  double calibpicoX = (picoA[0][0] * (magpicoX - picob[0])) + (picoA[0][1] * (magpicoY - picob[1])) + (picoA[0][2] * (magpicoZ - picob[2]));
  double calibpicoY = (picoA[1][0] * (magpicoX - picob[0])) + (picoA[1][1] * (magpicoY - picob[1])) + (picoA[1][2] * (magpicoZ - picob[2]));

  double picoyaw = atan2(calibpicoY, calibpicoX) * 180.0 / PI;
  if (picoyaw < 0.0) picoyaw += 360.0;  // Normalize to 0-360 degrees

  // Calculate yaw change
  double changeMicroYaw = microyaw - lastMicroYaw;
  double changeNanoYaw = nanoyaw - lastNanoYaw;
  double changePicoYaw = picoyaw - lastPicoYaw;

  // Update last yaw values
  lastMicroYaw = microyaw;
  lastNanoYaw = nanoyaw;
  lastPicoYaw = picoyaw;

  // Print yaw and changes
  Serial.print("         ");
  //Serial.print(microyaw, 3);
  //Serial.print(" Change: ");
  Serial.print(changeMicroYaw, 3);
  //Serial.print(" | NanoYaw: ");
  //Serial.print(nanoyaw, 3);
  Serial.print("          ");
  Serial.print(changeNanoYaw, 3);
  //Serial.print(" | PicoYaw: ");
  //Serial.print(picoyaw, 3);
  Serial.print("          ");
  Serial.print(changePicoYaw, 3);


  String smallestDifferenceName = "";
double smallestDifference = abs(changeMicroYaw);  // Start with microyaw difference
smallestDifferenceName = "Micro";

if (abs(changeNanoYaw) < smallestDifference) {
  smallestDifference = abs(changeNanoYaw);
  smallestDifferenceName = "Nano";
}

if (abs(changePicoYaw) < smallestDifference) {
  smallestDifference = abs(changePicoYaw);
  smallestDifferenceName = "Pico";
}

// Print the name of the smallest difference
Serial.print("Smallest change is in: ");
Serial.println(smallestDifferenceName);

  delay(100);  // Update every 100ms
}
