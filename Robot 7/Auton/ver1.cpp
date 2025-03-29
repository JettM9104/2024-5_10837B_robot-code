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
distance rapidLoad = distance(PORT4);

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


using namespace vex;


void windCata();
void shootCata();
void straightForward();

void init();

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int lmaxSpeed = 100, const unsigned short int rmaxSpeed = 100);
void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);


int main() {
  vexcodeInit();
  init();

  BrainInertial.setRotation(0, degrees);
  intake.spinFor(forward, 800, degrees, false);
  intakeCatapultm.spinFor(reverse, 800, degrees, false);
  backrollerIntakem.spinFor(forward, 800, degrees, false);

  drive(160, 0.8, 0.01, 0.5);
  intake.stop();
  intakeCatapultm.stop();
  backrollerIntakem.stop();
  printf("a\n");
  drive(600, 0.8, 0.01, 0.5, 0, 0, 100);


  printf("b\n");
  drive(-800, 0.8, 0.01, 0.5, 0, 100, 62);
  printf("c\n");

  drive(-800, 0.8, 0.01, 0.5, 1.2, 55, 100);

  drive(-100000, 1000, 1000, 0, 2, 100, 100);
  shootCata();
  wait(500, msec);
  straightForward();

  wait(500, msec);

  intake.spin(forward);
  intakeCatapultm.spin(reverse);
  backrollerIntakem.spin(forward);

  while (true) {
    drive(70, 1, 0.01, 0.5, 0.4, 100, 100);
    drive(-100, 1, 0.01, 0.5, 0.6, 100, 100);
    Brain.Timer.reset();
    while(rapidLoad.objectDistance(mm) < 150) {
      if (Brain.Timer.value() > 2) break;
      wait(20, msec);
    }
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
  leftDrive.setVelocity(100, percent   );
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout, const unsigned short int lmaxSpeed, const unsigned short int rmaxSpeed) {
  float error = distance - (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2;
  float integral = 0, derivative = 0, lastError = 0;
  float motorSpeed;

  float beginTimer = Brain.Timer.value();

  leftDrive.resetPosition();
  rightDrive.resetPosition();

  while (true) {
    error = distance - (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2;
    integral = integral <= 3 ? 0 : error + integral;
    derivative = error - lastError;

    motorSpeed = (error * kp) + (integral * ki) + (derivative * kd);

    float leftMotorSpeed = motorSpeed, rightMotorSpeed = motorSpeed;

    if (leftMotorSpeed > lmaxSpeed) leftMotorSpeed = lmaxSpeed;
    if (leftMotorSpeed < -lmaxSpeed) leftMotorSpeed = -lmaxSpeed;

    if (rightMotorSpeed > rmaxSpeed) rightMotorSpeed = rmaxSpeed;
    if (rightMotorSpeed < -rmaxSpeed) rightMotorSpeed = -rmaxSpeed;
    
    leftDrive.spin(forward, leftMotorSpeed, percent);
    rightDrive.spin(forward, rightMotorSpeed, percent);

    if (fabs(error) < 5) { printf("break by threshold\n"); break; }
    if (timeout != 0 && (Brain.Timer.value() - beginTimer) > timeout) { printf("break by timeout"); break; }
    lastError = error;
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}

void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout, const unsigned short int maxSpeed) {
  float error = rawTheta - (leftDrive.position(degrees) - rightDrive.position(degrees)) / 2;
  float integral = 0, derivative = 0, lastError = 0;
  float motorSpeed;

  float beginTimer = Brain.Timer.value();

  leftDrive.resetPosition();
  rightDrive.resetPosition();

  while (true) {
    error = rawTheta - (leftDrive.position(degrees) - rightDrive.position(degrees)) / 2;
    integral = integral <= 3 ? 0 : error + integral;
    derivative = error - lastError;

    motorSpeed = (error * kp) + (integral * ki) + (derivative * kd);
    
    leftDrive.spin(forward, motorSpeed, percent);
    rightDrive.spin(forward, motorSpeed, percent);

    if (error < 5) break;
    if (timeout != 0 && (timeout - beginTimer) > timeout) break;
    lastError = error;
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}


void windCata() {
  intakeCatapultm.spin(forward, 100, percent);
  while (!catapultSensor.pressing()) { wait(20, msec); }
  wait(20, msec);
  intakeCatapultm.stop();
}

void shootCata() {
  intakeCatapultm.spin(forward, 100, percent);
  wait(400, msec);
  intakeCatapultm.stop();
}

void straightForward() {
  backrollerIntakem.spin(reverse);
  intakeCatapultm.spin(forward);
  wait(700, msec);
  intakeCatapultm.stop();

  wait(200, msec);
  backrollerIntakem.stop();
  
  
}