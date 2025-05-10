// in the blocking code , the sesnor will run contineously and will give data , while the motors are running .

// just set rpm value and target value ( mostly motor2Target change hoga . and motor2Target 360 hee rahayga , so full 1 roation of pitch)

// motor 1 is pitch rotaion , motor 2 is yaw roation . 


//no dealy() stuff .   it slows the motor roataion down . and vaise hee bohot slow hai.


// done . 

// rmp dosent matter , all you can do here is motor2taget 
// and also it takes values super fast (12000 per rotation) so what you can do is 
// do dealy() to slow the sesnor reading . it will not effect motor . just the sesnor
// data capturing will be slowed . 

#include <CheapStepper.h>
#include <Wire.h>
#include <SparkFun_MMC5983MA_Arduino_Library.h>

SFE_MMC5983MA myMag;

CheapStepper motor1(2, 3, 4, 5);
CheapStepper motor2(6, 7, 8, 9);


int motor1Target = 380;  
int motor2Target = 20;   

int counter=0;
int TimesRotate = 380/motor2Target ;   // so 360/10 = 36 times motor2 will move to complete 1 rotaion . and after 1 rotation the if will go infinity loop.

bool motor1Moving = true;
bool motor2Moving = false;

bool rotation = true ;


void setup() {
  Serial.begin(115200);  
  
  if (!myMag.begin()) {
    Serial.println("Error initializing MMC5983MA sensor. Check connections.");
    while (1);  
  }
  //Serial.println("Magnetometer Initialized.");

delay(3000);

  motor1.setRpm(40);
  motor2.setRpm(12);

 
  motor1.newMoveDegrees(rotation, motor1Target);  // Motor 1 moves clockwise
}

void loop() {
  
  motor1.run();
  motor2.run();

 
  Serial.print(myMag.getMeasurementX());  Serial.print(",");
  Serial.print(myMag.getMeasurementY());  Serial.print(",");
  Serial.println(myMag.getMeasurementZ());
  

  if (motor1Moving && motor1.getStepsLeft() == 0) {
    motor1Moving = false; 
    motor2.newMoveDegrees(true, motor2Target);  
    motor2Moving = true;  
  }



  if (motor2Moving && motor2.getStepsLeft() == 0) {

    counter++;

    if (counter == TimesRotate ) {
    stopMotors();  // Stop both motors
    infiniteLoop();  // Enter infinite loop with 0 values
    }
    
    motor2Moving = false; // Motor 2 has finished its move
      rotation = !(rotation) ;
    motor1.newMoveDegrees(rotation, motor1Target);  // Motor 1 moves clockwise
    motor1Moving = true;  // Mark Motor 1 as moving
  }

}


void stopMotors() {
  motor1.stop();  // Stop motor1
  motor2.stop();  // Stop motor2
  //Serial.println("Motors stopped.");
}


void infiniteLoop() {
  while (true) {
  Serial.print(0);  Serial.print(",");
  Serial.print(0);  Serial.print(",");
  Serial.println(0);
    delay(1000);  // Print every second to show that the motors have stopped
  }
}