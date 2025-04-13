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


inertial BrainInertial = inertial();
controller Controller;
motor diffLeft = motor(PORT7, true);
motor diffRight = motor(PORT4, false);
motor leftDrive = motor(PORT9, true); // confirmed
motor rightDrive = motor(PORT3, false); // confirmed
motor intake = motor(PORT8);
motor metro = motor(PORT5);
distance chassis = distance(PORT1);
touchled indicator = touchled(PORT2);
bumper catapultDetector = bumper(PORT10);

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

double delta_t = 0.02;


int main() {
  vexcodeInit();
  init();

  printf("-------------------------------\n");

  double x = Brain.Battery.voltage(volt);
  double y = Brain.Battery.current(amp);
  int z = Brain.Battery.capacity(percent);
  printf("Voltage: %f V\n", x);
  printf("Current: %f A\n", y);
  printf("Capacity: %d percent\n", z);
  printf("Wattage: %f W\n", x*y);



}

void init() {
  //sigms
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

    motorSpeed = (error * kp) + (integral * delta_t * ki) + (derivative / delta_t * kd);

    float leftMotorSpeed = motorSpeed, rightMotorSpeed = motorSpeed;

    if (leftMotorSpeed > lmaxSpeed) leftMotorSpeed = lmaxSpeed;
    if (leftMotorSpeed < -lmaxSpeed) leftMotorSpeed = -lmaxSpeed;

    if (rightMotorSpeed > rmaxSpeed) rightMotorSpeed = rmaxSpeed;
    if (rightMotorSpeed < -rmaxSpeed) rightMotorSpeed = -rmaxSpeed;
    
    leftDrive.spin(forward, leftMotorSpeed - (BrainInertial.rotation(degrees) - beginInertial), percent);
    rightDrive.spin(forward, rightMotorSpeed + (BrainInertial.rotation(degrees) - beginInertial), percent);

    if (fabs(error) < 5) { break; }
    if (timeout != 0 && (Brain.Timer.value() - beginTimer) > timeout) { break; }
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

    motorSpeed = (error * kp) + (integral * delta_t * ki) + (derivative  / delta_t * kd);
    
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

