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
distance loadingZone = distance(PORT6);
distance chassis = distance(PORT8);
motor leftUpIntake = motor(PORT5, false);
motor leftDownIntake = motor(PORT4, false);
motor_group leftIntake = motor_group(leftUpIntake, leftDownIntake);
motor rightUpIntake = motor(PORT12, true);
motor rightDownIntake = motor(PORT11, true);
motor_group rightIntake = motor_group(rightUpIntake, rightDownIntake);
motor_group_group intake = motor_group_group(leftIntake, rightIntake);
motor leftDrive = motor(PORT3, true);
motor rightDrive = motor(PORT9, false);
touchled indicator = touchled(PORT1);

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

void updateIntakeMotors();
void directIntake();
void updateLED();
bool driveon = true;


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code

  Controller.ButtonLUp.pressed(updateIntakeMotors);
  Controller.ButtonLDown.pressed(updateIntakeMotors);
  Controller.ButtonLUp.released(updateIntakeMotors);
  Controller.ButtonLDown.released(updateIntakeMotors);

  Controller.ButtonEDown.pressed(directIntake);

  thread led = thread(updateLED);

  indicator.setBrightness(80);

  while (true) {
    if (driveon) {
      leftDrive.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
      rightDrive.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
    }
    wait(20, msec);
  }
  
}

void updateIntakeMotors() {
  if (Controller.ButtonLUp.pressing()) {
    intake.spin(forward, 100, percent);
  }
  else if (Controller.ButtonLDown.pressing()) {
    intake.spin(reverse, 100, percent);
  }
  else {
    intake.stop();
  }
}

void directIntake() { 
  driveon = false;
  bool quit = false;
  while (true) {
    while (loadingZone.objectDistance(mm) <= 440 || Controller.ButtonEUp.pressing()) { 
      if (Controller.ButtonEDown.pressing()) {
        quit = true;
        break;
      }
      wait(20, msec); 
    }
    
    if (quit) break;

    leftDrive.spin(forward);
    rightDrive.spin(forward);

    while (chassis.objectDistance(mm) <= 80 || Controller.ButtonEUp.pressing()) {
      if (Controller.ButtonEDown.pressing()) {
        quit = true;
        break;
      } 
      wait(20, msec); 
    }
    if (quit) break;

    leftDrive.spin(reverse);
    rightDrive.spin(reverse);
  }
  driveon = true;
}
void updateLED() {
  while (true) {
    if (loadingZone.objectDistance(mm) < 460) indicator.setColor(red);
    else {
      indicator.setColor(green);
    }
    wait(20, msec);
  }
}