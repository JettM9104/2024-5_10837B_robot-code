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
controller Controller = controller();
motor leftDrivetrain = motor(PORT1, true);
motor rightDrivetrain = motor(PORT2, false);


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


// Function Declaration
void init();

// Movement Functions
void drive(double distance, double timeout = 0, directionType dir = forward); // Distance in Units Declared in function, Timeout in Seconds
void turn(double angle, double timeout = 0, directionType dir = forward); // Angle in Degrees, Timeout in Seconds


// Namespaces for organization of PID Coefficients
namespace pid
{
    namespace drive { float kP, kI, kD; }
    namespace turn { float kP, kI, kD; }
    namespace correction { float kP, kI, kD; }
}

// Refrences for ease of access of variables
float& dkP = pid::drive::kP, dkI = pid::drive::kI, dkD = pid::drive::kD;
float& tkP = pid::turn::kP, tkI = pid::turn::kI, tkD = pid::turn::kD;
float& ckP = pid::correction::kP, ckI = pid::correction::kI, ckD = pid::correction::kD;

// PIEEEEEEEEEEEE!!
const double pi = 3.1415926;

int main() {
  // run stuff here
  vexcodeInit();
  init();
  drive(20, 3);
  drive(20, 3, reverse);
  drive(20);
}


void init() {
  // initalize stuff here, for example, setstopping to hold, coast, or brake
}

void drive(double distance, double timeout, directionType dir) { // Drive Function
  // Direction Parameter
  if (dir == reverse) { distance *= 1; }

  // Coefficients for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double leftSpeed, rightSpeed;


  // Coefficients for PID Correcion System
  double correctionError, correctionIntegral = 0, correctionDerivative, correctionLastError = 0;
  double correctionFactor;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();
  double beginInertial = BrainInertial.value();

  // Wheel Distance Calculation
  double wheelCircum = 200; // The units of double "distance" will be the same as wheelCircum
  double gearRatio = 2 / 1;
  double goalDegrees =  (360 / wheelCircum) / gearRatio;

  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();
  
  while (true) {
    // Calculate PID Values
    error = goalDegrees - ((leftDrivetrain.position(degrees) + rightDrivetrain.position(degrees)) / 2);
    integral = error < integralResetZone ? 0 : integral + error;
    derivative = error - lastError;

    if (fabs(error) < 3) { integral = 0; } // Reset integral when target is almost met

    // Correction Calculation
    correctionError = (rightDrivetrain.position(degrees) - leftDrivetrain.position(degrees)) + (BrainInertial.rotation(degrees) - beginInertial);
    correctionIntegral += correctionError;
    correctionDerivative = correctionError - correctionLastError;

    // Calculate Correction Factor
    correctionFactor = (correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD);
    
    // Calculate Motor Speed
    leftSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkD)) - correctionFactor;
    rightSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkD)) + correctionFactor;

    // Spin Motors
    leftDrivetrain.spin(forward, leftSpeed, percent);
    rightDrivetrain.spin(forward, rightSpeed, percent);

    // Exit Conditions
    if (fabs(error) < threshold) { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) { break; }

    // Set lastError Values
    correctionLastError = correctionError;
    lastError = error;

    wait(20, msec); // Wait so the brain doesnt explode
  }
}

void turn(double angle, double timeout, directionType dir) {
  // Direction Parameter
  if (dir == reverse) { angle *= 1; }

  // Coefficients for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double motorSpeed;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();
  double beginInertial = BrainInertial.value();

  // Wheel Distance Calculation
  double wheelCircum = 200;
  double gearRatio = 2 / 1;
  double wheelBase = 254; // Must be same units as wheelCircum
  double goalDegrees =  (angle / 360) * pi * 10 * wheelBase / 360 * 360 / wheelCircum / gearRatio;

  
  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  while (true) {
    // Calculate PID Values
    error = ((goalDegrees - (leftDrivetrain.position(degrees) + rightDrivetrain.position(degrees)) / 2) + (angle - BrainInertial.rotation(degrees) + beginInertial)) / 2;
    integral = error < integralResetZone ? 0 : integral + error;
    derivative = error - lastError;

    if (fabs(error) < 3) { integral = 0; } // Reset integral when target is almost met

    // Calculate Motor Speed
    motorSpeed = ((error * tkP) + (integral * tkI) + (derivative * tkD));

    // Spin Motors
    leftDrivetrain.spin(forward, motorSpeed, percent);
    rightDrivetrain.spin(reverse, motorSpeed, percent);

    // Exit Conditions
    if (fabs(error) < threshold) { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) { break; }

    // Set lastError Variable
    lastError = error;

    wait(20, msec); // Wait so the brain doesnt explode
  }
}