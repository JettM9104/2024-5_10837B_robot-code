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
distance loadingZone = distance(PORT7);
motor leftUpIntake = motor(PORT7, false);
motor leftDownIntake = motor(PORT4, false);
motor_group leftIntake = motor_group(leftUpIntake, leftDownIntake);
motor rightUpIntake = motor(PORT10, true);
motor rightDownIntake = motor(PORT11, true);
motor_group rightIntake = motor_group(rightUpIntake, rightDownIntake);
motor_group_group intake = motor_group_group(leftIntake, rightIntake);
motor leftDrive = motor(PORT3, true);
motor rightDrive = motor(PORT9, false);




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

void driveTo(int target, double timeout); 

bool driveon = true;

float kP = 0.1;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code

  Controller.ButtonLUp.pressed(updateIntakeMotors);
  Controller.ButtonLDown.pressed(updateIntakeMotors);
  Controller.ButtonLUp.released(updateIntakeMotors);
  Controller.ButtonLDown.released(updateIntakeMotors);

  Controller.ButtonL3.pressed(directIntake);

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
  unsigned long long int i = 0;
  while (Controller.ButtonL3.pressing()) wait(20, msec);
  while (!Controller.ButtonL3.pressing()) {

    intake.spin(reverse, 100, percent);
    driveTo(-300, 2);
    driveon = true;

    
    while (leftDrive.position(degrees) > -180) { wait(20, msec); }

    while (loadingZone.objectDistance(mm) < 400) { wait(20, msec); }
    while (loadingZone.objectDistance(mm) > 400) { wait(20, msec); }
    driveTo(400, 2);
    driveon = true;

    leftDrive.spin(forward, 30, percent);
    rightDrive.spin(forward, 30, percent);
    while (fabs(leftDrive.velocity(percent)) >= 3 || i < 100) { 
      i++; 
      wait(20, msec); 
    } 
    printf("done iteration\n");
    
  }


}

void driveTo(int target, double timeout) {
  double error;
  leftDrive.resetPosition();
  rightDrive.resetPosition();
  Brain.Timer.reset();
  while (true) {
    error = target - leftDrive.position(degrees);

    if ((fabs(Controller.AxisA.position()) + fabs(Controller.AxisB.position()) + fabs(Controller.AxisC.position()) + fabs(Controller.AxisD.position())) == 0) {
      driveon = false;
      leftDrive.spin(forward, error * kP, percent);
      rightDrive.spin(forward, error * kP, percent);
    }
    else {
      driveon = true;
    }

    if (fabs(error) < 10) break;
    if (Brain.Timer.value() > timeout) break;
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}
