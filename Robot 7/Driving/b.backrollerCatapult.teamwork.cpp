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
motor intakeCatapultLm = motor(PORT12, false);
motor intakeCatapultRm = motor(PORT6, true);
motor_group intakeCatapultm = motor_group(intakeCatapultLm, intakeCatapultRm);
motor intake = motor(PORT7, true);
motor backrollerIntakem = motor(PORT1);
motor leftDrive = motor(PORT3, true);
motor rightDrive = motor(PORT9, false);
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

void init();

int main() {
  vexcodeInit();

  init();

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
    updateMotors();

    wait(20, msec);
  }
}

void init() {
  intake.setMaxTorque(100, percent);
  intake.setVelocity(100, percent);

  intakeCatapultm.setMaxTorque(100, percent);
  intakeCatapultm.setVelocity(100, percent);

  backrollerIntakem.setMaxTorque(100, percent);
  backrollerIntakem.setVelocity(100, percent);

  leftDrive.setMaxTorque(100, percent);
  leftDrive.setVelocity(100, percent);
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}

void updateMotors() {
  if (Controller.ButtonLDown.pressing()) { // make balls go in
    printf("backroller is %d\ncatapult is %d\n\n", backroller, catapult);

    intake.spin(forward);

    if (backroller) {
      backrollerIntakem.spin(forward);
    }
    else {
      backrollerIntakem.spin(reverse);
    }

    if (catapult) {
      intakeCatapultm.spin(forward);
    }
    else {
      intakeCatapultm.spin(reverse);
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
  intakeCatapultm.spin(reverse, 100, percent);

  while (!catapultSensor.pressing()) { wait(20, msec); }

  intakeCatapultm.stop();
}