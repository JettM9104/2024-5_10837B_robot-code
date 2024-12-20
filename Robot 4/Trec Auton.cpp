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

// Macros
void windPuncher();
void shootPuncher();
void squeezeBall();
void ballSwing();

// Main Functions
void init();


int main() {
  // Initialize Robot Configuration
  vexcodeInit();
  init();

  windPuncher();

  for (int i = 5; i > 0; --i) {
    wait(1000, msec);
    printf("%d\n", i);
  }

  Brain.Timer.reset();

  dogs.retract(cylinder2);
  cats.retract(cylinder2);

  pid(5000, 0, 3);

  dogs.extend(cylinder2);
  cats.extend(cylinder2);
  pid(-100, 0, 1);

  pid(0, -1000, 1.3);

  pid(1000, 0, 1.3);
  thread wind0 = thread(windPuncher);
  wait(300, msec);
  pid(-500, 0, 1);
  pid(0, 10000, 2);
  pid(800, 0, 2);

  thread wind1 = thread(windPuncher);

  pid(-2000, 0, 2);

  ballSwing();
  pid(1900, 0, 1.9);

  thread wind2 = thread(windPuncher);

  while (true) {
    pid(-2200, 0, 2);
    pid(0, 4000, 1);
    do {
      if (puncher == 2) {
        shooting1.spin(reverse);
        shooting2.spin(reverse);
      }
      if (puncher == 3) {
        shooting1.stop();
        shooting2.stop();
      }
      wait(20, msec);
    } while (puncher != 3)

    shooting1.stop();
    shooting2.stop();
    
    wait(1500, msec);

    squeezeBall();
    if (Brain.Timer.value() >= 50.0) break;

    shooting1.spin(forward);
    shooting2.spin(forward);
    wait(1000, msec);

    shooting1.stop();
    shooting2.stop();
    pid(10000, 0, 3);
    pid(0, 4000, 1);
    dogs.extend(cylinder2);
    shooting1.spin(forward);
    shooting2.spin(forward);
    wait(1, seconds);
    shooting1.stop();
    shooting2.stop();
  }
  pid(0, -4000, 2);
  pid(2300, 0, 2.2);
  shooting1.spin(forward);
  shooting2.spin(forward);





}


void init() { // Runs in the beginning of the code
  frontLeft.setStopping(hold);
  frontRight.setStopping(hold);
  backRight.setStopping(hold);
  backLeft.setStopping(hold);
  dogs.extend(cylinder2);
  cats.extend(cylinder2);
}

void windPuncher() { // Winds the puncher
  unsigned int tick = 0;
  puncher = 0;
  cats.retract(cylinder1);
  dogs.extend(cylinder1);
  wait(1000, msec);
  cats.retract(cylinder1);
  dogs.retract(cylinder1);
  

  while (true){
    puncher = 1
    tick++;
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    printf("%f\n", shooting1.current(amp));

    if (tick > 10) {
      if (shooting1.velocity(percent) < 2) {
        break;
      }
    }
    wait(20, msec);
  }
  puncher = 2;
  wait(100, msec);
  shooting1.spin(forward, 100, percent);
  shooting2.spin(forward, 100, percent);
  wait(700, msec);

  cats.extend(cylinder1);
  dogs.extend(cylinder1);

  shooting1.spin(reverse, 100, percent);
  shooting2.spin(reverse, 100, percent);

  wait(1500, msec);
  printf("END\n");
  shooting1.stop();
  shooting2.stop();
  puncher = 3;
}

void ballSwing() { // Automatically lifts the mt
  cats.retract(cylinder2);
  dogs.extend(cylinder2);

  while (true) {
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    if (conveyerSensor.objectDistance(mm) < 40) { break; }
    wait(20, msec);
  }
  shooting1.stop();
  shooting2.stop();

  dogs.retract(cylinder2);
  
  wait(2000, msec);

  dogs.extend(cylinder2);
}

void squeezeBall() {
  while (true) {
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    if (closerSensor.objectDistance(mm) < 40) { wait(100, msec); break; }
    wait(20, msec);
  }
  dogs.retract(cylinder2);
  shooting1.stop();
  shooting2.stop();
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

  
  if (targetVertical != 0 && targetHorizontal == 0) {
    kP_angle = kP_angle_drive;
    kI_angle = kI_angle_drive;
    kD_angle = kD_angle_drive;
    
    direction = 0;
  }
  else if (targetVertical == 0 && targetHorizontal != 0) {
    kP_angle = kP_angle_strafe;
    kI_angle = kI_angle_strafe;
    kD_angle = kD_angle_strafe;

    direction = 1;
  }
  else {
    kP_angle = 0;
    kI_angle = 0;
    kD_angle = 0;

    direction = 2;
  }
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

    if (drive_completed && direction == 0) { printf("break by drive"); break; }

    if (strafe_completed && direction == 1) { printf("break by strafe"); break; }

    if (tick > 20) {
      if (fabs(verticalError) < 20) { kP_drive_local = 0, kI_drive_local = 0, kD_drive_local = 0; drive_completed = true; }
      if (fabs(horizontalError) < 20) { kP_strafe_local = 0, kI_strafe_local = 0, kD_strafe_local = 0; strafe_completed = true; }
    }
    if (((Brain.Timer.value() - bT) > timeout) && (timeout != 0)) { printf("break by timeout"); break; }

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
