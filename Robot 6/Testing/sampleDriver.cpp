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
motor leftDrive = motor(PORT2, true);
motor rightDrive = motor(PORT4, false);
motor pdgsLeft = motor(PORT3, false);
motor pdgsRight = motor(PORT6, true);
motor metroLeft = motor(PORT1, true);
motor metroRight = motor(PORT5, false);
pneumatic pneum1 = pneumatic(PORT8);
pneumatic pneum2 = pneumatic(PORT9);
touchled indicator = touchled(PORT7);

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

enum drive {tankdrive, splitarcade};

drive drivetype = tankdrive;

bool mPTO = 0;
bool sPTO = 0;
bool cPTO = 0;

void updateSPTO();
void updateMPTO();
void updateCPTO();
void updateMPTOmotors();


int main() {
  vexcodeInit();

  Controller.ButtonRDown.pressed(updateMPTO);
  Controller.ButtonEDown.pressed(updateSPTO);
  Controller.ButtonFUp.pressed(updateCPTO);

  Controller.ButtonLUp.pressed(updateMPTOmotors);
  Controller.ButtonLUp.released(updateMPTOmotors);
  Controller.ButtonLDown.pressed(updateMPTOmotors);
  Controller.ButtonLDown.released(updateMPTOmotors);

  while (true) {
    leftDrive.spin(forward, Controller.AxisA.position(), percent);
    rightDrive.spin(forward, Controller.AxisD.position(), percent);
    wait(20, msec);

  }
}
void updateSPTO() {
  printf("updated spto");
  if (!sPTO) {
    pneum2.extend(cylinder2);
    sPTO = 1;
  }
  else {
    pneum2.retract(cylinder2);
    sPTO = 0;
  }
}

void updateMPTO() {
  if (!mPTO) {
    pneum2.extend(cylinder1);
    mPTO = 1;
  }
  else {
    pneum2.retract(cylinder1);
    mPTO = 0;
  }
}

void updateCPTO() {
  if (!cPTO) {
    pneum1.extend(cylinder1);
    cPTO = 1;
  }
  else {
    pneum1.retract(cylinder1);
    cPTO = 0;
  }
}

void updateMPTOmotors() {
  if (Controller.ButtonLDown.pressing()) {
    if (mPTO) {
      metroLeft.spin(forward, 100, percent);
      metroRight.spin(forward, 100, percent);
    }
    else {
      pdgsLeft.spin(reverse, 100, percent);
      pdgsRight.spin(reverse, 100, percent);
      metroLeft.spin(reverse, 100, percent);
      metroRight.spin(reverse, 100, percent);
    }
  }
  else if (Controller.ButtonLUp.pressing()) {
    if (mPTO) {
      metroLeft.spin(reverse, 100, percent);
      metroRight.spin(reverse, 100, percent);
    }
    else {
      pdgsLeft.spin(forward, 100, percent);
      pdgsRight.spin(forward, 100, percent);
      metroLeft.spin(forward, 100, percent);
      metroRight.spin(forward, 100, percent);
    }
  }
  else {
    metroLeft.stop();
    metroRight.stop();
    pdgsLeft.stop();
    pdgsRight.stop();
  }

}

