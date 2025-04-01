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
bool motorsactive = 1;
bool macrosRunning = 0;
bool interrupt = 0;
using namespace vex;

void updateCatapult();
void updateBackroller();

void updateMotors();

void windCata();
void shootCata();
void straightForward();

void init();

int main() {
  vexcodeInit();

  init();

  Controller.ButtonLUp.pressed(updateMotors);
  Controller.ButtonLUp.released(updateMotors);
  Controller.ButtonLDown.pressed(updateMotors);
  Controller.ButtonLDown.released(updateMotors);

  Controller.ButtonRDown.pressed(updateCatapult);
  Controller.ButtonRUp.pressed(straightForward);

  Controller.ButtonEDown.pressed(windCata);
  Controller.ButtonEUp.pressed(shootCata);

  unsigned short int i = 0;

  double average = 0;
  
  while (true) {
    i++;
    leftDrive.spin(forward, (Controller.AxisA.position() + Controller.AxisC.position()), percent);
    rightDrive.spin(forward, (Controller.AxisA.position() - Controller.AxisC.position()), percent);

    average += intakeCatapultm.current(amp);

    if (i >= 100) {
      average /= 100;
      printf("%f\n", average);
    }


    if (motorsactive) {
      updateMotors();
    }
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

  intake.setStopping(brake);
  backrollerIntakem.setStopping(brake);
  intakeCatapultm.setStopping(brake);
}

void updateMotors() {
  if (Controller.ButtonLDown.pressing()) { // make balls go in

    intake.spin(forward);

    backrollerIntakem.spin(forward);

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
  interrupt = true;
  macrosRunning = true;
  motorsactive = false;
  intakeCatapultm.spin(forward, 100, percent);

  while (!catapultSensor.pressing()) { intakeCatapultm.spin(forward, 100, percent);wait(20, msec); }
  wait(20, msec);

  intakeCatapultm.stop();
  motorsactive = true;
  macrosRunning = false;
  interrupt = false;

}

void shootCata() {
  if (!macrosRunning) {
    while (true) {
      if (interrupt) {break;}
      motorsactive = false;
      if (interrupt) {break;}
      intakeCatapultm.resetPosition();
      intakeCatapultm.spin(forward, 100, percent);

      if (interrupt) {break;}
      // while (intakeCatapultm.position(degrees) < 200) wait(20, msec);

      wait(550, msec);
      if (interrupt) {break;}
      intakeCatapultm.stop();
      if (interrupt) {break;}
      motorsactive = true;
      break;
    }
  } 
}


void straightForward() {
  if (!macrosRunning) {
    while (true) {
      if (interrupt) {break;}
      motorsactive = false;
      if (interrupt) {break;}
      backrollerIntakem.spin(reverse);
      intakeCatapultm.spin(forward);
      intakeCatapultm.spin(forward, 100, percent);
      while (!catapultSensor.pressing()) { wait(20, msec); }
      wait(20, msec);
      intakeCatapultm.stop();
      if (interrupt) {break;}
      intakeCatapultm.stop();
      if (interrupt) {break;}
      wait(200, msec);
      if (interrupt) {break;}
      backrollerIntakem.stop();
      if (interrupt) {break;}
      motorsactive = true;
      break;
    }
  }
}