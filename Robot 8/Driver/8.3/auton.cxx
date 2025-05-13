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
motor leftDrive = motor(PORT3);
motor rightDrive = motor(PORT9, true);

motor frontRightMetro = motor(PORT5);
motor backRightMetro = motor(PORT6);
motor intake = motor(PORT11);
motor leftMetro = motor(PORT12);

touchled indicator = touchled(PORT1);

distance rpLoad = distance(PORT4);

distance intakeSensor = distance(PORT2);
distance backrollerSensor = distance(PORT10);

bumper catapultSensor = bumper(PORT7);


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


using namespace vex;


void init();

void drive(const float distance, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int lmaxSpeed = 100, const unsigned short int rmaxSpeed = 100);
void turn(const float rawTheta, const float kp, const float ki, const float kd, const float timeout = 0, const unsigned short int maxSpeed = 100);

void spinIntake();
void spinUntilDetect();
void windCata();


int main() {
  vexcodeInit();
  init();
    while (!indicator.pressing()) wait(20, msec);

    BrainInertial.calibrate();
  // Print that the Inertial Sensor is calibrating while
  // waiting for it to finish calibrating.
  while(BrainInertial.isCalibrating()){

    indicator.setColor(white);

    
      Brain.Screen.clearScreen();
      Brain.Screen.print("Inertial Calibrating");
      wait(50, msec);
  }
  


  if (Brain.Battery.capacity(percent) > 95) indicator.setColor(green);
  else if (Brain.Battery.capacity(percent) > 90) indicator.setColor(yellow);
  else if (Brain.Battery.capacity(percent) > 85) indicator.setColor(orange);
  else { indicator.setColor(red); }

  
  while (!indicator.pressing()) wait(20, msec);



  while (!catapultSensor.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();

  while (!indicator.pressing()) wait(20, msec);

  thread spud = thread(spinUntilDetect);

  BrainInertial.setRotation(0, degrees); // change to zero after

  drive(500, 1, 0.01, 1.5, 3, 100, 100);
  //leftDrive.spinFor(forward, 630, degrees, false);
  //rightDrive.spinFor(forward, 630, degrees, true);

  leftDrive.setStopping(brake);
  rightDrive.setStopping(brake);

  wait(500, msec);

  

  // turn to heading 90

  leftDrive.spinFor(reverse, 150, degrees, false);
  rightDrive.spinFor(forward, 150, degrees, false);
  wait(3, seconds);
  leftDrive.stop();
  rightDrive.stop();

  printf("%f\n", BrainInertial.rotation(degrees));

  wait(500, msec);

  // drive back

  drive(-100000, 1000, 1000, 0, 3, 100, 100);


  leftDrive.stop();
  rightDrive.stop();
  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);

  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);

  wait(100, msec);


  intake.stop();
  backRightMetro.stop();
  leftMetro.stop();
  frontRightMetro.stop();

  Brain.Timer.reset();
  while (Brain.Timer.value() < 0.8) {
    leftMetro.spin(forward, 100, percent); 
    wait(20, msec);
  }

  intake.spin(reverse, 100, percent);
  backRightMetro.spin(forward, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  frontRightMetro.spin(reverse, 100, percent);

  while (backrollerSensor.objectDistance(mm) > 60) wait(20, msec);

  while (backrollerSensor.objectDistance(mm) < 60) wait(20, msec);

  
  wait(700, msec);

  intake.stop();
  backRightMetro.stop();
  leftMetro.stop();
  frontRightMetro.stop();

  ///// wind cata


  thread ohio = thread(windCata);


  printf("inital curve\n");
  drive(160, 1, 0.01, 0.5, 0.8, 100, 100);

  wait(1000, msec);
  

  intake.spin(reverse, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  frontRightMetro.spin(reverse, 100, percent);



  float derror = 0;
  float dintegral = 0;
  float dderivative =0;
  float dlastError = 0;
  
  Brain.Timer.reset();
  while (!(BrainInertial.rotation(degrees) >= 132 && BrainInertial.rotation(degrees) <= 138)) {
    derror = 135 - BrainInertial.rotation(degrees);
    dintegral = fabs(derror) < 5 ? 200 : dintegral + derror;
    dderivative = derror - dlastError;
    printf("derror %f\nintegral %f\nderivative %f\n\n\n", derror, dintegral, dderivative);

    leftDrive.spin(reverse, (derror * 0.75 + dintegral * 0.035 + dderivative * 2), percent);
    rightDrive.spin(forward, (derror * 0.75 + dintegral * 0.035 + dderivative * 2), percent);

    if (Brain.Timer.value() > 3) break;
    wait(200, msec);
    dlastError = derror;
  }


  leftDrive.setStopping(hold);
  rightDrive.setStopping(hold);

  printf("inital drive back\n");
  


  drive(530, 1, 0.01, 0.1, 4, 100, 100);

  printf("adjist to be close to wall\n");

  derror = 0;
  dintegral = 0;
  dderivative =0;
  dlastError = 0;
  



  Brain.Timer.reset();
  while (!(BrainInertial.rotation(degrees) >= 87 && BrainInertial.rotation(degrees) <= 93)) {
    derror = 90 - BrainInertial.rotation(degrees);
    dintegral = fabs(derror) < 3 ? 200 : dintegral + derror;
    dderivative = derror - dlastError;
    printf("inertial reading %f\n", BrainInertial.rotation(degrees));
    float motorSpeed = (derror * 0.42 + dintegral * 0.015 + dderivative * 3);

    if (Brain.Timer.value() > 2.3) break; // :) :()


    leftDrive.spin(reverse, motorSpeed, percent);
    rightDrive.spin(forward, motorSpeed, percent);

    wait(333, msec);
    dlastError = derror;
  }

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

  wait(200, msec);


  
  drive(-100000, 1000, 1000, 0, 2, 100, 100);


  drive(100, 1, 0.01, 0.5, 0.4, 100, 100);

  drive(-200, 1, 0.01, 0.5, 0.8, 100, 100);


  Brain.Timer.reset();


  wait(1700, msec);

  Brain.Timer.reset();
  while (Brain.Timer.value() < 0.8) {
    leftMetro.spin(forward, 100, percent); 
    wait(20, msec);
  }



  intake.spin(reverse, 100, percent);
  backRightMetro.spin(forward, 100, percent);
  leftMetro.spin(reverse, 100, percent);
  frontRightMetro.spin(reverse, 100, percent);
  while (true) {
    if ((rpLoad.objectDistance(inches) > 28 && rpLoad.objectDistance(inches) < 33) && backrollerSensor.objectDistance(mm) > 60) {
      leftDrive.spin(forward);
      rightDrive.spin(forward);

      wait(350, msec);

      leftDrive.spin(reverse);
      rightDrive.spin(reverse);
      wait(550, msec);
      leftDrive.stop();
      rightDrive.stop();
    }
    wait(20, msec);
  }

  


}

void init() {


  leftDrive.setMaxTorque(100, percent);
  leftDrive.setVelocity(100, percent);
  rightDrive.setMaxTorque(100, percent);
  rightDrive.setVelocity(100, percent);
}

void spinUntilDetect() {
  intake.spin(reverse, 70, percent);
  backRightMetro.spin(forward, 70, percent);
  leftMetro.spin(reverse, 70, percent);
  frontRightMetro.spin(reverse, 70, percent);

  while (intakeSensor.objectDistance(mm) > 60) wait(20, msec);

  intake.stop();
  backRightMetro.stop();
  leftMetro.stop();
  frontRightMetro.stop();
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

void windCata() {
  while (!catapultSensor.pressing()) {
    backRightMetro.spin(reverse, 100, percent); 
    wait(20, msec);
  }
  backRightMetro.stop();
  backRightMetro.spin(forward, 100, percent); 
}
