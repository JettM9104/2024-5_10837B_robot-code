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
class motor_group_group {
private:
  motor_group motora;
  motor_group motorb;

public:
  motor_group_group(motor_group motora, motor_group motorb): motora(motora), motorb(motorb) {}

  void spin(directionType dir, int32_t velocity, percentUnits units) {
    motora.spin(dir, velocity, units);
    motorb.spin(dir, velocity, units);
  }
  void spin(directionType dir) {
    motora.spin(dir);
    motorb.spin(dir);
  }
  void stop() {
    motora.stop();
    motorb.stop();
  }
};


// Robot configuration code.
inertial BrainInertial = inertial();
distance side = distance(PORT1);
touchled indicator = touchled(PORT2);
motor rightDrive = motor(PORT3);
motor rightFlywheel = motor(PORT4);
motor ohio = motor(PORT5);
motor rightMetro = motor(PORT6); ////
bumper catapultDetector = bumper(PORT7);
motor leftDrive = motor(PORT9, true);

motor leftFlywheel = motor(PORT11, true);
motor leftMetro = motor(PORT12, true); ////

motor_group flywheel = motor_group(leftFlywheel, rightFlywheel);


bool driveDone = true;
bool cancel = false;


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

  
}

void updateIndexer() {
  if (Controller.ButtonRDown.pressing()) {
    leftMetro.spin(reverse, 100, percent);
    leftDrive.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) {
    leftMetro.spin(forward, 100, percent);
    leftDrive.spin(forward, 100, percent);
  }
  else {
    leftMetro.stop();
    leftDrive.stop();
  }
}

void updateFlywheel() {
  if (Controller.ButtonLDown.pressing()) {
    rightMetro.spin(forward, 100, percent);
    flywheel.spin(forward, 100, percent);
    ohio.spin(reverse);
  }
  else if (Controller.ButtonLUp.pressing()) {
    rightMetro.spin(reverse, 100, percent);
    ohio.spin(forward);
    flywheel.spin(reverse, 100, percent);
  }
  else {
    rightMetro.stop();
    flywheel.stop();
    ohio.stop();
  }
}