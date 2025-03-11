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


controller Controller = controller();

// Robot configuration code.
inertial BrainInertial = inertial();
distance loadingZone = distance(PORTNULL);
motor leftIntake = motor(PORTNULL, true);
motor rightIntake = motor(PORTNULL, false);
motor_group intake = motor_group(leftIntake, rightIntake);
motor frontLeftDrive = motor(PORTNULL, false);
motor backLeftDrive = motor(PORTNULL, false);
motor_group leftDrive = motor_group(frontLeftDrive, backLeftDrive);
motor frontRightDrive = motor(PORTNULL, true);
motor backRightDrive = motor(PORTNULL, true);
motor_group rightDrive = motor_group(frontRightDrive, backRightDrive);


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

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code
  
}

void directIntake() { 
  bool quit = false;


  unsigned long long int i = 0;

  while (Controller.ButtonL3.pressing()) wait(20, msec);
  
  while (!Controller.ButtonL3.pressing()) {
    Brain.Timer.value();

    if (Controller.ButtonL3.pressing()) { break; }
    leftDrive.spin(forward, 100, percent);
    rightDrive.spin(forward, 100, percent);


    while (fabs(leftDrive.velocity(percent)) >= 3 || i < 100) { 
      i++; 
      if (Controller.ButtonL3.pressing()) {
        quit = true;
        break;
      }
      wait(20, msec); 
    } 
    
    if (quit) break;

    leftDrive.resetPosition();
    rightDrive.resetPosition();

    Brain.Timer.reset();

    leftDrive.spin(reverse, 100, percent);
    rightDrive.spin(reverse, 100, percent);

    
    while (!(leftDrive.position(degrees) < -140 || Brain.Timer.value() > 1.3)) { 
      if (Controller.ButtonL3.pressing()) {
        quit = true;
        break;
      }
      wait(20, msec); 
    }
    if (quit) break;


    i = 0;
  }
}
