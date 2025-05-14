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

touchled indicator = touchled(PORT1);
bumper catapultSensor = bumper(PORT7);

distance sideDistance = distance(PORT8);
distance rapidLoad = distance(PORT4);
distance directIntake = distance(PORT10);

optical intakeSensor = optical(PORT2);

motor rightDrive = motor(PORT3, true);
motor leftDrive = motor(PORT9, false);

motor frontRightMetro = motor(PORT5);
motor rearRightMetro = motor(PORT6);
motor frontLeft = motor(PORT11);
motor rearLeftMetro = motor(PORT12);


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

void updateIndexer();
void updateFlywheel();

void updateLiftState();

void macro();

bool liftState;
bool driveOn = true;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code

  Controller.ButtonRUp.pressed(updateIndexer);
  Controller.ButtonRUp.released(updateIndexer);
  Controller.ButtonRDown.pressed(updateIndexer);
  Controller.ButtonRDown.released(updateIndexer);

  Controller.ButtonLUp.pressed(updateFlywheel);
  Controller.ButtonLUp.released(updateFlywheel);
  Controller.ButtonLDown.pressed(updateFlywheel);
  Controller.ButtonLDown.released(updateFlywheel);

  Controller.ButtonFUp.pressed(macro);



  //while (true) { if (driveOn) { rightDrive.spin(forward, Controller.AxisA.position(), percent); } else { rightDrive.stop(); } }
}

void updateIndexer() {
  if (Controller.ButtonRDown.pressing()) {
    rearLeftMetro.spin(forward, 100, percent);
    leftDrive.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) {
    rearLeftMetro.spin(reverse, 100, percent);
    leftDrive.spin(forward, 100, percent);
  }
  else {
    rearLeftMetro.stop();
    leftDrive.stop();
  }
}

void updateFlywheel() {
  if (Controller.ButtonLDown.pressing()) {
    frontRightMetro.spin(reverse, 100, percent);
    rearRightMetro.spin(forward, 100, percent);
    frontLeft.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonLUp.pressing()) {
    frontRightMetro.spin(forward, 100, percent);
    rearRightMetro.spin(reverse, 100, percent);
    frontLeft.spin(forward, 100, percent);
  }
  else {
    frontRightMetro.stop();
    rearRightMetro.stop();
    frontLeft.stop();
  }
}

void updateLiftState() {
  if (liftState) {
    liftState = 0;
  }
  else {
    liftState = 1;
  }
}

void macro() {

  indicator.setColor(green);
  while (true) {
    
    printf("1\n");
    frontRightMetro.spin(reverse, 100, percent);
    rearRightMetro.spin(forward, 100, percent);
    frontLeft.spin(reverse, 100, percent);

    driveOn = false;
    while (Controller.ButtonFUp.pressing()) wait(20, msec);
    while (directIntake.objectDistance(inches) > 15 && !Controller.ButtonFUp.pressing()) wait(20, msec);
    printf("2\n");
    indicator.setColor(green);

    if (Controller.ButtonFUp.pressing()) break;

    rightDrive.spin(reverse, 100, percent);
    rearLeftMetro.spin(reverse, 100, percent);
    leftDrive.spin(forward, 100, percent);


    while (sideDistance.objectDistance(mm) > 200 && !Controller.ButtonFUp.pressing()) wait(20, msec);
    if (Controller.ButtonFUp.pressing()) break;
    indicator.setColor(red);
    printf("4\n");


    rightDrive.spin(forward, 100, percent);

    rearLeftMetro.spin(forward, 100, percent);
    leftDrive.spin(reverse, 100, percent);

    printf("5\n");
  }
  indicator.setColor(colorType::none);
  rightDrive.stop();
  rearLeftMetro.stop();
  leftDrive.stop();
  frontRightMetro.stop();
  rearRightMetro.stop();
  frontLeft.stop();

    rightDrive.spin(forward, 100, percent);

    rearLeftMetro.spin(forward, 100, percent);
    leftDrive.spin(reverse, 100, percent);

    wait(800, msec);

        rightDrive.stop();
    rearLeftMetro.stop();
    leftDrive.stop();


}