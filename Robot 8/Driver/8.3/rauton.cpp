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


// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor leftMotor = motor(PORT9, true);
motor rightMotor = motor(PORT3, false);


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

//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       Jett Mu                                                  
//    Created:      Tues May 13th AT WORLDSSSS                                                   
//    Description:  Lets gooooo                                                
//                                                                            
//----------------------------------------------------------------------------

// Include the IQ Library
#include "vex.h"
#include "iq_cpp.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

// Allows for easier use of the VEX Library
using namespace vex;

// Constants
const float kP_drive = 0.37;
const float kI_drive = 0.0;
const float kD_drive = 0.3;

const float kP_turn = 0.70;
const float kI_turn = 0.09;
const float kD_turn = 0.45;

// Thresholds
const float errorThreshold = 30.0;     // Acceptable error margin
const float angleErrorThreshold = 1.0;     // Acceptable error margin
const int settleTimeDefault = 250;    // msInertialSensor

// Robot configuration code.
//inertial BrainInertial = inertial();
motor leftDrive = motor(PORT9, true);
motor rightDrive = motor(PORT3);

motor frontRightMetro = motor(PORT5);
motor backRightMetro = motor(PORT6);
motor intake = motor(PORT11);
motor leftMetro = motor(PORT12);

touchled indicator = touchled(PORT1);
distance rpLoad = distance(PORT4);

optical intakeSensor = optical(PORT2);
distance backrollerSensor = distance(PORT10);

bumper catapultSensor = bumper(PORT7);

// Helper function to reset encoders
void resetDriveEncoders() {
  leftMotor.resetPosition();
  rightMotor.resetPosition();
}

// Clamp function to keep output within a specific range
double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

// PID Drive Function with Gyro Correction
void pidDrive(double targetDistance, int maxSpeed = 50, int settleTime = settleTimeDefault, int timeout = 3000) {
  resetDriveEncoders();
  Brain.Timer.reset();
  BrainInertial.resetRotation();

  float integral = 0;
  float lastError = 0;

  float gyroCorrection;
  float initialHeading = BrainInertial.rotation();
  float error, derivative;
  int settleTimer = 0;

  while (settleTimer < settleTime && Brain.Timer.time(msec) < timeout) {
    float leftPos = leftMotor.position(degrees);
    float rightPos = rightMotor.position(degrees);
    float avgPos = (leftPos + rightPos) / 2.0;

    error = targetDistance - avgPos;
    integral += error;
    derivative = error - lastError;

    float output = (kP_drive * error) + (kI_drive * integral) + (kD_drive * derivative);
    output = clamp(output, -maxSpeed, maxSpeed);

    // Gyro correction
    float currentHeading = BrainInertial.rotation();
    gyroCorrection = (initialHeading - currentHeading) * 9.3;  // Correction gain

    Brain.Screen.print("%d", BrainInertial.rotation(degrees));

    leftMotor.spin(forward, output + gyroCorrection, percent);
    rightMotor.spin(forward, output - gyroCorrection, percent);

    if (fabs(error) < errorThreshold)
      settleTimer += 20;
    else
      settleTimer = 0;

    lastError = error;
    wait(20, msec);
  }

  leftMotor.stop(brake);
  rightMotor.stop(brake);
}

// PID Turn Function using Inertial Sensor
void pidTurn(double targetAngle, int maxSpeed = 50, int settleTime = settleTimeDefault, int timeout = 3000) {
  Brain.Timer.reset();
  float integral = 0;
  float lastError = 0;
  int settleTimer = 0;

  while (settleTimer < settleTime && Brain.Timer.time(msec) < timeout) {
    float currentAngle = BrainInertial.heading(degrees);
    float error = targetAngle - currentAngle;

    if (fabs(error) < 7.0) {
      integral += error;
    }

    if (fabs(error) < 0.6) {
      integral = 0;
    }

    // Normalize error to [-180, 180]
    if (error > 180) error -= 360;
    if (error < -180) error += 360;

    //integral += error;
    float derivative = error - lastError;

    float output = (kP_turn * error) + (kI_turn * integral) + (kD_turn * derivative);
    output = clamp(output, -maxSpeed, maxSpeed);

    Brain.Screen.print("%d", BrainInertial.heading(degrees));

    leftMotor.spin(forward, output, percent);
    rightMotor.spin(reverse, output, percent);

    if (fabs(error) < angleErrorThreshold)
      settleTimer += 20;
    else
      settleTimer = 0;

    lastError = error;
    wait(20, msec);
  }

  leftMotor.stop(brake);
  rightMotor.stop(brake);
}

void init() {


  leftDrive.setMaxTorque(100, percent);
  leftDrive.setVelocity(100, percent);
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}

void spinUntilDetect() {
  intake.spin(reverse, 50, percent);
  backRightMetro.spin(forward, 50, percent);
  leftMetro.spin(reverse, 50, percent);
  frontRightMetro.spin(reverse, 50, percent);

  while (!intakeSensor.isNearObject()) wait(20, msec);

  intake.setStopping(hold);
  backRightMetro.setStopping(hold);
  leftMetro.setStopping(hold);
  frontRightMetro.setStopping(hold);

  intake.spin(reverse, 2, percent);
  backRightMetro.spin(forward, 1, percent);
  leftMetro.spin(reverse, 2, percent);
  frontRightMetro.spin(reverse, 2, percent);
  // intake.stop();
  // backRightMetro.stop();
  // leftMetro.stop();
  // frontRightMetro.stop();
}

void windCata() {
  while (!catapultSensor.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();
  backRightMetro.spin(forward, 100, percent); 
}


int main() {
  init();
  while (!indicator.pressing()) wait(20, msec);

  BrainInertial.calibrate();
  // Print that the Inertial Sensor is calibrating while
  // waiting for it to finish calibrating.
  while(BrainInertial.isCalibrating()){
     indicator.setColor(white);
      Brain.Screen.clearScreen();
      Brain.Screen.print("Inertial Calibrating");
      wait(50, msec);
  }
  
  if (Brain.Battery.capacity(percent) > 95) indicator.setColor(green);
  else if (Brain.Battery.capacity(percent) > 90) indicator.setColor(yellow);
  else if (Brain.Battery.capacity(percent) > 85) indicator.setColor(orange);
  else { indicator.setColor(red); }

  while (!indicator.pressing()) wait(20, msec);

  while (!catapultSensor.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();

  while (!Controller.ButtonR3.pressing()) wait(20, msec);

  thread spud = thread(spinUntilDetect);

  BrainInertial.setHeading(0, degrees); // change to zero after

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code
  pidDrive(673, 100, 500, 3000);
  Brain.playSound(siren);
  pidTurn(90, 100, 500, 3000);
  Brain.playSound(siren);
  pidDrive(-980, 100, 250, 3000);
  Brain.playSound(siren);
  pidDrive(100, 100, 250, 1000);
  Brain.playSound(siren);
  pidDrive(-2000,100,250, 1000);

  intake.stop();
  backRightMetro.stop();
  leftMetro.stop();
  frontRightMetro.stop();

  Brain.Timer.reset();
  while (Brain.Timer.value() < 0.8) {
    leftMetro.spin(forward, 100, percent); 
    wait(20, msec);
  }
  Brain.Timer.reset();
  while (Brain.Timer.value() < 1.69) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }

  backRightMetro.spin(reverse, 100, percent); 
  intake.spin(reverse, 100, percent);
  backRightMetro.spin(forward, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  frontRightMetro.spin(reverse, 100, percent);

  while (backrollerSensor.objectDistance(mm) > 60) wait(20, msec);

  while (backrollerSensor.objectDistance(mm) < 60) wait(20, msec);

  intake.stop();
  backRightMetro.stop();
  leftMetro.stop();
  frontRightMetro.stop();

  ///// wind cata


  thread ohio = thread(windCata);


  printf("inital curve\n");
  pidDrive(200,100,250, 3000);

  intake.spin(reverse, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  //frontRightMetro.spin(reverse, 100, percent);

  pidTurn(140,100,500, 5000);
  Brain.playSound(siren);

  pidDrive(645,100,250, 5000);
  Brain.playSound(siren);

  pidTurn(90,100,250, 5000);
  Brain.playSound(siren);

  pidDrive(-620,100,250, 3000);
  Brain.playSound(siren);

  pidDrive(100, 100, 250, 1000);
  Brain.playSound(siren);

  pidDrive(-2000,100,250, 1000);
  Brain.playSound(siren);
  
  Brain.Timer.reset();
  while (Brain.Timer.value() < 0.8) {
    leftMetro.spin(forward, 100, percent); 
    wait(20, msec);
  }

  Brain.Timer.reset();
  while (Brain.Timer.value() < 2.0) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }

  intake.spin(reverse, 100, percent);
  backRightMetro.spin(forward, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  frontRightMetro.spin(reverse, 100, percent);
  while (true) {
    if ((rpLoad.objectDistance(inches) > 28 && rpLoad.objectDistance(inches) < 33) && backrollerSensor.objectDistance(mm) > 60) {
      leftDrive.spin(forward);
      rightDrive.spin(forward);

      wait(350, msec);

      leftDrive.spin(reverse);
      rightDrive.spin(reverse);
      wait(550, msec);
      leftDrive.stop();
      rightDrive.stop();
    }
    wait(20, msec);
  }
}