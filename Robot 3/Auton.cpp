#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS


// Robot configuration code.a
inertial BrainInertial = inertial();
motor flywheelMotorA = motor(PORT1, true);
motor flywheelMotorB = motor(PORT8, false);
motor_group flywheel = motor_group(flywheelMotorA, flywheelMotorB);

controller Controller = controller();
distance frontDist = distance(PORT5);
distance rightDist = distance(PORT4);
motor dtL = motor(PORT6, false);
motor dtR = motor(PORT12, true);
motor leftShare = motor(PORT3, false);
motor rightShare = motor(PORT7, true);
touchled indicator = touchled(PORT2);
pneumatic motorShare = pneumatic(PORT10);
pneumatic pushPneumatic = pneumatic(PORT9);
gyro Gyro11 = gyro(PORT11);


// generating and setting random seed
void initializeRandomSeed(){
  wait(100,msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  // Combine these values into a single integer
  int seed = int(
    xAxis + yAxis + zAxis
  );
  // Set the seed
  srand(seed); 
}



void vexcodeInit() {

  // Initializing random seed.
  initializeRandomSeed(); 
}


// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration


/*

additions required:

line 412 for the Motor Encoder as backup if Brain Inertial Fails

*/


//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       Jett                                                      
//    Created:      2024-10-12                                                    
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------


// Include the IQ Library
#include "iq_cpp.h"


// Macros for the timeout
#define disable 0


// Allows for easier use of the VEX Library
using namespace vex;


//enumeration for number of pistons on intake whilst flywheel is on
enum pistonMode {two, one, zero};

//enumeration for the direction of the intake 
enum dir {forw, back};

//declaration of functions: definitions after int main()
void pidTurn(int target, double kp, double ki, double kd, double timeout);
void pidDrive(int target, double kp, double ki, double timeout);
void pidFlywheel(int target, double kp, double ki, double kd);
void spinMS(int motorNumber, dir direction, int speed, pistonMode mode); //both = 1, flywheel = 2, intrake = 3, disable/0 = off
void pidDriveC(int target, double kp, double ki, double kr, double angel,double timeout);
void pidDriveCDEC(int target, double kp, double ki, double kr, double angel,double timeout);
void pidDriveCNR(int target, double kp, double ki, double kr, double angel,double timeout);

//declaration of functions without parameters
void init();
void callback();

//constants for the PID system
const double kpTurn = 0.5, kiTurn = 0, kdTurn = 0.2;
const double kpTurnS = 0.5, kiTurnS = 0, kdTurnS = 0.6;
const double kpTurnF = 0.6; // fast turning speed
const double slow_kpDrive = 0.25, slow_kiDrive = 0.5;
const double kpDrive = 0.55, kiDrive = 0.2;
const double kpFlywheel = 0.5, kiFlywheel = 0.1, kdFlywheel = 0.2;
const double pi = 3.14159265358979323846264338502884;
int direction;


int tDir;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();


  //intalize the code using void init(), it sets the velocity, torque, and the pneumatic settings
  init();

  leftShare.setVelocity(100, percent);
  rightShare.setVelocity(100, percent);
  indicator.setColor(blue_green);
  while (!(indicator.pressing())) {}
  

  //BrainInertial.calibrate();
  //wait(5, seconds);
  BrainInertial.setHeading(0,degrees);
  BrainInertial.setRotation(0,degrees);

  indicator.setColor(yellow);

  //while (true) {
  //  printf("%f\n", BrainInertial.rotation(degrees));
  //}

  while ((indicator.pressing())) {}
  //wait until pressing touchLED
  while (!(indicator.pressing())) {}

  indicator.setColor(green);

  //collect ball
  pidTurn(-90, kpTurn, kiTurn, kdTurn, 2);

  //reset timer so the intake will spin for exactily 1500 msec
  Brain.Timer.reset();
  int i = 0;
  while (Brain.Timer.value() < 1.5) {
    i++;
    if (leftShare.current(amp) > 0.8) {
      spinMS(1, forw, 95, two);
    }
    else {
      spinMS(1, forw, 95, two);
    }
    printf("%f\n", leftShare.current(amp));
    if (i == 1) {
      pidDriveCDEC(-400,kpDrive, kiDrive, 5, direction, disable);
    }
  }

  //turn 180 degrees to face the goal zone
  pidTurn(180, kpTurnF, kiTurn, kdTurnS, 2);

  //let the robot recover after the turning
  wait(500, msec);
  
  //set intake to one-motor intake and set velocity to 105 for acceleration purposes
  spinMS(1, forw, 100, one);

  //drive towards the goal & align goal
  pidDriveCDEC(10000, kpDrive, kiDrive,  5, direction, 2.5);

  dtL.spin(forward, 100, percent);
  wait(1, seconds);
  dtL.stop();
  dtR.spin(forward, 100, percent);
  wait(1, seconds);
  dtR.stop();


  //shoot ball
  pushPneumatic.extend(cylinder2);
  wait(2, seconds);
  pushPneumatic.retract(cylinder2);

  //decrease flywheel speed for low goal
  spinMS(1, forw, 70, one);

  //lower the leveler to aim for the low goal
  pushPneumatic.extend(cylinder1);

  //2 seconds of two-motor intake then 2 seconds of one-motor intake for the ball to go into the indexer
  spinMS(1, forw, 80, two);
  wait(1,seconds);

  //shoot ball
  pushPneumatic.extend(cylinder2);
  wait(2,seconds);
  pushPneumatic.retract(cylinder2);

  //set velocity to 120 rpm (max) for acceleration purposes
  spinMS(1, forw, 100, two);
  pidDriveCDEC(-200,kpDrive,kiDrive,5,direction,disable);
  pidTurn(-45, kpTurn, kiTurn, kdTurn, disable);

  Brain.Timer.reset();

  pidDriveC(-750,kpDrive, kiDrive,  6, direction,3);

  spinMS(1, forw, 100, two);
  pidDriveCDEC(330, kpDrive, kiDrive,  5, direction,1.54);
  pidTurn(45, kpTurn, kiTurn, kdTurn, 1.7);
  pushPneumatic.retract(cylinder1);
  pidDriveCDEC(10000, kpDrive, kiDrive,  5, direction,4);
  wait(500,msec);


  //shoot ball

  pushPneumatic.extend(cylinder2);
  wait(2, seconds);
  pushPneumatic.retract(cylinder2);

  //decrease flywheel speed for low goal
  spinMS(1, forw, 80, one);

  //lower the leveler to aim for the low goal
  pushPneumatic.extend(cylinder1);

  //2 seconds of two-motor intake then 2 seconds of one-motor intake for the ball to go into the indexer
  spinMS(1, forw, 80, two);
  wait(2,seconds);


  //shoot ball
  pushPneumatic.extend(cylinder2);
  wait(2,seconds);
  pushPneumatic.retract(cylinder2);


  //set velocity to 120 rpm (max) for acceleration purposes
  spinMS(1, forw, 120, two);

  
  spinMS(1, forw, 70, two);
  pidDriveCDEC(-600, kpDrive, kiDrive, 5,  direction,disable);

  wait(1, seconds);
  pushPneumatic.extend(cylinder1);

  pidDriveCNR(-200, kpDrive, kiDrive,  15, direction,disable);
  wait(1, seconds);
  pidDriveCNR(10000, kpDrive, kiDrive,  15, direction,2.5);

  pushPneumatic.extend(cylinder2);
  wait(2,seconds);
  pushPneumatic.retract(cylinder2);

  while (true) {
    spinMS(1, forw, 70, two);
    pidDriveCNR(-500, kpDrive, kiDrive, 10, direction, disable);

    wait(1, seconds);
    pidDriveCNR(10000, kpDrive, kiDrive, 10, direction, 2.5);

    if (BrainInertial.rotation(degrees) > 0) {
      dtR.spin(forward);
    } else {
      dtL.spin(forward);
    }
    wait(1, seconds);
    dtL.stop();
    dtR.stop();
    pushPneumatic.extend(cylinder2);
    wait(2,seconds);
    pushPneumatic.retract(cylinder2);
  }

  //eomc
  return 0;
}


void callback() {
  const double kpFlywheel = 0.5, kiFlywheel = 0.1, kdFlywheel = 0.2;
  pidFlywheel(80, kpFlywheel, kiFlywheel, kdFlywheel);
}

void spinMS(int motorNumber, dir direction, int speed, pistonMode mode) {
  if (motorNumber == 1) {
    flywheel.spin(forward, speed, rpm);
    if (mode == one) {
      motorShare.retract(cylinder1);
      motorShare.retract(cylinder2);
      if (direction == forw) {

        rightShare.spin(forward, speed, rpm);
        leftShare.spin(reverse, speed, rpm);
      }

      else if (direction == back) {

        rightShare.spin(forward, speed, rpm);
        leftShare.spin(forward, speed, rpm);
      }
    }

    else if (mode == two) {
      motorShare.retract(cylinder1);
      motorShare.extend(cylinder2);

      if (direction == forw) {

        rightShare.spin(reverse, speed, rpm);
        leftShare.spin(reverse, speed, rpm);
      } else if (direction == back) {
        rightShare.spin(forward, speed, rpm);
        leftShare.spin(forward, speed, rpm);
      }
    } 

  }

  else if (motorNumber == 2) {
    flywheel.spin(forward, speed, rpm);
    motorShare.extend(cylinder1);
    motorShare.retract(cylinder2);
    leftShare.spin(forward);
    rightShare.spin(forward);

  }
  else if (motorNumber == 3) {
    flywheel.stop();
    motorShare.retract(cylinder1);
    motorShare.extend(cylinder2);
    if (direction == forw) {
      leftShare.spin(reverse, speed, rpm);
      rightShare.spin(reverse, speed, rpm);
    }
    if (direction == back) {
      leftShare.spin(forward, speed, rpm);
      rightShare.spin(forward, speed, rpm);
    }
  }
  else {
    flywheel.stop();
    leftShare.stop();
    rightShare.stop();
  }
}


void pidTurn(int target, double kp, double ki, double kd, double timeout) {

  int maintarget;
  if (target > 0) {
    maintarget = BrainInertial.rotation(degrees) + abs(target);
  } else {
    maintarget = BrainInertial.rotation(degrees) - abs(target);
  }
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 2.5;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;

  double gearRatio = 2; // driver/driven
  double wheelBase = 8; //inches: left to right
  double wheelCircum = 7.87401575; //inches
  double wheelLength = 6.5; //inches: front to back

  
  error = maintarget-(BrainInertial.rotation(degrees));
  int holdd = BrainInertial.rotation(degrees);
  
  Brain.Timer.reset();

  dtL.resetPosition();
  dtR.resetPosition();

  while (true) {
    
    int encoderDirection = 0;/////////////////////////////////////////////////////////////
    int err;
    int skibidi;
    if (Brain.Timer.value() > timeout && timeout != 0) {
      break;
    }
    double current = BrainInertial.rotation();
    printf("\033[31m");
    printf("Inertial %f\n", BrainInertial.rotation(degrees));
    printf("Error %f\n", error);
    printf("\033[32m");
    error = maintarget - current;


    derivative = error-lastError;
    if (fabs(error) < threshold) {
      dtL.stop();
      dtR.stop();
      break; //:D
    }
  
    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }

    if (integral > maxIntegral) integral = maxIntegral;
    if (integral < -maxIntegral) integral = -maxIntegral;

    double motorSpeed = (kp * error) + (ki * integral) + (kd * derivative);

    if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
    if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;

    dtL.spin(forward, motorSpeed, percent);
    dtR.spin(reverse, motorSpeed, percent);


    lastError = error;
    wait(200,msec);
  }
  dtL.stop();
  dtR.stop();

}

void pidDrive(int target, double kp, double ki, double timeout) { //timeout in seconds, can have decimal
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 10;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;
  
  Brain.Timer.reset();

  dtL.resetPosition();
  dtR.resetPosition();

  while(true) {
    double currentDistance = (dtL.position(degrees) + dtR.position(degrees)) / 2.0;
    if ((timeout != 0) && (Brain.Timer.value() > timeout)) {
      break;
    }


    printf("\033[34m");
    printf("Current Distance %f\n", currentDistance);
    printf("Error %f\n", error);
    printf("tim %f\n", Brain.Timer.value());
    //printf("Amperage %f\n", intake.current(amp));
    error = target - currentDistance;


    if (fabs(error) < threshold) {
      dtL.stop();
      dtR.stop();
      break;
    }

    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }
  

  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;



  double motorSpeed = (kp * error) + (ki * integral);

  if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
  if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;

  dtL.spin(forward, motorSpeed, percent);
  dtR.spin(forward, motorSpeed, percent);

  wait(20,msec);


  }
}


void init() { //initalization code
  pushPneumatic.pumpOn();
  pushPneumatic.retract(cylinder2);
  pushPneumatic.retract(cylinder2);
  leftShare.setMaxTorque(100,percent);
  rightShare.setMaxTorque(100,percent);
  flywheel.setMaxTorque(100,percent);
  rightShare.setVelocity(30, percent);
  leftShare.setVelocity(30, percent);
  flywheel.setVelocity(30, percent);
  spinMS(0, forw, 100, zero);
  pushPneumatic.retract(cylinder1);
}



void pidFlywheel(int target, double kp, double ki, double kd) { 
  double derivative = 0, integral = 0, lastError = 0, current = 0, error = 0, motorSpeed = 0;
  double threshold = 1, maxIntegral = 100, maxSpeed = 120; 
  int tick = 0;
  
  while (true) {
    tick++;
    current = flywheel.velocity(rpm);
    error = target - current;
    
    /*
    if (error <= threshold) {
      break;
    }
    */
    
    integral += error;
    derivative = error - lastError;

    /*
    if (integral > maxIntegral) {integral = maxIntegral;}
    if (-integral < -maxIntegral) {integral = -maxIntegral;}
    */

    motorSpeed = (error * kp) + (integral * ki) + (derivative * kd);
/*
    
    printf("\033[30m ---------------- tick %d", tick);
    printf(" ----------------\n all values in rpm\n");
    printf("\033[31m Motor Speed: %f\n", motorSpeed);
    printf("\033[32m Velocity: %f\n", flywheel.velocity(rpm));
    printf("\033[38;5;214m Velocity of Shooting Mechanism: %f\n", flywheel.velocity(rpm) * 25);
    printf("\n\n");
    printf("\033[35m Error: %f\n", error);
    printf(" where kp is equal to %f\n", kp);
    printf("\n");
    printf("\033[34m Integral: %f\n", integral);
    printf(" where ki is equal to %f\n", ki);
    printf("\n");
    printf("\033[91m Derivative: %f\n", derivative);
    printf(" where kd is equal to %f\n", kd);
    printf("\n\n\n");

*/
    if (motorSpeed > maxSpeed) {motorSpeed = maxSpeed;}
    if (-motorSpeed < -maxSpeed) {motorSpeed = -maxSpeed;}

    spinMS(2, forw, motorSpeed, one);

    lastError = error;

    wait(200, msec);
  }


}


void pidDriveC(int target, double kp, double ki, double kr, double angel, double timeout) { //with correction
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 10;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;

  double leftTurn = 0, rightTurn = 0;
  BrainInertial.setRotation(0,degrees);
  Brain.Timer.reset();
  double holdd = BrainInertial.rotation(degrees);
  direction += target;
  dtL.resetPosition();
  dtR.resetPosition();
  
  while(true) {
    double currentDistance = (dtL.position(degrees) + dtR.position(degrees)) / 2.0;
    if ((timeout != 0) && (Brain.Timer.value() > timeout)) {
      break;
    }

    printf("\033[34m");
    printf("Current Distance %f\n", currentDistance);
    printf("Error %f\n", error);
    //printf("Amperage %f\n", intake.current(amp));
    error = target - currentDistance;


    if (fabs(error) < threshold) {
      dtL.stop();
      dtR.stop();
      break;
    }

    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }
  

  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;



  double motorSpeed = (kp * error) + (ki * integral);

  if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
  if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;
  
  if (BrainInertial.rotation(degrees) > 0) {
    rightTurn = fabs(BrainInertial.rotation(degrees));
    leftTurn = 0;
    
  } 
  else if (BrainInertial.rotation(degrees) < 0) {
    rightTurn = fabs(BrainInertial.rotation(degrees));
    leftTurn = 0;
  }
  else {
    leftTurn = 0;
    rightTurn = 0;
  }
  printf("%f\n", BrainInertial.rotation(degrees));


  
  dtL.spin(forward, motorSpeed - (rightTurn*kr), percent);
  dtR.spin(forward, motorSpeed - (leftTurn*kr), percent);

  wait(20,msec);


  }
  Brain.Timer.reset();

  while (Brain.Timer.value() < 0.7) { // extra correction after thing
    dtL.spin(forward, (BrainInertial.rotation(degrees))*kr, percent);
    dtR.spin(forward, (BrainInertial.rotation(degrees))*kr, percent);
  }
  

  dtL.stop();
  dtR.stop();

}

void pidDriveCDEC(int target, double kp, double ki, double kr, double angel,double timeout) { //with correction
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 10;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;
  double holdd = BrainInertial.rotation(degrees);
  double leftTurn = 0, rightTurn = 0;
  
  Brain.Timer.reset();
  BrainInertial.setRotation(0, degrees);

  dtL.resetPosition();
  dtR.resetPosition();
  while(true) {
    double currentDistance = (dtL.position(degrees) + dtR.position(degrees)) / 2.0;
    if ((timeout != 0) && (Brain.Timer.value() > timeout)) {
      break;
    }

    printf("\033[34m");
    printf("Current Distance %f\n", currentDistance);
    printf("Error %f\n", error);
    //printf("Amperage %f\n", intake.current(amp));
    error = target - currentDistance;


    if (fabs(error) < threshold) {
      dtL.stop();
      dtR.stop();
      break;
    }

    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }
  

  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;



  double motorSpeed = (kp * error) + (ki * integral);

  if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
  if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;
  
  if (BrainInertial.rotation(degrees) > 0) {
    rightTurn = fabs(BrainInertial.rotation(degrees));
    leftTurn = 0;
  } 
  else if (BrainInertial.rotation(degrees) < 0) {
    leftTurn = fabs(BrainInertial.rotation(degrees));
    rightTurn = 0;
  }
  else {
    leftTurn = 0;
    rightTurn = 0;
  }
  printf("%f\n", BrainInertial.rotation(degrees));


  
  dtL.spin(forward, motorSpeed - (rightTurn*kr), percent);
  dtR.spin(forward, motorSpeed - (leftTurn*kr), percent);

  wait(20,msec);


  }
  Brain.Timer.reset();


  

  dtL.stop();
  dtR.stop();

}


void pidDriveCNR(int target, double kp, double ki, double kr, double angel,double timeout) { //with correction
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 10;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;
  double holdd = BrainInertial.rotation(degrees);
  double leftTurn = 0, rightTurn = 0;
  
  Brain.Timer.reset();


  dtL.resetPosition();
  dtR.resetPosition();
  while(true) {
    double currentDistance = (dtL.position(degrees) + dtR.position(degrees)) / 2.0;
    if ((timeout != 0) && (Brain.Timer.value() > timeout)) {
      break;
    }

    printf("\033[34m");
    printf("Current Distance %f\n", currentDistance);
    printf("Error %f\n", error);
    //printf("Amperage %f\n", intake.current(amp));
    error = target - currentDistance;


    if (fabs(error) < threshold) {
      dtL.stop();
      dtR.stop();
      break;
    }

    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }
  

  if (integral > maxIntegral) integral = maxIntegral;
  if (integral < -maxIntegral) integral = -maxIntegral;



  double motorSpeed = (kp * error) + (ki * integral);

  if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
  if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;
  
  if (BrainInertial.rotation(degrees) > 0) {
    rightTurn = fabs(BrainInertial.rotation(degrees));
    leftTurn = 0;
  } 
  else if (BrainInertial.rotation(degrees) < 0) {
    leftTurn = fabs(BrainInertial.rotation(degrees));
    rightTurn = 0;
  }
  else {
    leftTurn = 0;
    rightTurn = 0;
  }
  printf("%f\n", BrainInertial.rotation(degrees));


  
  dtL.spin(forward, motorSpeed - (rightTurn*kr), percent);
  dtR.spin(forward, motorSpeed - (leftTurn*kr), percent);

  wait(20,msec);


  }
  Brain.Timer.reset();


  

  dtL.stop();
  dtR.stop();

}
