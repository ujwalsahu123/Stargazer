// in this blocing code . the motor will mover to a certain point then 
// sensor data will be output , and after that again it will move and then sensor data will print .
// sesnor data dosent output contieously . it only output when the motor is at rest .

//// motor 1 is pitch rotaion , motor 2 is yaw roation . 
// and you can inverse the motor connection but wire seen 

// // just set rpm value and target value.

// so you can play with -> rpm value (speed slow) // target value (this will read that number or readings) // dealy .



// i think more data is imp . fast chalayga . since mag can capture data in fast momentem.
//  so target 1 , rpm 5 ,  no delay , motor2target  5 or 10 


// code is done . 

#include <CheapStepper.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

// Create magnetometer object
SFE_MMC5983MA myMag;

// Define two motors with their pins connected to ULN2003 driver boards
CheapStepper motor1(2, 3, 4, 5);
CheapStepper motor2(6, 7, 8, 9);


// Target step positions for both motors
int motor1Target = 1;  
int TimesLoop = 380/motor1Target ;


int motor2Target = 15;   
int counter=0;
int TimesRotate = 380/motor2Target ;

bool rotation = true ;

void setup() {
  Serial.begin(115200);  // Set baud rate for serial communication
  
  // Initialize MMC5983MA magnetometer
  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
    while (1);  // Stay in a loop if sensor fails to initialize
  }
  //Serial.println("Magnetometer Initialized.");
  delay(3000);
  // Set RPM for both motors
  motor1.setRpm(1);
  motor2.setRpm(1);

}



void loop() {
  // Move motor1 one step at a time, print sensor data after each step
  for (int i = 0; i < TimesLoop; i++) {

  Serial.print(myMag.getMeasurementX());  Serial.print(",");
  Serial.print(myMag.getMeasurementY());  Serial.print(",");
  Serial.println(myMag.getMeasurementZ());
    
    motor1.moveDegrees(rotation,motor1Target);  
    //delay(10);    // put dealy for more slow values . 
  }

    rotation = !(rotation) ;


  // After motor1 completes its movement, move motor2
  
if (counter == TimesRotate-1 ) {
    infiniteLoop();  // Enter infinite loop with 0 values
    }


    motor2.moveDegrees(true,motor2Target);  
    counter++;
    

  }

 

void infiniteLoop() {
  while (true) {
  Serial.print(0);  Serial.print(",");
  Serial.print(0);  Serial.print(",");
  Serial.println(0);
    delay(1000);  // Print every second to show that the motors have stopped
  }
}