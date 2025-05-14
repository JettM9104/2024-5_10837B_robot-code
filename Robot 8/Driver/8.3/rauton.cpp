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

const float kP_turn = 0.36;
const float kI_turn = 0.01;
const float kD_turn = 0.42;

// Thresholds
const float errorThreshold = 30.0;     // Acceptable error margin
const float angleErrorThreshold = 1.8;     // Acceptable error margin
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
void pidDrive(double targetDistance, int maxSpeed = 50, int settleTime = settleTimeDefault) {
  resetDriveEncoders();
  Brain.Timer.reset();
  BrainInertial.resetRotation();

  float integral = 0;
  float lastError = 0;

  float gyroCorrection;
  float initialHeading = BrainInertial.rotation();
  float error, derivative;
  int settleTimer = 0;

  while (settleTimer < settleTime) {
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
    gyroCorrection = (initialHeading - currentHeading) * 6.8;  // Correction gain

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
void pidTurn(double targetAngle, int maxSpeed = 50, int settleTime = settleTimeDefault) {
  Brain.Timer.reset();
  float integral = 0;
  float lastError = 0;
  int settleTimer = 0;

  while (settleTimer < settleTime) {
    float currentAngle = BrainInertial.heading(degrees);
    float error = targetAngle - currentAngle;

    if (fabs(error) < 5.0) {
      integral += error;
    }

    if (fabs(error) < 0.7) {
      integral = 0;
    }

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


int main() {
  //while (!indicator.pressing()) wait(20, msec);

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

  //while (!indicator.pressing()) wait(20, msec);

  while (!catapultSensor.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();

  //while (!indicator.pressing()) wait(20, msec);

  BrainInertial.setHeading(0, degrees); // change to zero after

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code
  //pidDrive(610, 100, 250);
  //pidDrive(-940, 100, 250);
  Brain.playSound(siren);
  //pidTurn(90, 100, 250);
  Brain.playSound(siren);
  pidTurn(-90, 100, 250);
  Brain.playSound(siren);

}