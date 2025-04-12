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
controller Controller;
motor diffLeft = motor(PORT7, true);
motor diffRight = motor(PORT1, false);
motor leftDrive = motor(PORT9, true); // confirmed
motor rightDrive = motor(PORT3, false); // confirmed
motor intake = motor(PORT8);
motor metro = motor(PORT2);

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

bool backroller = 0;
void updateMotors();
void updateBackroller();
void updateCataMotors();

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Controller.ButtonLDown.pressed(updateMotors)
  Controller.ButtonLUp.pressed(updateMotors);
  Controller.ButtonLUp.released(updateMotors);
  Controller.ButtonLDown.released(updateMotors);

  Controller.ButtonRUp.pressed(updateCataMotors);
  Controller.ButtonRDown.pressed(updateCataMotors);
  Controller.ButtonRUp.pressed(updateCataMotors);
  Controller.ButtonRDown.pressed(updateCataMotors);
}

void updateBackroller() {
  if (backroller) backroller = 0;
  else { backroller = 1; }
}

void updateMotors() {
  if (Controller.ButtonLDown.pressing()) {
    diffLeft.spin(forward, 100, percent);
    diffRight.spin(reverse, 100, percent);
    intake.spin(reverse, 100, percent);

    if (!backroller) {
      metro.spin(forward, 100, percent);
    }
    else {
      metro.spin(reverse, 100, percent);
    }

  }
  else if (Controller.ButtonLUp.pressing()) {
    diffLeft.spin(reverse, 100, percent);
    diffRight.spin(forward, 100, percent);
    intake.spin(forward, 100, percent);
    if (!backroller) {
      metro.spin(forward, 100, percent);
    }
    else {
      metro.spin(reverse, 100, percent);
    }
  }
  else {
    diffLeft.stop();
    diffRight.stop();
    intake.stop();
    metro.stop();
  }
}

void updateCataMotors() {
  if (Controller.ButtonRDown.pressing()) {
    diffLeft.spin(reverse, 100, percent);
    diffRight.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) {
    diffLeft.spin(forward, 100, percent);
    diffRight.spin(forward, 100, percent);
  }
}
