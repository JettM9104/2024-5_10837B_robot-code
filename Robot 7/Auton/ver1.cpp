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

void therd();


int main() {
  vexcodeInit();
  init();

  while (rapidLoad.objectDistance(mm) > 20) wait(20, msec);

  while ((Brain.Battery.capacity(percent) < 95)) wait(20, msec);
  BrainInertial.setRotation(0, degrees);
  intake.spinFor(forward, 300, degrees, false);
  intakeCatapultm.spinFor(reverse, 300, degrees, false);
  backrollerIntakem.spinFor(forward, 300, degrees, false);

  drive(400, 1, 0.01, 0.1, 100, 100);
  wait(500, msec);
  turn(-1100, 0.5, 0.01, 0.5, 2.7, 100);
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

  thread hi = thread(therd);

  wait(1000, msec);
  
  shootCata();

  wait(1000, msec);

  shootCata();


  intake.spin(forward, 100, percent);
  intakeCatapultm.spin(reverse, 100, percent);
  backrollerIntakem.spin(forward, 100, percent);


  wait(3000, msec);






  // - second part
  thread sigma = thread(windCata);

  intake.spin(forward, 100, percent);
  backrollerIntakem.spin(forward, 100, percent);

  
  drive(420, 1, 0.01, 0.1, 0, 100, 100);

  intakeCatapultm.spin(reverse, 100, percent);
  

  turn(65, 1, 0.01, 0.1, 0, 100);

  



  wait(500, msec);


  
  drive(-100000, 1000, 1000, 0, 2, 100, 100);
  drive (200, 0.8, 0.01, 0.5, 0.5, 0, 100);
  
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);

  shootCata();
  wait(500, msec);
  straightForward();

  wait(500, msec);
  intake.spin(forward, 100, percent);
  intakeCatapultm.spin(reverse, 100, percent);
  backrollerIntakem.spin(forward, 100, percent);

  

  wait(1000, msec);

  while (true) {
    drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
    drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
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

void therd() {drive(-100000, 1000, 1000, 0, 3, 100, 100);}

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


void windCata() {
  intakeCatapultm.spin(forward, 100, percent);
  while (!catapultSensor.pressing()) { wait(20, msec); }
  wait(20, msec);
  intakeCatapultm.stop();
}

void shootCata() {
  intakeCatapultm.spin(forward, 100, percent);
  wait(500, msec);
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

