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

double kP_drive = 0.2, kI_drive = 0.01, kD_drive = 0.3;    // For forward/backward
double kP_strafe = 0.2, kI_strafe = 0.01, kD_strafe = 0.1; // For horizontal movement
double kP_angle_strafe = 2.3, kI_angle_strafe = 0.01, kD_angle_strafe = 1;    // For angular correction
double kP_angle_drive = 4.82, kI_angle_drive = 0.018, kD_angle_drive = 0.6;
double kP_turn = 0.4, kI_turn = 0.012, kD_turn = 1;       // For precise turning

double kP_angle, kI_angle, kD_angle;


void resetAll(); // Resets all the Encoder Positions
double vertEC(); // Returns the average of the Vertical Encoder Positions
double horzEC(); // Returns the average of the Horizontal Encoder Positions
void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed = 90); // Strafe and Drive into one function
void pidTurn(double targetAngle, double timeout, double maxSpeed = 50);
void windPuncher();
void shootPuncher();
void init();


int main() {

    // Initialize Robot Configuration
  vexcodeInit();
  BrainInertial.setRotation(0, degrees);
  BrainInertial.setHeading(0, degrees);


  init();

  pid(-600, 0, 0);

  // wait(2000, msec); // Wait for calibration to complete
  // for (int i = 0; i < 4; i++) {
  //   pidTurn(90, 0);
  //   wait(2, seconds);
  // }
  // pidTurn(-90, 0)
}


void init() {
  frontLeft.setStopping(hold);
  frontRight.setStopping(hold);
  backRight.setStopping(hold);
  backLeft.setStopping(hold);
}
void windPuncher() {
  unsigned int tick = 0;
  cats.retract(cylinder1);
  dogs.extend(cylinder1);
  wait(2000, msec);
  cats.retract(cylinder1);
  dogs.retract(cylinder1);
  

  while (true){
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
  wait(100, msec);
  shooting1.spin(forward, 100, percent);
  shooting2.spin(forward, 100, percent);
  wait(700, msec);

  cats.extend(cylinder1);
  dogs.extend(cylinder1);

  shooting1.spin(reverse, 100, percent);
  shooting2.spin(reverse, 100, percent);

  wait(200, msec);
  printf("END%d\n", tick);
  shooting1.stop();
  shooting2.stop();
}

void resetAll() {
  frontLeft.resetPosition();
  frontRight.resetPosition();
  backLeft.resetPosition();
  backRight.resetPosition();
  BrainInertial.resetRotation();
  BrainInertial.resetHeading();
}

double vertEC() { return (frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) + backRight.position(degrees)) / 4.0 * 3.0; }

double horzEC() { return (-frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) - backRight.position(degrees)) / 4.0 * 3.0; }

void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed) {
  resetAll();
  unsigned int tick = 0;
  int direction;

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
    
    double verticalSpeed = (verticalError * kP_drive) + (verticalIntegral * kI_drive) +
                           (verticalDerivative * kD_drive);
    double horizontalSpeed = ((horizontalError * kP_strafe) +
                             (horizontalIntegral * kI_strafe) +
                             (horizontalDerivative * kD_strafe)) * -1;
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

    if (fabs(verticalError) < 20 && fabs(horizontalError) < 20 && fabs(angleError) < 3) break;

    if (drive_completed && direction == 0) { break; }

    if (strafe_completed && direction == 1) { break; }

    if (tick > 20) {
      if (fabs(verticalError) < 20) { kP_drive = 0, kI_drive = 0, kD_drive = 0; drive_completed = true; }
      if (fabs(horizontalError) < 20) { kP_strafe = 0, kI_strafe = 0, kD_strafe = 0; strafe_completed = true; }
    }
    if ((Brain.Timer.value() - bT) > timeout && timeout != 0) break;

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

  double bT = Brain.Timer.value();

  while (true) {
    angleError = targetAngle - BrainInertial.rotation(degrees);

    printf("%f\n", angleError);

    angleIntegral += angleError;
    angleDerivative = angleError - prevAngleError;

    double turnSpeed = (angleError * kP_turn) + (angleIntegral * kI_turn) +
                       (angleDerivative * kD_turn);

    if (turnSpeed > maxSpeed) turnSpeed = maxSpeed;
    if (turnSpeed < -maxSpeed) turnSpeed = -maxSpeed;

  
    frontLeft.spin(forward, turnSpeed, pct);
    backLeft.spin(reverse, turnSpeed, pct);
    frontRight.spin(reverse, turnSpeed, pct);
    backRight.spin(forward, turnSpeed, pct);

    if (fabs(angleError) < 2) angleIntegral = 0;
    if (fabs(angleError) < 5) break;
    if ((Brain.Timer.value() - bT) > timeout && timeout != 0) break;

    prevAngleError = angleError;

    wait(20, msec);
  }
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

