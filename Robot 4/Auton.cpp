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
motor ApositiveU = motor(PORT1, true);
motor BpositiveR = motor(PORT6, false);
motor AnegativeD = motor(PORT7, true);
motor bNegativeL = motor(PORT12, false);

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


// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

//----------------------------------------------------------------------------
//                                                                            
//    Module:       main.cpp                                                  
//    Author:       {author}                                                  
//    Created:      {date}                                                    
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------

// Include the IQ Library
#include "iq_cpp.h"

// Allows for easier use of the VEX Library
using namespace vex;
void drive(double dir, double dist, double k, double kp, double ki, double kd, double timeout);
void turn(double target, double kp, double ki, double kd, double timeout);

void run();
namespace robot {
  namespace command {
    int a; //forwards backwards
    int b; //left right
    int c; //turning
    int d;
  }
  namespace drivet {
    double u;
    double r;
    double d;
    double l;
    double k = 1;
  }
  namespace bypass {
    bool driving = false; //bypass for driving
  }
  namespace constants {
    int maxMotorSpeed = 100;
  }
  namespace pid {
    double kp = 1;
    double ki = 1;
    double kd = 1;
  }
  namespace angl {
    double rot;
    double head;
    double limrot;
  }
  

}

// U   R
//   X
// L   D


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  thread myThread = thread(run);
  BrainInertial.setHeading(0,degrees);
  while (true) {
    drive(60,2000,100,1,0.1,0.1,0);
    wait(20, msec);
  }

}

void turnto(int heading, double kp, double ki, double kd, double timeout) {
  //
}

void run() {
  while (true) {

    robot::angl::head = BrainInertial.heading(degrees);

    robot::angl::limrot = (robot::angl::head > 179.9) ? -(360 - robot::angl::head) : robot::angl::head;
  wait(20, msec);
  }
}





void drive(double dir, double dist, double k, double kp, double ki, double kd, double timeout) {
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 5;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;
  double average;
  double current;
  double maximum;
  Brain.Timer.reset();
  robot::command::a = k * cos(dir); //forward backward
  robot::command::b = k * sin(dir); //strafing
  robot::command::c = 0; //rotatoin (don't need)

  while (true) {
    current = (fabs(ApositiveU.position(degrees)) + fabs(BpositiveR.position(degrees)) + fabs(AnegativeD.position(degrees)) + fabs(bNegativeL.position(degrees)))/4;
    error = dist - current;
    derivative = error-lastError;
    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }


    // Log the values into the console


    if (fabs(error) < threshold) {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      bNegativeL.stop();
      break; //:D
    }
    
    if (Brain.Timer.value() > timeout && timeout != 0) {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      bNegativeL.stop();
      break; //:D     
    }

    if (integral > maxIntegral) integral = maxIntegral;
    if (integral < -maxIntegral) integral = -maxIntegral;

    double motorSpeed = (kp * error) + (ki * integral) + (kd * derivative);

    if (motorSpeed > maxSpeed) motorSpeed = maxSpeed;
    if (motorSpeed < -maxSpeed) motorSpeed = -maxSpeed;
    robot::drivet::u = robot::command::a + robot::command::b + robot::command::c;
    robot::drivet::r = robot::command::a - robot::command::b - robot::command::c;
    robot::drivet::d = robot::command::a - robot::command::b + robot::command::c;
    robot::drivet::l = robot::command::a + robot::command::b - robot::command::c;

    if (robot::drivet::u * motorSpeed > robot::constants::maxMotorSpeed) {maximum = robot::drivet::u * motorSpeed; }
    if (robot::drivet::r * motorSpeed > robot::constants::maxMotorSpeed) {maximum = robot::drivet::r * motorSpeed; }
    if (robot::drivet::d * motorSpeed > robot::constants::maxMotorSpeed) {maximum = robot::drivet::d * motorSpeed; }
    if (robot::drivet::l * motorSpeed > robot::constants::maxMotorSpeed) {maximum = robot::drivet::l * motorSpeed; }
    /*
    robot::drivet::u *= maximum / (robot::drivet::u * motorSpeed > robot::constants::maxMotorSpeed);
    robot::drivet::r *= maximum / (robot::drivet::r * motorSpeed > robot::constants::maxMotorSpeed);
    robot::drivet::d *= maximum / (robot::drivet::d * motorSpeed > robot::constants::maxMotorSpeed);
    robot::drivet::l *= maximum / (robot::drivet::l * motorSpeed > robot::constants::maxMotorSpeed);
    */
    robot::drivet::u *= motorSpeed / 100;
    robot::drivet::r *= motorSpeed / 100;
    robot::drivet::d *= motorSpeed / 100;
    robot::drivet::l *= motorSpeed / 100;
    printf("Error: %f\n", error);
    printf("Integral: %f\n", integral);
    printf("Derivative: %f\n", derivative);
    printf("Command: %f\n", motorSpeed);
    printf("U Command: %f\n", robot::drivet::u);
    printf("R Command: %f\n", robot::drivet::r);
    printf("D Command: %f\n", robot::drivet::d);
    printf("L Command: %f\n", robot::drivet::l);
    printf("\n");

    ApositiveU.spin(forward, robot::drivet::u, percent);
    BpositiveR.spin(forward, robot::drivet::r, percent);
    AnegativeD.spin(forward, robot::drivet::d, percent);
    bNegativeL.spin(forward, robot::drivet::l, percent);

    lastError = error;
    wait(20, msec);
  }
  ApositiveU.stop();
  BpositiveR.stop();
  AnegativeD.stop();
  bNegativeL.stop();
}
void turn(double target, double kp, double ki, double kd, double timeout) {
  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 2.5;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;

  while (true) {
    error = target - robot::angl::limrot;
    derivative = error-lastError;
    if (fabs(error) < threshold) {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      bNegativeL.stop();
      break; //:D
    }
  
    if (fabs(error) < integralResetZone) {
      integral += error;
    } else {
      integral = 0;
    }

    if (integral > maxIntegral) integral = maxIntegral;
    if (integral < -maxIntegral) integral = -maxIntegral;

    double motorSpeed = (kp * error) + (ki * integral) + (kd * derivative);

    if (motorSpeed > maxSpeed) {motorSpeed = maxSpeed; }
    if (motorSpeed < -maxSpeed) {motorSpeed = -maxSpeed; }
    ApositiveU.spin(forward, motorSpeed, percent);
    BpositiveR.spin(forward, motorSpeed, percent);
    AnegativeD.spin(forward, motorSpeed, percent);
    bNegativeL.spin(forward, motorSpeed, percent);
    lastError = error;
  }
}

