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
motor leftDrive = motor(PORT3, false);
motor rightDrive = motor(PORT9, true);

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


void init();

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int lmaxSpeed = 100, const unsigned short int rmaxSpeed = 100);
void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);


int main() {
  vexcodeInit();
  init();

  BrainInertial.setRotation(0, degrees);


  drive(530, 1, 0.01, 0.1, 100, 100);
  wait(500, msec);
  turn(-130, 0.5, 0.01, 0.5, 2.7, 100);
  printf("%f\n", BrainInertial.rotation(degrees));

  
  if (BrainInertial.rotation(degrees) > -88) {
    while ((BrainInertial.rotation(degrees) > -88 && BrainInertial.rotation(degrees) < -91)) {
      leftDrive.spin(reverse, 100, percent);
      rightDrive.spin(forward, 100, percent);
    }
  }
  else if (BrainInertial.rotation(degrees) < -91) {
    while ((BrainInertial.rotation(degrees) > -88 && BrainInertial.rotation(degrees) < -91)) {
      leftDrive.spin(reverse, 100, percent);
      rightDrive.spin(forward, 100, percent);
    }
  }
  wait(500, msec);

  drive(-100000, 1000, 1000, 0, 2, 100, 100);

  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);

//////


  
  drive(420, 1, 0.01, 0.1, 0, 100, 100);

  

  drive(100, 1, 0.01, 0.1, 0, 100, 0);

  

  wait(500, msec);


  
  drive(-100000, 1000, 1000, 0, 2, 100, 100);
  drive (200, 0.8, 0.01, 0.5, 0.5, 0, 100);
  
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);
  wait(500, msec);

  

  wait(1000, msec);

  while (true) {
    drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
    drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);

  }

}

void init() {


  leftDrive.setMaxTorque(100, percent);
  leftDrive.setVelocity(100, percent);
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}


void drive(const float distance, const float kp, const float ki, const float kd, const float timeout, const unsigned short int lmaxSpeed, const unsigned short int rmaxSpeed) {
  float error = distance - (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2;
  float integral = 0, derivative = 0, lastError = 0;
  float motorSpeed;

  float beginTimer = Brain.Timer.value();
  float beginInertial = BrainInertial.rotation(degrees);

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
    
    leftDrive.spin(forward, leftMotorSpeed - (BrainInertial.rotation(degrees) - beginInertial), percent);
    rightDrive.spin(forward, rightMotorSpeed + (BrainInertial.rotation(degrees) - beginInertial), percent);

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
    rightDrive.spin(reverse, motorSpeed, percent);

    if (fabs(error) < 5) break;
    if (timeout != 0 && (Brain.Timer.value() - beginTimer) > timeout) break;
    lastError = error;
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}

