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


using namespace vex;


void windCata();
void shootCata();
void straightForward();

void init();

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);
void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);
void curve(const float radius, const float arcTheta, const float kp, const float ki, const float kd, const float timeout = 0, const float maxSpeed = 70);

const float pi = 3.14159;

int main() {
  vexcodeInit();
  init();

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

// implement timeout for curve function
// ensure data types are sufficicemt amd typedefs are ok

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout, const unsigned short int maxSpeed) {
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

void curve(const float radius, const float arcTheta, const float kp, const float ki, const float kd, const float timeout, const float maxSpeed) {
  float wheelBase = 20.0; 
  float arcLength = (pi / 180) * arcTheta * radius;
  
  float innerRadius = radius - (wheelBase / 2);
  float outerRadius = radius + (wheelBase / 2);
  float innerArcLength = (pi / 180) * arcTheta * innerRadius;
  float outerArcLength = (pi / 180) * arcTheta * outerRadius;

  float leftError, rightError;
  float leftIntegral = 0, rightIntegral = 0;
  float leftDerivative, rightDerivative;
  float leftLastError = 0, rightLastError = 0;

  float beginTimer = Brain.Timer.value();

  float rightMaxSpeed, leftMaxSpeed;

  if (innerArcLength > outerArcLength) { // this is assuming innter arc lenth is the left if not switch 
    rightMaxSpeed = maxSpeed;
    leftMaxSpeed = maxSpeed * innerArcLength / outerArcLength;
  }
  if (innerArcLength < outerArcLength) { 
    leftMaxSpeed = maxSpeed;
    rightMaxSpeed = maxSpeed * outerArcLength / innerArcLength;
  }
  else { // they are equal (likely imposible but nice to have)
    rightMaxSpeed = maxSpeed;
    leftMaxSpeed = maxSpeed;
  }
  
  leftDrive.resetPosition();
  rightDrive.resetPosition();

  while (true) {
    float leftPosition = leftDrive.position(degrees);
    float rightPosition = rightDrive.position(degrees);

    leftError = innerArcLength - leftPosition;
    rightError = outerArcLength - rightPosition;

    leftIntegral = leftIntegral <= 3 ? 0 : leftIntegral + leftError;
    rightIntegral = rightIntegral <= 3 ? 0 : rightIntegral + rightError;

    leftDerivative = leftError - leftLastError;
    rightDerivative = rightError - rightLastError;

    float leftSpeed = (kp * leftError) + (ki * leftIntegral) + (kd * leftDerivative);
    float rightSpeed = (kp * rightError) + (ki * rightIntegral) + (kd * rightDerivative);

    if (fabs(leftSpeed) > leftMaxSpeed) {
      if (leftSpeed > leftMaxSpeed) leftSpeed = leftMaxSpeed;
      if (-leftSpeed < -leftMaxSpeed) leftSpeed = -leftMaxSpeed;
    }

    if (fabs(rightSpeed) > rightMaxSpeed) {
      if (rightSpeed > rightMaxSpeed) rightSpeed = rightMaxSpeed;
      if (-rightSpeed < -rightMaxSpeed) rightSpeed = -rightMaxSpeed;
    }

    if (arcTheta > 0) { 
        leftDrive.spin(forward, leftSpeed, percent);
        rightDrive.spin(forward, rightSpeed, percent);
    } else {
        leftDrive.spin(forward, rightSpeed, percent);
        rightDrive.spin(forward, leftSpeed, percent);
    }

    if (fabs(leftError) < 3 && fabs(rightError) < 3) break;
    if (((Brain.Timer.value() - beginTimer) > timeout) && (timeout != 0)) break;

    leftLastError = leftError;
    rightLastError = rightError;

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
  wait(500, msec);
  backrollerIntakem.stop();
  intakeCatapultm.stop();
}