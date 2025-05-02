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

motor frontRightMetro = motor(PORT5);
motor backRightMetro = motor(PORT6);
motor intake = motor(PORT11);
motor leftMetro = motor(PORT12);

touchled indicator = touchled(PORT1);

distance sideWall = distance(PORT2);
distance rpLoad = distance(PORT4);

optical intakeSensor = optical(PORT8);
optical backrollerSensor = optical(PORT10);

bumper catapultSensor = bumper(PORT7);

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

bool backroller = 1;

void updateIntake();
void updateCatapult();

void toggleBackroller();

void triggerWindCatapult();
void triggerShootCatapult();

void windCatapult();
void shootCatapult();

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  Controller.ButtonLDown.pressed(updateIntake);
  Controller.ButtonLDown.released(updateIntake);
  Controller.ButtonLUp.pressed(updateIntake);
  Controller.ButtonLUp.released(updateIntake);

  Controller.ButtonRDown.pressed(updateCatapult);
  Controller.ButtonRDown.released(updateCatapult);
  Controller.ButtonRUp.pressed(updateCatapult);
  Controller.ButtonRUp.released(updateCatapult);

  Controller.ButtonFUp.pressed(toggleBackroller);

  Controller.ButtonEDown.pressed(triggerWindCatapult);

  // Begin project code
  while (true) {
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

void triggerWindCatapult() {
  thread thd1 = thread(windCatapult);
  thd1.join();
  thd1.setPriority(5);
}

void triggerShootCatapult() {
  thread thd2 = thread(shootCatapult);
  thd2.join();
  thd2.setPriority(4);
}

void toggleBackroller() {
  if (backroller) {
    backroller = 0;
  }
  else {
    backroller = 1;
  }
}

void updateIntake() {
  if (Controller.ButtonLDown.pressing()) {
    intake.spin(reverse, 100, percent);
    backRightMetro.spin(forward, 100, percent);
    leftMetro.spin(reverse, 100, percent);

    if (backroller) {
      frontRightMetro.spin(forward, 100, percent);
    }
    else {
      frontRightMetro.spin(reverse, 100, percent);
    }
  }
  else if (Controller.ButtonLUp.pressing()) {
    intake.spin(forward, 100, percent);
    if (backroller) {
      frontRightMetro.spin(forward, 100, percent);
    }
  }
  else {
    intake.stop();
    backRightMetro.stop();
    leftMetro.stop();
    frontRightMetro.stop();
  }
}

void updateCatapult() {
  if (Controller.ButtonRDown.pressing()) {
    backRightMetro.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) {
    leftMetro.spin(forward, 100, percent);
  }
  else {
    backRightMetro.stop();
    leftMetro.stop();
  }
}

void windCatapult() {
  while (Controller.ButtonEDown.pressing()) wait(20, msec);
  while (!catapultSensor.pressing() && !Controller.ButtonEDown.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();
}

void shootCatapult() {
  while (Controller.ButtonEUp.pressing()) wait(20, msec);
  Brain.Timer.reset();
  while (Brain.Timer.value() < 0.8 && !Controller.ButtonEUp.pressing()) {
    leftMetro.spin(forward, 100, percent); 
    wait(20, msec);
  }
  leftMetro.stop();
}


