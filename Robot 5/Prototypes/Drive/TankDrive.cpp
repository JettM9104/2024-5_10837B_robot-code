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
motor leftDrivetrain = motor(PORT9, true);
motor rightDrivetrain = motor(PORT3, false);
pneumatic jett = pneumatic(PORT1);
pneumatic grayson = pneumatic(PORT7);
motor ptoLeft = motor(PORT6, false);
motor ptoRight = motor(PORT12, true);
motor conveyerLeft = motor(PORT11, false);
motor conveyerRight = motor(PORT5, true);
motor_group conveyer = motor_group(conveyerLeft, conveyerRight);

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
bool ratchetState = 0;
bool charlesState = 0;

// Function Declarations
void init();
void updateMetro();
void updatePDGS();
void updateConveyer();
void updateRatchet();
void updateCharles();


int main() { 
  vexcodeInit(); 

  Controller.ButtonRDown.pressed(updatePDGS);
  Controller.ButtonRUp.pressed(updateMetro);

  Controller.ButtonLUp.pressed(updateConveyer);
  Controller.ButtonLUp.released(updateConveyer);
  Controller.ButtonLDown.pressed(updateConveyer);
  Controller.ButtonLDown.released(updateConveyer);

  Controller.ButtonFUp.pressed(updateRatchet);

  Controller.ButtonFDown.pressed(updateCharles);

  while (true) { 
    if (!pdgsState) { 
      leftDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent); 
      rightDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
    }
    else {
      leftDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent); 
      rightDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
      ptoLeft.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent); 
      ptoRight.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
    }
    wait(20, msec);
  } 
}

void init() {
  conveyer.setMaxTorque(100, percent);
}


void updatePDGS() {
  if (!pdgsState) {
    jett.extend(cylinder2); 
    pdgsState = 1;
  }
  else {
    jett.retract(cylinder2);
    pdgsState = 0;
  }
}

void updateMetro() {
  if (!metroState) {
    grayson.extend(cylinder2);
    metroState = 1;
  }
  else {
    grayson.retract(cylinder2);
    metroState = 0;
  }
}

void updateConveyer() {
  if (!pdgsState) { 
    if (Controller.ButtonLUp.pressing()) {
      conveyer.spin(forward, 100, percent);
      ptoLeft.spin(forward, 100, percent);
      ptoRight.spin(forward, 100, percent);
    }
    else if (Controller.ButtonLDown.pressing()) {
      conveyer.spin(reverse, 100, percent);
      ptoLeft.spin(reverse, 100, percent);
      ptoRight.spin(reverse, 100, percent);

    }
    else {
      conveyer.stop();
    }    
  }
  else {
    if (Controller.ButtonLUp.pressing()) {
      conveyer.spin(forward, 100, percent);
    }
    else if (Controller.ButtonLDown.pressing()) {
      conveyer.spin(reverse, 100, percent);
    }
    else {
      conveyer.stop();
    }
  }
}

void updateRatchet() {
  if (!ratchetState) {
    jett.extend(cylinder1);
    ratchetState = 1;
  }
  else {
    jett.retract(cylinder1);
    ratchetState = 0;
  }
}

void updateCharles() {
  if (!charlesState) {
    grayson.extend(cylinder1);
    charlesState = 1;
  }
  else {
    grayson.retract(cylinder1);
    charlesState = 0;
  }
}