#include "vex.h"


using namespace vex;

inertial BrainInertial = inertial();
controller Controller = controller();
motor leftDrive = motor(PORT2, true);
motor rightDrive = motor(PORT4, false);
motor pdgsLeft = motor(PORT3, false);
motor pdgsRight = motor(PORT6, true);
motor metroLeft = motor(PORT1, true);
motor metroRight = motor(PORT5, false);
pneumatic pneum1 = pneumatic(PORT8);
pneumatic pneum2 = pneumatic(PORT9);
touchled indicator = touchled(PORT7);

brain Brain;

void initializeRandomSeed(){
  wait(100, msec);
  int seed = int(BrainInertial.acceleration(xaxis) * 1000 +
                 BrainInertial.acceleration(yaxis) * 1000 +
                 BrainInertial.acceleration(zaxis) * 1000);
  srand(seed);
}

void vexcodeInit() {
  initializeRandomSeed(); 
}

// PID constants for drive and turn
namespace pid {
  namespace drive { float kP = 0.4, kI = 0.2, kD = 0.3; }
  namespace turn { float kP = 0.65, kI = 0.2, kD = 0.1; }
}

float& dkP = pid::drive::kP, dkI = pid::drive::kI, dkD = pid::drive::kD;
float& tkP = pid::turn::kP, tkI = pid::turn::kI, tkD = pid::turn::kD;

double convertEncoderToDist(double gearRatio, bool reversed = false, double wheelCirc = 200);
double convertDistToEncoder(double gearRatio, bool reversed = false, double wheelCirc = 200);
void drive(double distance, double timeout = 0);
void turn(double angle, double timeout = 0);

int main() {
  vexcodeInit();  // Initialize Robot Configuration
  
  drive(500);

  // pdgsLeft.spin(forward);
  // pdgsRight.spin(forward);
}

double convertEncoderToDist(double gearRatio, bool reversed, double wheelCirc) {
  return gearRatio * (wheelCirc / 360) * (reversed ? -1 : 1);
}

double convertDistToEncoder(double gearRatio, bool reversed, double wheelCirc) {
  return (1 / gearRatio) * (360 / wheelCirc) * (reversed ? -1 : 1);
}

void drive(double distance, double timeout) {
  double error, integral = 0, derivative = 0, lastError = 0;
  leftDrive.resetPosition();
  rightDrive.resetPosition();
  double beginTimer = Brain.Timer.value();

  printf("%f", (convertDistToEncoder(5/2) * distance));
  error = ((convertDistToEncoder(5/2) * distance) - (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2);

  while (true) {
    error = ((convertDistToEncoder(5/2) * distance) - (leftDrive.position(degrees) + rightDrive.position(degrees)) / 2);
    printf("error = %f;\nintegral = %f;\nderivative = %f\n\n", error, integral, derivative);
    integral = error < 3 ? 0 : integral + error;
    derivative = error - lastError;

    double motorSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkD));

    leftDrive.spin(forward, motorSpeed, percent);
    rightDrive.spin(forward, motorSpeed, percent);

    lastError = error;

    if (fabs(error) < 5 || (timeout != 0 && Brain.Timer.value() - beginTimer > timeout)) {
      break;
    }

    wait(20, msec);
  }
  printf("break\n");
  leftDrive.stop();
  rightDrive.stop();
}

void turn(double angle, double timeout) {
  double error, integral = 0, derivative, lastError = 0;
  leftDrive.resetPosition();
  rightDrive.resetPosition();
  BrainInertial.setRotation(0, degrees);
  double beginTimer = Brain.Timer.value();

  while (true) {
    double angleEncoders = ((leftDrive.position(degrees) + rightDrive.position(degrees)) / 2) * 200 * (60/24) / 250;
    error = (BrainInertial.rotation(degrees) + angleEncoders) / 2 - angle;
    integral = error < 3 ? 0 : integral + error;
    derivative = error - lastError;

    double leftMS = (error * tkP) + (integral * tkI) + (derivative * tkD);
    double rightMS = leftMS;

    leftDrive.spin(forward, leftMS, velocityUnits::pct);
    rightDrive.spin(forward, rightMS, velocityUnits::pct);

    lastError = error;

    if (fabs(error) < 5 || (timeout != 0 && Brain.Timer.value() - beginTimer > timeout)) {
      break;
    }
  }
}
