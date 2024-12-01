#include "vex.h"

using namespace vex;

// Motor configuration
motor frontLeft(PORT1, ratio18_1, false);
motor frontRight(PORT2, ratio18_1, true);
motor backLeft(PORT3, ratio18_1, false);
motor backRight(PORT4, ratio18_1, true);

// PID Constants
double kP_drive = 0.2;
double kI_drive = 0.01;
double kD_drive = 0.1;

double kP_turn = 0.3;
double kI_turn = 0.02;
double kD_turn = 0.1;

// Function for resetting motors and sensors
void resetMotorsAndSensors() {
  frontLeft.resetRotation();
  frontRight.resetRotation();
  backLeft.resetRotation();
  backRight.resetRotation();
}

// Function to calculate the average encoder value
double averageEncoderValue() {
  return (fabs(frontLeft.rotation(degrees)) +
          fabs(frontRight.rotation(degrees)) +
          fabs(backLeft.rotation(degrees)) +
          fabs(backRight.rotation(degrees))) / 4.0;
}

// PID function for driving
void PIDDrive(double targetDistance, double maxSpeed = 100) {
  resetMotorsAndSensors();

  double error, prevError = 0;
  double integral = 0;
  double derivative = 0;
  double speed;

  while (true) {
    // Calculate error
    double currentPosition = averageEncoderValue();
    error = targetDistance - currentPosition;

    // Integral and derivative
    integral += error;
    derivative = error - prevError;

    // Calculate motor speed
    speed = (error * kP_drive) + (integral * kI_drive) + (derivative * kD_drive);

    // Limit motor speed
    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < -maxSpeed) speed = -maxSpeed;

    // Set motor speeds
    frontLeft.spin(forward, speed, pct);
    frontRight.spin(forward, speed, pct);
    backLeft.spin(forward, speed, pct);
    backRight.spin(forward, speed, pct);

    // Exit condition
    if (fabs(error) < 5) break;

    prevError = error;
    wait(20, msec);
  }

  // Stop motors
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

// PID function for turning
void PIDTurn(double targetAngle, double maxSpeed = 100) {
  resetMotorsAndSensors();

  double error, prevError = 0;
  double integral = 0;
  double derivative = 0;
  double speed;

  while (true) {
    // Calculate error (assumes a gyro sensor is used for heading)
    double currentAngle = InertialSensor.heading(degrees);
    error = targetAngle - currentAngle;

    // Integral and derivative
    integral += error;
    derivative = error - prevError;

    // Calculate motor speed
... (36 lines left)
Collapse
message.txt
4 KB
﻿
L U C K Y
lucky_richard
 
 
```
#include "vex.h"

using namespace vex;

// Motor configuration
motor frontLeft(PORT1, ratio18_1, false);
motor frontRight(PORT2, ratio18_1, true);
motor backLeft(PORT3, ratio18_1, false);
motor backRight(PORT4, ratio18_1, true);

// PID Constants
double kP_drive = 0.2;
double kI_drive = 0.01;
double kD_drive = 0.1;

double kP_turn = 0.3;
double kI_turn = 0.02;
double kD_turn = 0.1;

// Function for resetting motors and sensors
void resetMotorsAndSensors() {
  frontLeft.resetRotation();
  frontRight.resetRotation();
  backLeft.resetRotation();
  backRight.resetRotation();
}

// Function to calculate the average encoder value
double averageEncoderValue() {
  return (fabs(frontLeft.rotation(degrees)) +
          fabs(frontRight.rotation(degrees)) +
          fabs(backLeft.rotation(degrees)) +
          fabs(backRight.rotation(degrees))) / 4.0;
}

// PID function for driving
void PIDDrive(double targetDistance, double maxSpeed = 100) {
  resetMotorsAndSensors();

  double error, prevError = 0;
  double integral = 0;
  double derivative = 0;
  double speed;

  while (true) {
    // Calculate error
    double currentPosition = averageEncoderValue();
    error = targetDistance - currentPosition;

    // Integral and derivative
    integral += error;
    derivative = error - prevError;

    // Calculate motor speed
    speed = (error * kP_drive) + (integral * kI_drive) + (derivative * kD_drive);

    // Limit motor speed
    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < -maxSpeed) speed = -maxSpeed;

    // Set motor speeds
    frontLeft.spin(forward, speed, pct);
    frontRight.spin(forward, speed, pct);
    backLeft.spin(forward, speed, pct);
    backRight.spin(forward, speed, pct);

    // Exit condition
    if (fabs(error) < 5) break;

    prevError = error;
    wait(20, msec);
  }

  // Stop motors
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

// PID function for turning
void PIDTurn(double targetAngle, double maxSpeed = 100) {
  resetMotorsAndSensors();

  double error, prevError = 0;
  double integral = 0;
  double derivative = 0;
  double speed;

  while (true) {
    // Calculate error (assumes a gyro sensor is used for heading)
    double currentAngle = InertialSensor.heading(degrees);
    error = targetAngle - currentAngle;

    // Integral and derivative
    integral += error;
    derivative = error - prevError;

    // Calculate motor speed
    speed = (error * kP_turn) + (integral * kI_turn) + (derivative * kD_turn);

    // Limit motor speed
    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < -maxSpeed) speed = -maxSpeed;

    // Set motor speeds for turning
    frontLeft.spin(forward, speed, pct);
    frontRight.spin(reverse, speed, pct);
    backLeft.spin(forward, speed, pct);
    backRight.spin(reverse, speed, pct);

    // Exit condition
    if (fabs(error) < 2) break;

    prevError = error;
    wait(20, msec);
  }

  // Stop motors
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

// Main function
int main() {
  // Initialize robot configuration
  vexcodeInit();

  // Example usage
  PIDDrive(500);  // Drive forward 500 degrees
  PIDTurn(90);    // Turn 90 degrees
}

