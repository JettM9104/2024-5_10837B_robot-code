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
motor left = motor(PORT1, true);
motor right = motor(PORT2, false);


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


void init();


void drive(double distance, double timeout); // Distance in Inches, Timeout in Seconds
void turn(double angle, double timeout); // Angle in Degrees, Timeout in Seconds


// Namespaces for organization of PID Coefficients
namespace pid
{
    namespace drive { double kP, double kI, double kD; }
    namespace turn { double kP, double kI, double kD; }
    namespace correction { double kP, double kI, double kD; }
}

double& dkP = pid::drive::kP, dkI = pid::drive::kI, dkD = pid::drive::kD;
double& tkP = pid::turn::kP, tkI = pid::turn::kI, tkD = pid::turn::kD;
double& ckP = pid::correction::kP, ckI = pid::correction::kI, ckD = pid::correction::kD;


int main() {
  // run stuff here
  vexcodeInit();
  init();
}


void init() {
  // initalize stuff here
}

void drive(double distance, double timeout) { // Drive Function
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
  left.resetPosition();
  right.resetPosition();

  while (true) {
    // Calculate PID Values
    error = goalDegrees - left.position(degrees)
    integral = error < integralResetZone ? 0 : integral + error;
    derivative = error - lastError;

    if (fabs(error) < 3) { integral = 0; } // Reset integral when target is almost met

    // Correction Calculation
    correctionError = (right.position(degrees) - left.position(degrees)) + (BrainInertial.rotation(degrees) - beginInertial);
    correctionIntegral += correctionError;
    correctionDerivative = correctionError - correctionLastError;

    // Calculate Correction Factor
    correctionFactor = (correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD);
    
    // Calculate Motor Speed
    leftSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkP)) - correctionFactor;
    rightSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkP)) + correctionFactor;

    // Spin Motors
    left.spin(forward, leftSpeed, percent);
    right.spin(forward, rightSpeed, percent);

    // Exit Conditions
    if (fabs(error) < threshold) { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) { break; }
  }
}