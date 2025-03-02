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
bumper catSensor = bumper(PORT10);

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

void init();
void windCata();
void shootCata();
void updateSPTO();
void updateMPTO();
void updateCPTO();

bool macroActive = 0;
bool sPTO = 0;
bool mPTO = 0;
bool cPTO = 0;
bool blocker = 0;

const float pi = 3.141592;
int main() {
  vexcodeInit();  // Initialize Robot Configuration

  init();

  while (!indicator.pressing()) {
    if (Brain.buttonRight.pressing()) {
      while (!Brain.buttonRight.pressing()) {}
      windCata();
    }

    wait(20, msec);
  }
  drive(800);

  wait(500, msec);

  turn(-90);
  Brain.playSound(siren);

  wait(700, msec);
  drive(-100000, 3);
  wait(2250, msec);
  shootCata();

  pdgsLeft.spin(reverse, 100, percent);
  pdgsRight.spin(reverse, 100, percent);
  metroLeft.spin(reverse, 100, percent);
  metroRight.spin(reverse, 100, percent);

  drive(75, 0.5);
  wait(55, msec);
  drive(-400, 0.7);
  wait(4000, msec);
  drive(75, 0.5);
  windCata();
  pdgsLeft.spin(reverse, 100, percent);
  pdgsRight.spin(reverse, 100, percent);
  metroLeft.spin(reverse, 100, percent);
  metroRight.spin(reverse, 100, percent);
  wait(55, msec);
  drive(-400, 0.7);
  wait(3000, msec);
  pdgsLeft.stop();
  pdgsRight.stop();
  metroLeft.stop();
  metroRight.stop();
  
  thread wind = thread(windCata);

  drive(300);

  wait(600, msec);

  turn(80);
  wait(600, msec);
  drive(-400);
  wait(600, msec);
  
  turn(-90);
  wait(600, msec);
  drive(-1000, 2);
  wait(1000, msec);
  shootCata();
  pdgsLeft.spin(reverse, 100, percent);
  pdgsRight.spin(reverse, 100, percent);
  metroLeft.spin(reverse, 100, percent);
  metroRight.spin(reverse, 100, percent);

  while (true) {
    drive(75, 0.5);
    wait(55, msec);
    drive(-400, 0.7);
    wait(55, msec);
    drive(75, 0.5);
    wait(55, msec);
    drive(-400, 0.7);

    wait(4000, msec);
  }

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
    double angleEncoders = (leftDrive.position(degrees) - rightDrive.position(degrees) / 2) * 8 * pi / 81 * 4 / 3 * 1.0975;
    error = angle - angleEncoders;
    printf("angleEncoders = %f, rotation = %f;\n", angleEncoders, BrainInertial.rotation(degrees));
    integral = error < 3 ? 0 : integral + error;
    derivative = error - lastError;
    printf("error = %f;\nintegral = %f;\nderivative = %f\n\n", error, integral, derivative);
    double motorSpeed = (error * tkP) + (integral * tkI) + (derivative * tkD);

    leftDrive.spin(forward, motorSpeed, percent);
    rightDrive.spin(reverse, motorSpeed, percent);

    lastError = error;

    if (fabs(error) < 5 || (timeout != 0 && Brain.Timer.value() - beginTimer > timeout)) {
      break;
    }
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}

void updateSPTO() {
  if (!sPTO) {
    pneum2.extend(cylinder2);
    sPTO = 1;
  }
  else {
    pneum2.retract(cylinder2);
    sPTO = 0;
  }
}

void updateMPTO() {
  if (!mPTO) {
    pneum2.extend(cylinder1);
    mPTO = 1;
  }
  else {
    pneum2.retract(cylinder1);
    mPTO = 0;
  }
}

void updateCPTO() {
  if (!cPTO) {
    pneum1.extend(cylinder1);
    cPTO = 1;
  }
  else {
    pneum1.retract(cylinder1);
    cPTO = 0;
  }
}

void updateIndex() {
  if (!blocker) {
    pneum1.extend(cylinder2);
    blocker = 1;
  }
  else {
    pneum1.retract(cylinder2);
    blocker = 0;
  }
}

void windCata() {
  macroActive = true;

  if (!mPTO) updateMPTO();
  
  do {
    metroLeft.spin(forward, 100, percent);
    metroRight.spin(forward, 100, percent);
  } while (!catSensor.pressing());

  metroLeft.stop();
  metroRight.stop();

  updateMPTO();

  macroActive = false;
}

void shootCata() {
  macroActive = true;

  if (!mPTO) updateMPTO();
  metroLeft.spin(forward, 100, percent);
  metroRight.spin(forward, 100, percent);

  wait(400, msec);

  metroLeft.stop();
  metroRight.stop();

  updateMPTO();

  macroActive = false;
}

void init() {
  leftDrive.setStopping(hold);
  rightDrive.setStopping(hold);

  updateMPTO();
  updateSPTO();
  updateCPTO();

  wait(1000, msec);

  updateMPTO();
  updateSPTO();
  updateCPTO();

  wait(1000, msec);
  updateCPTO();
}


