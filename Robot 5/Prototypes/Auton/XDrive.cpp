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
motor frontLeft = motor(PORT1, true);
motor frontRight = motor(PORT2, false);
motor backRight = motor(PORT7, true);
motor backLeft = motor(PORT8, false);
motor shooting1 = motor(PORT4, false);
motor shooting2 = motor(PORT10, true);
pneumatic cats = pneumatic(PORT5);
pneumatic dogs = pneumatic(PORT11);
gyro turning = gyro(PORT3);
distance conveyerSensor = distance(PORT9);
distance closerSensor = distance(PORT6);
touchled indicator = touchled(PORT12);

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

//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       {author}                                                  
//    Created:      {date}                                                    
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------

// Global PID Coefficients
double kP_drive = 2, kI_drive = 0.01, kD_drive = 0.3;    // For forward/backward
double kP_strafe = 0.2, kI_strafe = 0.01, kD_strafe = 0.1; // For horizontal movement
double kP_angle_strafe = 1.7, kI_angle_strafe = 0.01, kD_angle_strafe = 1;    // For angular correction
double kP_angle_drive = 4.82, kI_angle_drive = 0.018, kD_angle_drive = 0.6;
double kP_turn = 0.5, kI_turn = 0.012, kD_turn = 1.2;       // For precise turning

// References to access PID Coefficients more easi;y
double& kpD = kP_drive, kiD = kI_drive, kdD = kD_drive;
double& kpS = kP_strafe, kiS = kI_strafe, kdS = kD_strafe;
double& kpAS = kP_angle_strafe, kiAS = kI_angle_strafe, kdAS = kD_angle_strafe;
double& kpAD = kP_angle_drive, kiAD = kP_angle_drive, kdAD = kD_angle_drive;
double& kpT = kP_turn, kiT = kI_turn, kdT = kD_turn;

// Empty global variable (will be changed in pid() function)
double kP_angle, kI_angle, kD_angle;

// Variables
const double pi = 3.141592;
unsigned short int puncher = 0;

// Motor Functions
void resetAll(); // Resets all the Encoder Positions
double vertEC(); // Returns the average of the Vertical Encoder Positions
double horzEC(); // Returns the average of the Horizontal Encoder Positions
double turnEC();

// Drive Functions
void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed = 90); // Strafe and Drive into one function
void pidTurn(double targetAngle, double timeout, double maxSpeed = 50);

// Main Functions
void init();


int main() {
  // Initialize Robot Configuration
  vexcodeInit();
  init();
}


void init() { // Runs in the beginning of the code
  //initalization code
}

void resetAll() { // Resets all encoder positions
  frontLeft.resetPosition();
  frontRight.resetPosition();
  backLeft.resetPosition();
  backRight.resetPosition();
  BrainInertial.setRotation(0, degrees);
  BrainInertial.setHeading(0, degrees);

}

double vertEC() { return (frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) + backRight.position(degrees)) / 4.0 * 3.0; }

double horzEC() { return (-frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) - backRight.position(degrees)) / 4.0 * 3.0; }

double turnEC() { return (frontLeft.position(degrees) - frontRight.position(degrees) - backLeft.position(degrees) + backRight.position(degrees)) / 4.0; }

void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed) {
  resetAll();
  unsigned int tick = 0;
  int direction;

  double kP_drive_local = kP_drive;
  double kI_drive_local = kI_drive;
  double kD_drive_local = kD_drive;

  double kP_strafe_local = kP_strafe; 
  double kI_strafe_local = kI_strafe;
  double kD_strafe_local = kD_strafe;

  double movementAngle = atan2(targetHorizontal, targetVertical);
  
  kP_angle = cos(movementAngle) * kP_angle_drive + sin(movementAngle) * kP_angle_strafe;
  kI_angle = cos(movementAngle) * kI_angle_drive + sin(movementAngle) * kI_angle_strafe;
  kD_angle = cos(movementAngle) * kD_angle_drive + sin(movementAngle) * kD_angle_strafe;


  double verticalError, prevVerticalError = 0;
  double verticalIntegral = 0, verticalDerivative = 0;

  double horizontalError, prevHorizontalError = 0;
  double horizontalIntegral = 0, horizontalDerivative = 0;

  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;
  
  float bT = Brain.Timer.value();

  bool drive_completed = false;
  bool strafe_completed = false;
  
  while (true) {
    
    tick++;

    double currentVertical = vertEC();
    double currentHorizontal = horzEC();
    verticalError = targetVertical - currentVertical;
    horizontalError = targetHorizontal - currentHorizontal;

    verticalIntegral += verticalError;
    verticalDerivative = verticalError - prevVerticalError;

    horizontalIntegral += horizontalError;
    horizontalDerivative = horizontalError - prevHorizontalError;

    angleError = 0 - BrainInertial.rotation(degrees);

    angleIntegral += angleError;
    angleDerivative = angleError - prevAngleError;

    if (fabs(verticalError) <= 1) verticalIntegral = 0;
    if (fabs(horizontalError) <= 1) horizontalError = 0;
    if (fabs(angleError) <= 1) angleIntegral = 0;
    
    double verticalSpeed = (verticalError * kP_drive_local) + (verticalIntegral * kI_drive_local) +
                           (verticalDerivative * kD_drive_local);
    double horizontalSpeed = ((horizontalError * kP_strafe_local) +
                             (horizontalIntegral * kI_strafe_local) +
                             (horizontalDerivative * kD_strafe_local)) * -1;
    double correctionSpeed = (angleError * kP_angle) +
                             (angleIntegral * kI_angle) +
                             (angleDerivative * kD_angle);

    if (verticalSpeed > maxSpeed) verticalSpeed = maxSpeed;
    if (verticalSpeed < -maxSpeed) verticalSpeed = -maxSpeed;

    if (horizontalSpeed > maxSpeed) horizontalSpeed = maxSpeed;
    if (horizontalSpeed < -maxSpeed) horizontalSpeed = -maxSpeed;

    frontLeft.spin(forward, verticalSpeed + horizontalSpeed + correctionSpeed, pct);
    backLeft.spin(forward, verticalSpeed + horizontalSpeed - correctionSpeed, pct);
    frontRight.spin(forward, verticalSpeed - horizontalSpeed - correctionSpeed, pct);
    backRight.spin(forward, verticalSpeed - horizontalSpeed + correctionSpeed, pct);

    if (fabs(verticalError) < 20 && fabs(horizontalError) < 20 && fabs(angleError) < 3) { printf("break by threshold\n"); break; }

    if (drive_completed && direction == 0) { printf("break by drive\n"); break; }

    if (strafe_completed && direction == 1) { printf("break by strafe\n"); break; }

    if (tick > 20) {
      if (fabs(verticalError) < 20) { kP_drive_local = 0, kI_drive_local = 0, kD_drive_local = 0; drive_completed = true; }
      if (fabs(horizontalError) < 20) { kP_strafe_local = 0, kI_strafe_local = 0, kD_strafe_local = 0; strafe_completed = true; }
    }
    if (((Brain.Timer.value() - bT) > timeout) && (timeout != 0)) { printf("break by timeout\n"); break; }

    prevVerticalError = verticalError;
    prevHorizontalError = horizontalError;
    prevAngleError = angleError;

    wait(20, msec);
  }
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}


void pidTurn(double targetAngle, double timeout, double maxSpeed) {
  resetAll();

  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;
  double maxIntegral = 100; 
  double bT = Brain.Timer.value();
  double targetEncoder = pi * 10 * targetAngle / 7.874 / 3;

  while (true) {
    angleError = targetEncoder - turnEC();
    printf("Angle Error: %f\n", angleError);
    printf("\033[34m");
    printf("%f\n", BrainInertial.rotation(degrees));

    printf("\033[30m");
    angleIntegral += angleError;
    
    if (angleIntegral > maxIntegral) angleIntegral = maxIntegral;
    if (angleIntegral < -maxIntegral) angleIntegral = -maxIntegral;
    
    printf("Angle Integral: %f\n", angleIntegral);

    angleDerivative = angleError - prevAngleError;
    printf("Angle Derivative: %f\n", angleDerivative);

    double turnSpeed = (angleError * kP_turn) + (angleIntegral * kI_turn) +
                       (angleDerivative * kD_turn);
    printf("Turn Speed (before limiting): %f\n", turnSpeed);

    if (turnSpeed > maxSpeed) turnSpeed = maxSpeed;
    if (turnSpeed < -maxSpeed) turnSpeed = -maxSpeed;
    printf("Turn Speed (after limiting): %f\n", turnSpeed);
  
    frontLeft.spin(forward, turnSpeed, pct);
    backLeft.spin(reverse, turnSpeed, pct);
    frontRight.spin(reverse, turnSpeed, pct);
    backRight.spin(forward, turnSpeed, pct);

    if (fabs(angleError) < 2) angleIntegral = 0;
    printf("Angle Integral (after reset if error < 2): %f\n", angleIntegral);

    if (fabs(angleError) < 5) break;
    if ((Brain.Timer.value() - bT) > timeout && timeout != 0) break;

    prevAngleError = angleError;
    printf("Previous Angle Error: %f\n\n\n", prevAngleError);

    wait(20, msec);
  }
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}
