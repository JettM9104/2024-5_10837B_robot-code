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
motor frontLeft = motor(PORT1, 3/1, false);
motor frontRight = motor(PORT2, 3/1, true);
motor backLeft = motor(PORT8, 3/1, false);
motor backRight = motor(PORT7, 3/1, true);


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

// PID Constants
double kP_drive = 0.2;
double kI_drive = 0.01;
double kD_drive = 0.1;

double kP_turn = 0.3;
double kI_turn = 0.02;
double kD_turn = 0.1;

// Function for resetting motors and sensors
void resetMotorsAndSensors() {
  frontLeft.resetPosition();
  frontRight.resetPosition();
  backLeft.resetPosition();
  backRight.resetPosition();
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
    frontLeft.spin(forward, speed, percent);
    frontRight.spin(reverse, speed, percent);
    backLeft.spin(forward, speed, percent);
    backRight.spin(reverse, speed, percent);

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

