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
motor leftDrive = motor(PORT9, true);
motor rightDrive = motor(PORT3, false);
motor leftMetro = motor(PORT12);
motor leftIntake = motor(PORT11);
motor rightMetro = motor(PORT6);
motor rightIntake = motor(PORT4);
touchled indicator = touchled(PORT2);
distance rpLoad = distance(PORT5);
optical chassis = optical(PORT10);
bumper catapultDetector = bumper(PORT7);
distance intakeChassis = distance(PORT8);


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

// ---- Variable Declaration & Definitions ----
unsigned short int catapultState = 0;
bool errors[3] = {false, false, false};
u_int8_t macrosActive = 0;
bool driveActive = true;

// ---- Function Declarations ----
// --- Initilitation Functions ---
void init();
void checkPorts();

// --- Motor Updates ---
void updateIntake();
void updateCatapult();

// --- Thread (Not Macros) ---
void updateLED();

// -- Macros --
void windCata();
void shootCata();
void rapidLoad();

// -- Macro Triggers --
void triggerWindCata();
void triggerShootCata();
void triggerRapidLoad();

// ---- Main Function ----
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  checkPorts();
  init();

  Controller.ButtonLUp.pressed(updateIntake);
  Controller.ButtonLUp.released(updateIntake);
  Controller.ButtonLDown.pressed(updateIntake);
  Controller.ButtonLDown.released(updateIntake);

  Controller.ButtonRUp.pressed(updateCatapult);
  Controller.ButtonRUp.released(updateCatapult);
  Controller.ButtonRDown.pressed(updateCatapult);
  Controller.ButtonRDown.released(updateCatapult);

  Controller.ButtonEUp.pressed(triggerShootCata);
  Controller.ButtonEDown.pressed(triggerWindCata);
  Controller.ButtonFDown.pressed(triggerRapidLoad);
  
  thread led = thread(updateLED);

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
    wait(20, msec);
  }
}


// ---- Function Definitions ----
// --- Macro Triggers ---
void triggerWindCata() {
  thread thd1 = thread(windCata);
  thd1.setPriority(5);
  thd1.join();
}
void triggerShootCata() {
  thread thd2 = thread(shootCata);
  thd2.setPriority(4);
  thd2.join();
}
void triggerRapidLoad() {
  thread thd3 = thread(rapidLoad);
  thd3.setPriority(3);
  thd3.join();
}

// --- Initilization Functions ---
// -- Initilization Function --
void init() {
  leftDrive.setMaxTorque(100, percent);
  rightDrive.setMaxTorque(100, percent);
  leftMetro.setMaxTorque(100, percent);
  rightMetro.setMaxTorque(100, percent);
  leftIntake.setMaxTorque(100, percent);
  rightIntake.setMaxTorque(100, percent);

  leftDrive.setVelocity(100, percent);
  rightDrive.setVelocity(100, percent);
  leftMetro.setVelocity(100, percent);
  rightMetro.setVelocity(100, percent);
  leftIntake.setVelocity(100, percent);
  rightIntake.setVelocity(100, percent);

  for (int i = 0; i < 3; i++) {
    indicator.setColor(blue_green);
    wait(100, msec);
    indicator.setColor(colorType::none);
    wait(100, msec);
  }
}

// -- Port Check Function --
void checkPorts() {
  unsigned short int y = 1;
  Brain.Screen.setFont(mono15);
  Brain.Screen.setCursor(y, 1);

  if (!leftDrive.installed()) {
    y++;
    Brain.Screen.print("9-leftDrive");
  }
  Brain.Screen.setCursor(y, 1);

  if (!rightDrive.installed()) {
    y++;
    Brain.Screen.print("3-rightDrive");
  }
  Brain.Screen.setCursor(y, 1);
  if (!leftMetro.installed()) {
    y++;
    Brain.Screen.print("12-leftMetro");
  }
  Brain.Screen.setCursor(y, 1);

  if (!rightMetro.installed()) {
    y++;
    Brain.Screen.print("6-rightMetro");
  }
  Brain.Screen.setCursor(y, 1);

  if (!leftIntake.installed()) {
    y++;
    Brain.Screen.print("11-leftIntake");
  }
  Brain.Screen.setCursor(y, 1);

  if (!rightIntake.installed()) {
    y++;
    Brain.Screen.print("4-rightIntake");
  }
  Brain.Screen.setCursor(y, 1);
  if (!rpLoad.installed()) {
    y++;
    Brain.Screen.print("5-rpLoad");
  }
  Brain.Screen.setCursor(y, 1);

  if (!indicator.installed()) {
    y++;
    Brain.Screen.print("2-indicator");
  }
  Brain.Screen.setCursor(y, 1);

  if (!chassis.installed()) {
    y++;
    Brain.Screen.print("10-chassis");
  }
  Brain.Screen.setCursor(y, 1);
  if (y > 1){
    Brain.Screen.print("PLUG EM IN");
    errors[0] = true;
  } 
}

// --- Motor Updates ---
// -- Intake Motor Update --
void updateIntake() {
  if (Controller.ButtonLDown.pressing()) {
    if (catapultState != 1) {
      rightMetro.spin(forward, 100, percent);
    }
    if (catapultState != 2) {
      leftMetro.spin(reverse, 100, percent);
    }
    leftIntake.spin(reverse, 100, percent);
    rightIntake.spin(forward, 100, percent);
  }
  else if (Controller.ButtonLUp.pressing()) {
    leftIntake.spin(forward, 100, percent);
    rightIntake.spin(reverse, 100, percent);
  }

  else {
    leftIntake.stop();
    rightIntake.stop();
    if (catapultState == 0) {
      leftMetro.stop();
      rightMetro.stop();
    }
  }
}

// -- Catapult Motor Update --
void updateCatapult() {
  if (Controller.ButtonRDown.pressing()) {
    catapultState = 1;
    leftMetro.spin(forward, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) {
    catapultState = 2;
    rightMetro.spin(reverse, 100, percent);
  }

  else {
    catapultState = 0;
    leftMetro.stop();
    rightMetro.stop();
  }
}

// --- Thread Functions (Not Macros) ---
// -- LED Update --
void updateLED() {
  while (true) {
    if (errors[0] || errors[1] || errors[2]) {
      indicator.setColor(red);
      wait(300, msec);
      indicator.setColor(colorType::none);
      wait(300, msec);
    }
    else {
      indicator.setColor(blue_green);
    }
    wait(20, msec);
  }
}

// --- Macros ---
// -- Wind Catapult --
void windCata() {
  macrosActive++;
  while (Controller.ButtonEDown.pressing()) wait(20, msec);

  while (!catapultDetector.pressing() && !Controller.ButtonEDown.pressing()) {
    rightMetro.spin(reverse, 100, percent);
  }
  rightMetro.stop();
  leftMetro.stop();
  macrosActive--;
}

// -- Shoot Catapult --
void shootCata() {
  macrosActive++;
  while (Controller.ButtonEUp.pressing()) wait(20, msec);

  Brain.Timer.reset();
  while ((Brain.Timer.value() < 0.5) && !Controller.ButtonEUp.pressing()) {
    leftMetro.spin(forward, 100, percent);
  }
  rightMetro.stop();
  leftMetro.stop();
  macrosActive--;
}


// -- Rapid Load --
void rapidLoad() { 
  macrosActive++;
  while (Controller.ButtonFDown.pressing()) wait(20, msec);
  driveActive = false;
  while (true) {
    if (Controller.ButtonFDown.pressing()) break;
    if (catapultState != 1) {
      rightMetro.spin(forward, 100, percent);
    }
    if (catapultState != 2) {
      leftMetro.spin(reverse, 100, percent);
    }
    leftIntake.spin(reverse, 100, percent);
    rightIntake.spin(forward, 100, percent);
    while (!((((rpLoad.objectDistance(inches) < 33) && (rpLoad.objectDistance(inches) > 28)) && !chassis.isNearObject()) || Controller.ButtonFDown.pressing())) wait(20, msec);
    printf("2\n");
    if (Controller.ButtonFDown.pressing()) break;
    printf("3\n");
    leftDrive.spin(reverse, -100, percent);
    rightDrive.spin(reverse, -100, percent);

    Brain.Timer.reset();
    while (rpLoad.objectDistance(inches) > 28 && !Controller.ButtonFDown.pressing()) wait(20, msec);
    if (Controller.ButtonFDown.pressing()) break;
    printf("4\n");
    leftDrive.spin(forward, -100, percent);
    rightDrive.spin(forward, -100, percent);

    Brain.Timer.reset();
    while (Brain.Timer.value() < 0.6 && !Controller.ButtonFDown.pressing()) wait(20, msec);
    if (Controller.ButtonFDown.pressing()) break;
    printf("5\n");
    leftDrive.stop();
    rightDrive.stop();
  }
  driveActive = true;
  printf("b\n");
  rightIntake.stop();
  leftIntake.stop();
  rightMetro.stop();
  leftMetro.stop();
  macrosActive--;
}

