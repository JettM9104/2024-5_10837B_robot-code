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
// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor leftDrive = motor(PORT2, true);
motor rightDrive = motor(PORT4, false);
motor pdgsLeft = motor(PORT3, false);
motor pdgsRight = motor(PORT6, true);
motor metroLeft = motor(PORT1, true);
motor metroRight = motor(PORT5, false);
pneumatic pneum1 = pneumatic(PORT8);
pneumatic pneum2 = pneumatic(PORT9);
touchled indicator = touchled(PORT7);


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

#pragma endregion VEXcode Generated Robot Configuration

//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       {author}                                                  
//    Created:      {date}                                                    
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------

// Include the IQ Library
#include "iq_cpp.h" 

// Allows for easier use of the VEX Library
using namespace vex;

enum motorCount { twoMotor, fourMotor };

void drive(double distance, double timeout = 0, motorCount pdgs = fourMotor);
void turn(double angle, double timeout = 0, motorCount pdgs = fourMotor);

double convertEncoderToDist(double gearRatio = 60/24, bool reversed = false, double wheelCirc = 200);
double convertDistToEncoder(double gearRatio = 60/24, bool reversed = false, double wheelCirc = 200);

namespace pid
{
    namespace drive { float kP = 0.4, kI = 0.2, kD = 0.3; }
    namespace turn { float kP = 0.65, kI = 0.2, kD = 6.7; }
    namespace correction { float kP = 0.1, kI = 0.1, kD = 0.1; }
    namespace curve { float kP = 0.1, kI = 0.1, kD = 0.1; }
    namespace decelerate { 
      namespace drive { float kP = 0.1, kI = 0.01, kD = 03; }
      namespace turn { float kP = 1, kI = 0.01, kD = 0.7; }
    }
}

// Refrences for ease of access of variables
float& dkP = pid::drive::kP, dkI = pid::drive::kI, dkD = pid::drive::kD;
float& tkP = pid::turn::kP, tkI = pid::turn::kI, tkD = pid::turn::kD;
float& ckP = pid::correction::kP, ckI = pid::correction::kI, ckD = pid::correction::kD;
float& ukP = pid::curve::kP, ukI = pid::curve::kI, ukD = pid::curve::kD;
float& ekP = pid::decelerate::drive::kP, ekI = pid::decelerate::drive::kI, ekD = pid::decelerate::drive::kD;
float& fkP = pid::decelerate::turn::kP, fkI = pid::decelerate::turn::kI, fkD = pid::decelerate::turn::kD;


#include <iostream>

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code
  
}

double convertEncoderToDist(double gearRatio, bool reversed, double wheelCirc) {
  return gearRatio * (wheelCirc / 360) * (reversed ? -1 : 1);
}

double convertDistToEncoder(double gearRatio, bool reversed, double wheelCirc) {
  return (1/gearRatio) * (360 / wheelCirc) * (reversed ? -1 : 1);
}


void drive(double distance, double timeout, motorCount pdgs) {
  double error = 0, integral = 0, derivative = 0, lastError = 0; 
  double correctionError = 0, correctionIntegral = 0, correctionDerivative = 0, correctionLastError = 0;
  double leftMS, rightMS;

  leftDrive.resetPosition();
  rightDrive.resetPosition();

  double beginTimer = Brain.Timer.value();

  while (true) {
    error = ((leftDrive.position(degrees) + rightDrive.position(degrees)) / 2) - convertDistToEncoder(distance);
    integral = error < 3 ? 0 : integral + error;
    derivative = error - lastError;

    correctionError = leftDrive.position(degrees) - rightDrive.position(degrees);
    correctionIntegral = correctionError < 3 ? 0 : correctionIntegral + correctionError;
    correctionDerivative = correctionError - correctionLastError;

    leftMS = ((error * dkP) + (integral * dkI) + (derivative * dkD)) - ((correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD));
    rightMS = ((error * dkP) + (integral * dkI) + (derivative * dkD)) + ((correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD));
    
    lastError = error;
    correctionLastError = correctionError;

    if (fabs(error) < 5) [[unlikely]] { break; }
    if (((Brain.Timer.value() - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }
  }
}

void turn(double angle, double timeout, motorCount pdgs) {
  double error = 0, integral = 0, derivative = 0, lastError = 0; 
  double correctionError = 0, correctionIntegral = 0, correctionDerivative = 0, correctionLastError = 0;
  double leftMS, rightMS;

  double angleEncoders;


  leftDrive.resetPosition();
  rightDrive.resetPosition();

  double beginTimer = Brain.Timer.value();

  BrainInertial.setRotation(0, degrees);

  while (true) {
    angleEncoders = ((leftDrive.position(degrees) + rightDrive.position(degrees)) / 2) * 200 * (60/24) / 250;
    error = ((BrainInertial.rotation(degrees) + angleEncoders) / 2) - angle;
    integral = error < 3 ? 0 : integral + error;
    derivative = error - lastError;

    correctionError = leftDrive.position(degrees) + rightDrive.position(degrees);
    correctionIntegral = correctionError < 3 ? 0 : correctionIntegral + correctionError;
    correctionDerivative = correctionError - correctionLastError;

    leftMS = ((error * dkP) + (integral * dkI) + (derivative * dkD)) - ((correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD));
    rightMS = ((error * dkP) + (integral * dkI) + (derivative * dkD)) + ((correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD));
    
    lastError = error;
    correctionLastError = correctionError;

    if (fabs(error) < 5) [[unlikely]] { break; }
    if (((Brain.Timer.value() - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }
  }
}

