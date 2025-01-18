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
distance ballDetector = distance(PORT10);
touchled indicator = touchled(PORT8);

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
bool metroState = 1; // Puncher Connection to Conveyer, set default to extended
bool pdgsState = 0; // Pneumatic Driven Gear Shifter (Drivetrain to Conveyer)
bool ratchetState = 0;
bool charlesState = 0;
bool pumpState = 1;

bool macroActive;

// Function Declarations
void init();
void updateMetro();
void updatePDGS();
void updateConveyer();
void updateRatchet();
void updateCharles();
void updatePump();
void liftMacro();
void shootPuncher();
void windPuncher();


int main() { 
  vexcodeInit(); 

  init();

  Controller.ButtonRDown.pressed(updatePDGS);
  Controller.ButtonRUp.pressed(updateMetro);

  Controller.ButtonLUp.pressed(updateConveyer);
  Controller.ButtonLUp.released(updateConveyer);
  Controller.ButtonLDown.pressed(updateConveyer);
  Controller.ButtonLDown.released(updateConveyer);

  Controller.ButtonFUp.pressed(updateRatchet);

  Controller.ButtonFDown.pressed(updateCharles);
  Controller.ButtonR3.pressed(updatePump);

  Controller.ButtonEUp.pressed(liftMacro);
  Controller.ButtonEDown.pressed(shootPuncher);
  Controller.ButtonL3.pressed(windPuncher);

  while (true) { 
    if (!pdgsState) { 
      leftDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent); 
      rightDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
    }
    else {
      leftDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent); 
      rightDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
      ptoLeft.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent); 
      ptoRight.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
    }

    if (macroActive) {
      indicator.setColor(red);
    }
    else {
      if (pdgsState) {
        indicator.setColor(yellow);
      }
      else  {
        indicator.setColor(blue_green);
      }
    }
    wait(20, msec);
  } 
}

void init() {
  conveyer.setMaxTorque(100, percent);
  jett.extend(cylinder2); 
  pdgsState = 1;
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
    if (Controller.ButtonLDown.pressing()) {
      conveyer.spin(forward, 100, percent);
      ptoLeft.spin(forward, 100, percent);
      ptoRight.spin(forward, 100, percent);
    }
    else if (Controller.ButtonLUp.pressing()) {
      conveyer.spin(reverse, 100, percent);
      ptoLeft.spin(reverse, 100, percent);
      ptoRight.spin(reverse, 100, percent);

    }
    else {
      conveyer.stop();
      ptoLeft.stop();
      ptoRight.stop();
    }    
  }
  else {
    if (Controller.ButtonLDown.pressing()) {
      conveyer.spin(forward, 100, percent);
    }
    else if (Controller.ButtonLUp.pressing()) {
      conveyer.spin(reverse, 100, percent);
    }
    else {
      conveyer.stop();
      ptoLeft.stop();
      ptoRight.stop();
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

void updatePump() {
  if (pumpState) {
    jett.pumpOn();
    pumpState = 0;
  }
  else {
    jett.pumpOff();
    pumpState = 1;
  }
}

void liftMacro() {
  bool cancel = false;
  macroActive = true;
  if (!pdgsState) { 
    conveyer.spin(forward, 100, percent);
    ptoLeft.spin(forward, 100, percent);
    ptoRight.spin(forward, 100, percent);
  }
  else {
    conveyer.spin(forward, 100, percent);
  }
  while (Controller.ButtonEUp.pressing()) { wait(20, msec); }
  while (ballDetector.objectDistance(mm) > 100) { 
    if (Controller.ButtonEUp.pressing()) { cancel = true; break; } 
    wait(5, msec); 
  }

  conveyer.stop(); 
  ptoLeft.stop();
  ptoRight.stop(); 

  if (!cancel) {
    if (!pdgsState) { 
      wait(70, msec);
    }
    else {
      wait(200, msec);
    }
    updateCharles();
    wait(2, seconds);
    updateCharles();
  }
  macroActive = false;
}
void shootPuncher() {
  macroActive = true;
  grayson.extend(cylinder2);
  metroState = 1;

  jett.retract(cylinder1);
  ratchetState = 0;
  if (!pdgsState) {
    conveyer.spin(forward, 100, percent);
    ptoLeft.spin(forward, 100, percent);
    ptoRight.spin(forward, 100, percent);
  }
  else {
    conveyer.spin(forward, 100, percent);
  }
  
  wait(500, msec);
  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();

  wait(200, msec);
  grayson.retract(cylinder2);
  metroState = 0;
  macroActive = false;

}

void windPuncher() {
  grayson.extend(cylinder2);
  metroState = 1;
  macroActive = true;

  jett.extend(cylinder1);
  ratchetState = 1;
  while (Controller.ButtonL3.pressing()) { wait(20,  msec); }
  unsigned int x = 0;
  do {
    if (!pdgsState) {
      conveyer.spin(forward, 100, percent);
      ptoLeft.spin(forward, 100, percent);
      ptoRight.spin(forward, 100, percent);
    }
    else {
      conveyer.spin(forward, 100, percent);
    }
    x++;
    wait(20, msec);
    
    if (Controller.ButtonL3.pressing()) {
      conveyer.stop();
      break;
    }
  } while ((conveyerLeft.velocity(percent) > 3) || (x < 10));

  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();

  grayson.retract(cylinder2);
  metroState = 0;
  macroActive = false;

}