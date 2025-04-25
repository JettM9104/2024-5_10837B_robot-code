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
motor metroLeft = motor(PORT11);//
motor metroRight = motor(PORT1);

motor leftDrive = motor(PORT9, true); // confirmed
motor rightDrive = motor(PORT3, false); // confirmed

motor backIntake = motor(PORT10, true); // marked with x
motor frontintake = motor(PORT7);//
touchled indicator = touchled(PORT2);//

distance rpLoad = distance(PORT5);//


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
  while (true) {
    if (Controller.ButtonLUp.pressing()) { metroleft.spin(forward, 100, percent); } //
    else if (Controller.ButtonLDown.pressing()) { metroleft.spin(reverse, 100, percent); }////
    else  { metroleft.stop();}

    if (Controller.ButtonRUp.pressing()) { metroRight.spin(forward, 100, percent); }
    else if (Controller.ButtonRDown.pressing()) { metroRight.spin(reverse, 100, percent); } ////
    else { metroRight.stop(); }

    if (Controller.ButtonEUp.pressing()) { frontIntake.spin(forward, 100, percent); }
    else if (Controller.ButtonEDown.pressing()) { frontIntake.spin(reverse, 100, percent); } //
    else { frontIntake.stop(); }

    if (Controller.ButtonFUp.pressing()) { backIntake.spin(forward, 100, percent); }
    else if (Controller.ButtonFDown.pressing()) { backIntake.spin(reverse, 100, percent); }
    else { backIntake.stop(); }

    if ((abs(Controller.AxisA.position()) + abs(Controller.AxisC.position())) > 5) {
      leftDrive.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
      rightDrive.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
    }
    else {
      leftDrive.stop();
      rightDrive.stop();
    }

   
    wait(20, msec);
  }
  
}
