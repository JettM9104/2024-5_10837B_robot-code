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
inertial BrainInertial = inertial();
motor leftDrive = motor(PORT3, false);
motor rightDrive = motor(PORT9, true);
distance sigma = distance(PORT5);
distance alpha = distance(PORT1);

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


void init();

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int lmaxSpeed = 100, const unsigned short int rmaxSpeed = 100);
void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);


int main() {
  vexcodeInit();
  init();


  BrainInertial.calibrate();
  // Print that the Inertial Sensor is calibrating while
  // waiting for it to finish calibrating.
  while(BrainInertial.isCalibrating()){
      Brain.Screen.clearScreen();
      Brain.Screen.print("Inertial Calibrating");
      wait(50, msec);
  }

  BrainInertial.setRotation(0, degrees); // change to zero after


  // drive out

  drive(530, 1, 0.01, 0.1, 100, 100);

  float derror;
  float dintegral = 0;
  float dderivative =0;
  float dlastError = 0;

  float values[5] = {0, 0, 0, 0, 0};

  for (int i = 0; i < 5; i++) {
    values[i] = sigma.objectDistance(inches);
  }
  float averages = sigma.objectDistance(inches);

  // pid w/ distance sensor
  while (!(averages >= 24.5 && averages <= 25.5)) {
    
    for (int i = 0; i < 5; i++) {
      averages += values[i];
    }
    averages /= 5;

    derror = 25 - averages;
    dintegral += derror;
    dderivative = derror - dlastError;
    printf("dist %f\n", averages);

    leftDrive.spin(reverse, (derror * 0.9 + dintegral * 0.06 + dderivative * 0.7), percent);
    rightDrive.spin(reverse, (derror * 0.9 + dintegral * 0.06 + dderivative * 0.7), percent);

    wait(120, msec);
    dlastError = derror;

    for (int i = 0; i < 4; i++) {
      values[i] = values[i+1];
    }

    values[4] = sigma.objectDistance(inches);
  }  

  leftDrive.stop();
  rightDrive.stop();
  wait(500, msec);

  // turn to heading 90

  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;

  Brain.Timer.reset();
  while (!(BrainInertial.rotation(degrees) >= 89 && BrainInertial.rotation(degrees) <= 91)) {
    derror = 90 - BrainInertial.rotation(degrees);
    dintegral = fabs(derror) < 5 ? 200 : dintegral + derror;
    dderivative = derror - dlastError;
    printf("derror %f\nintegral %f\nderivative %f\n\n\n", derror, dintegral, dderivative);

    leftDrive.spin(reverse, (derror * 0.4 + dintegral * 0.0025 + dderivative * 1.4), percent);
    rightDrive.spin(forward, (derror * 0.4 + dintegral * 0.0025 + dderivative * 1.4), percent);

    if (Brain.Timer.value() > 2) break;
    wait(100, msec);
    dlastError = derror;
  }  

  leftDrive.stop();
  rightDrive.stop();
  printf("%f\n", BrainInertial.rotation(degrees));

  wait(500, msec);

  // drive back

  drive(-100000, 1000, 1000, 0, 3, 100, 100);


  leftDrive.stop();
  rightDrive.stop();
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);

  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;
  while (!(BrainInertial.rotation(degrees) >= 89 && BrainInertial.rotation(degrees) <= 91)) {
    derror = 90 - BrainInertial.rotation(degrees);
    dintegral += derror;
    dderivative = derror - dlastError;
    printf("derror %f\nintegral %f\nderivative %f\n\n\n", derror, dintegral, dderivative);

    leftDrive.spin(reverse, (derror * 0.9 + dintegral * 0.1 + dderivative * 0.1), percent);
    rightDrive.spin(forward, (derror * 0.9 + dintegral * 0.1 + dderivative * 0.1), percent);

    wait(20, msec);
    dlastError = derror;
  }  
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  wait(500, msec);
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);

  

//////



  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;
  while (!(BrainInertial.rotation(degrees) >= 89 && BrainInertial.rotation(degrees) <= 91)) {
    derror = 90 - BrainInertial.rotation(degrees);
    dintegral += derror;
    dderivative = derror - dlastError;
    printf("derror %f\nintegral %f\nderivative %f\n\n\n", derror, dintegral, dderivative);

    leftDrive.spin(reverse, (derror * 0.9 + dintegral * 0.01 + dderivative * 0.8), percent);
    rightDrive.spin(forward, (derror * 0.9 + dintegral * 0.01 + dderivative * 0.8), percent);

    wait(20, msec);
    dlastError = derror;
  }  
  leftDrive.stop();
  rightDrive.stop();
  printf("inital curve\n");
  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;
  while (!(BrainInertial.rotation(degrees) >= 118 && BrainInertial.rotation(degrees) <= 122)) {
    derror = 120 - BrainInertial.rotation(degrees);
    dintegral = fabs(derror) > 3 ? dintegral + derror : 0;
    dderivative = derror - dlastError;

    rightDrive.spin(forward, (derror * 0.5 + dintegral * 0.005 + dderivative * 0.9), percent);

    wait(20, msec);
    dlastError = derror;
  }  
  leftDrive.stop();
  rightDrive.stop();
  leftDrive.setStopping(hold);
  rightDrive.setStopping(hold);

  printf("inital drive back\n");

  drive(400, 1, 0.01, 0.1, 2, 100, 100);

  printf("adjist to be close to wall\n");
  while (!((-alpha.objectDistance(inches) * sin(-BrainInertial.rotation(degrees) * M_PI / 180)) >= 20 && (-alpha.objectDistance(inches) * sin(-BrainInertial.rotation(degrees) * M_PI / 180)) <= 22)) {
    derror = 21 - (-alpha.objectDistance(inches) * sin(-BrainInertial.rotation(degrees) * M_PI / 180));
    dintegral = fabs(derror) > 3 ? dintegral + derror : 0;
    dderivative = derror - dlastError;

    leftDrive.spin(reverse, (derror * 0.5 + dintegral * 0.05 + dderivative * 0.9), percent);
    rightDrive.spin(reverse, (derror * 0.5 + dintegral * 0.05 + dderivative * 0.9), percent);

    wait(20, msec);
    dlastError = derror;
  } 
  leftDrive.stop();
  rightDrive.stop();
  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;

  printf("point to heading 90\n");
  do {
    derror = 90 - BrainInertial.rotation(degrees);
    dintegral = fabs(derror) < 3 ? 0 : dintegral + derror;

    printf("error %f\nintegral %f\n\n", derror, dintegral);

    leftDrive.spin(reverse, (derror * 0.4 + dintegral * 0.007), percent);
    rightDrive.spin(forward, (derror * 0.4 + dintegral * 0.007), percent);

    wait(20, msec);
    dlastError = derror;
  } while (fabs(derror) > 1);
  leftDrive.stop();
  rightDrive.stop();
  printf("full speed\n");
  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;

  Brain.Timer.reset();

  leftDrive.stop();
  rightDrive.stop();


  wait(1000, msec);

  
  drive(-100000, 1000, 1000, 0, 2, 100, 100);
  
  // drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  // drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  // wait(500, msec);
  // drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  // drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);
  // wait(500, msec);
  // wait(500, msec);

  

  // wait(1000, msec);

  // while (true) {
  //   drive(100, 1, 0.01, 0.5, 0.4, 100, 100);
  //   drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);

  // }

}

void init() {


  leftDrive.setMaxTorque(100, percent);
  leftDrive.setVelocity(100, percent);
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}


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

    motorSpeed = (error * kp) + (integral * ki) + (derivative * kd);

    float leftMotorSpeed = motorSpeed, rightMotorSpeed = motorSpeed;

    if (leftMotorSpeed > lmaxSpeed) leftMotorSpeed = lmaxSpeed;
    if (leftMotorSpeed < -lmaxSpeed) leftMotorSpeed = -lmaxSpeed;

    if (rightMotorSpeed > rmaxSpeed) rightMotorSpeed = rmaxSpeed;
    if (rightMotorSpeed < -rmaxSpeed) rightMotorSpeed = -rmaxSpeed;
    
    leftDrive.spin(forward, leftMotorSpeed - (BrainInertial.rotation(degrees) - beginInertial), percent);
    rightDrive.spin(forward, rightMotorSpeed + (BrainInertial.rotation(degrees) - beginInertial), percent);

    if (fabs(error) < 5) { printf("break by threshold\n"); break; }
    if (timeout != 0 && (Brain.Timer.value() - beginTimer) > timeout) { printf("break by timeout"); break; }
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
    error = rawTheta - ((leftDrive.position(degrees) - rightDrive.position(degrees)) / 2);
    integral = integral <= 3 ? 0 : error + integral;
    derivative = error - lastError;
    printf("error       %f\nintegral    %f\nderivative%f\n\n\n", error, integral, derivative);

    motorSpeed = (error * kp) + (integral * ki) + (derivative * kd);
    
    leftDrive.spin(forward, motorSpeed, percent);
    rightDrive.spin(reverse, motorSpeed, percent);

    if (fabs(error) < 5) break;
    if (timeout != 0 && (Brain.Timer.value() - beginTimer) > timeout) break;

    printf("%f\n\n\n", (Brain.Timer.value() - beginTimer));
    lastError = error;
    wait(20, msec);
  }
  leftDrive.stop();
  rightDrive.stop();
}

