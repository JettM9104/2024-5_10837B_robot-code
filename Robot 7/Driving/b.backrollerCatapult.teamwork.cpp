#pragma region VEXcode Generated Robot Configuration

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

brain Brain;

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)

controller Controller = controller();
inertial BrainInertial = inertial();
motor intakeCatapultLm = motor(PORT9, true);
motor intakeCatapultRm = motor(PORT3, false);
motor_group intakeCatapultm = motor_group(intakeCatapultLm, intakeCatapultRm);
motor intake = motor(PORT7);
motor backrollerIntakem = motor(PORT1);
motor leftDrive = motor(PORT10, true);
motor rightDrive = motor(PORT4, false);
bumper catapultSensor = bumper(PORT8);

void initializeRandomSeed(){
  wait(100,msec);
  double xAxis = BrainInertial.acceleration(xaxis) * 1000;
  double yAxis = BrainInertial.acceleration(yaxis) * 1000;
  double zAxis = BrainInertial.acceleration(zaxis) * 1000;
  int seed = int(
    xAxis + yAxis + zAxis
  );
  srand(seed); 
}

void vexcodeInit() {
  initializeRandomSeed(); 
}

#pragma endregion VEXcode Generated Robot Configuration

bool catapult = 0;
bool backroller = 0;

using namespace vex;

void updateCatapult();
void updateBackroller();

void updateMotors();

void windCata();

int main() {
  Controller.ButtonLUp.pressed(updateMotors);
  Controller.ButtonLUp.released(updateMotors);
  Controller.ButtonLDown.pressed(updateMotors);
  Controller.ButtonLDown.released(updateMotors);

  Controller.ButtonRDown.pressed(updateCatapult);
  Controller.ButtonRUp.pressed(updateBackroller);

  Controller.ButtonEUp.pressed(windCata);
  
  while (true) {
    leftDrive.spin(forward, (Controller.AxisA.position() + Controller.AxisC.position()), percent);
    rightDrive.spin(forward, (Controller.AxisA.position() - Controller.AxisC.position()), percent);
  }
}

void updateMotors() {
  if (Controller.ButtonLDown.pressing()) { // make balls go in
    intake.spin(forward);
    if (backroller) {
      backrollerIntakem.spin(reverse, 100, percent);
    }
    else {
      backrollerIntakem.spin(forward, 100, percent);
    }

    if (catapult) {
      intakeCatapultm.spin(reverse);
    }
    else {
      intakeCatapultm.spin(forward);
    }
  }

  else if (Controller.ButtonLUp.pressing()) {
    intake.spin(reverse);
  }

  else {
    intakeCatapultm.stop();
    intake.stop();
    backrollerIntakem.stop();
  }
}

void updateCatapult() {
  if (catapult) { catapult = 0; }
  else { catapult = 1; }
}

void updateBackroller() {
  if (backroller) { backroller = 0; }
  else { backroller = 1; }
}

void windCata() {
  intakeCatapultm.spin(reverse);

  while (!catapultSensor.pressing()) { wait(20, msec); }

  intakeCatapultm.stop();
}

