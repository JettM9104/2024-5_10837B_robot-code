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
motor leftDrive = motor(PORT10, false);
motor rightDrive = motor(PORT4, true);

motor pushback = motor(PORT1);


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

bool driveActive = true;


int main() {

  while (true) {
    if (driveActive) {
      if ((abs(Controller.AxisA.position()) + abs(Controller.AxisC.position())) > 5) {
        leftDrive.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
        rightDrive.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
      }
      else {
        leftDrive.stop();
        rightDrive.stop();
      }
    }

    if (Controller.ButtonLUp.pressing()) {
      pushback.spin(reverse, 100, percent);
    }
    else if (Controller.ButtonLDown.pressing()) {
      pushback.spin(forward, 100, percent);
    }
    else {
      pushback.stop();
    }
    wait(20, msec);
  }
  return 0;
}