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
motor leftDrivetrain = motor(PORT1, false);
motor rightDrivetrain = motor(PORT2, true);
pneumatic pdgs = pneumatic(PORT3);
pneumatic metro = pneumatic(PORT8);

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



// Include the IQ Library
#include "iq_cpp.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Variables for the State of the Pneumatic Pistons
// 0 for Retracted, 1 for Extended
bool metroState = 0; // Puncher Connection to Conveyer
bool pdgsState = 0; // Pneumatic Driven Gear Shifter (Drivetrain to Conveyer)

// Function Declarations
void updateMetro();
void updatePDGS();


int main() { 
  vexcodeInit(); 

  Controller.ButtonRDown.pressed(updatePDGS);
  Controller.ButtonRUp.pressed(updateMetro);

  while (true) { 
    leftDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent); 
    rightDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
  } 
}

void updatePDGS() {
  if (!pdgsState) {
    pdgs.extend(cylinder1); // Assuming they are connected into one sector, cylinder1
    metroState = 1;
  }
  else {
    pdgs.retract(cylinder1);
    metroState = 0;
  }
}

void updateMetro() {
  if (!metroState) {
    metro.extend(cylinder1); // Assuming they are connected into one sector, cylinder1
    metroState = 1;
  }
  else {
    metro.retract(cylinder1);
    metroState = 0;
  }
}