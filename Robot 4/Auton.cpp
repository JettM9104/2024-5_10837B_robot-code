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
motor BnegativeL = motor(PORT12, false);

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

// Namespaces for variable organization

// Namespace Tree for better organization
// robot
// ├── command
// │   ├── a (int)
// │   ├── b (int)
// │   ├── c (int)
// │   └── d (int)
// ├── drivet
// │   ├── u (double)
// │   ├── r (double)
// │   ├── d (double)
// │   ├── l (double)
// │   └── k (double, initialized to 1)
// ├── bypass
// │   └── driving (bool, initialized to false)
// ├── constants
// │   ├── maxMotorSpeed (int, initialized to 100)
// │   └── pid
// │       ├── kp (double, initialized to 1)
// │       ├── ki (double, initialized to 1)
// │       └── kd (double, initialized to 1)
// ├── angl
// │   ├── rot (double)
// │   ├── head (double)
// │   └── limrot (double)
// └── auton
//     └── pid
//         ├── threshold (double, initialized to 5)
//         ├── integralResetZone (double, initialized to 3)
//         ├── maxSpeed (double, initialized to 100)
//         ├── maximum (double, initalized to 1)
//         ├── u
//         │   ├── error (double, initialized to 0)
//         │   ├── integral (double, initialized to 0)
//         │   ├── derivative (double, initialized to 0)
//         │   ├── lastError (double, initialized to 0)
//         │   └── motorSpeed (double)
//         ├── r
//         │   ├── error (double, initialized to 0)
//         │   ├── integral (double, initialized to 0)
//         │   ├── derivative (double, initialized to 0)
//         │   ├── lastError (double, initialized to 0)
//         │   └── motorSpeed (double)
//         ├── d
//         │   ├── error (double, initialized to 0)
//         │   ├── integral (double, initialized to 0)
//         │   ├── derivative (double, initialized to 0)
//         │   ├── lastError (double, initialized to 0)
//         │   └── motorSpeed (double)
//         └── l
//             ├── error (double, initialized to 0)
//             ├── integral (double, initialized to 0)
//             ├── derivative (double, initialized to 0)
//             ├── lastError (double, initialized to 0)
//             └── motorSpeed (double)


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

    namespace pid {
      double kp = 1;
      double ki = 1;
      double kd = 1;
    }
  }

  namespace angl {
    double rot;
    double head;
    double limrot;
  }
  
  namespace auton {
    namespace pid {
      double threshold = 5,  integralResetZone = 3,  maxSpeed = 100;
      double maximum = 1;

      namespace u {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
      namespace r {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
      namespace d {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
      namespace l {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
    }
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

void run() {
  while (true) {
    robot::angl::head = BrainInertial.heading(degrees);
    robot::angl::limrot = (robot::angl::head > 179.9) ? -(360 - robot::angl::head) : robot::angl::head;
    wait(20, msec);
  }
}





void drive(double dir, double dist, double k, double kp, double ki, double kd, double timeout) {
  Brain.Timer.reset();
  robot::command::a = cos(dir); //forward backward 0.707
  robot::command::b = sin(dir); //strafing         0.707
  robot::command::c = 0; 

  robot::drivet::u = robot::command::a + robot::command::b + robot::command::c;
  robot::drivet::r = robot::command::a - robot::command::b - robot::command::c;
  robot::drivet::d = robot::command::a - robot::command::b + robot::command::c;
  robot::drivet::l = robot::command::a + robot::command::b - robot::command::c;
  
  robot::auton::pid::maximum = robot::drivet::u > robot::auton::pid::maximum ? robot::drivet::u : robot::auton::pid::maximum;
  robot::auton::pid::maximum = robot::drivet::r > robot::auton::pid::maximum ? robot::drivet::r : robot::auton::pid::maximum;
  robot::auton::pid::maximum = robot::drivet::d > robot::auton::pid::maximum ? robot::drivet::d : robot::auton::pid::maximum;
  robot::auton::pid::maximum = robot::drivet::l > robot::auton::pid::maximum ? robot::drivet::l : robot::auton::pid::maximum;

  robot::drivet::u /= robot::auton::pid::maximum;
  robot::drivet::r /= robot::auton::pid::maximum;
  robot::drivet::d /= robot::auton::pid::maximum;
  robot::drivet::l /= robot::auton::pid::maximum;

  while (true) {
    robot::auton::pid::u::error = dist - fabs(ApositiveU.position(degrees));
    robot::auton::pid::r::error = dist - fabs(BpositiveR.position(degrees));
    robot::auton::pid::d::error = dist - fabs(AnegativeD.position(degrees));
    robot::auton::pid::l::error = dist - fabs(BnegativeL.position(degrees));

    robot::auton::pid::u::integral = fabs(robot::auton::pid::u::error) > robot::auton::pid::integralResetZone ? robot::auton::pid::u::integral + robot::auton::pid::u::error : 0;
    robot::auton::pid::r::integral = fabs(robot::auton::pid::r::error) > robot::auton::pid::integralResetZone ? robot::auton::pid::r::integral + robot::auton::pid::r::error : 0;
    robot::auton::pid::d::integral = fabs(robot::auton::pid::d::error) > robot::auton::pid::integralResetZone ? robot::auton::pid::d::integral + robot::auton::pid::d::error : 0;
    robot::auton::pid::l::integral = fabs(robot::auton::pid::l::error) > robot::auton::pid::integralResetZone ? robot::auton::pid::l::integral + robot::auton::pid::l::error : 0;

    robot::auton::pid::u::derivative = robot::auton::pid::u::error - robot::auton::pid::u::lastError;
    robot::auton::pid::r::derivative = robot::auton::pid::r::error - robot::auton::pid::r::lastError;
    robot::auton::pid::d::derivative = robot::auton::pid::d::error - robot::auton::pid::d::lastError;
    robot::auton::pid::l::derivative = robot::auton::pid::l::error - robot::auton::pid::l::lastError;
    
    if (((robot::auton::pid::u::error)+(robot::auton::pid::r::error)+(robot::auton::pid::d::error)+(robot::auton::pid::l::error)/(4)) < robot::auton::pid::threshold) {break; }
    if ((timeout > Brain.Timer.value()) && (timeout != 0)) {break; }

    robot::auton::pid::u::motorSpeed = (robot::auton::pid::u::error * robot::constants::pid::kp + robot::auton::pid::u::integral * robot::constants::pid::ki + robot::auton::pid::u::derivative * robot::constants::pid::kd) * robot::drivet::u;
    robot::auton::pid::r::motorSpeed = (robot::auton::pid::r::error * robot::constants::pid::kp + robot::auton::pid::r::integral * robot::constants::pid::ki + robot::auton::pid::r::derivative * robot::constants::pid::kd) * robot::drivet::r;
    robot::auton::pid::d::motorSpeed = (robot::auton::pid::d::error * robot::constants::pid::kp + robot::auton::pid::d::integral * robot::constants::pid::ki + robot::auton::pid::d::derivative * robot::constants::pid::kd) * robot::drivet::d;
    robot::auton::pid::l::motorSpeed = (robot::auton::pid::l::error * robot::constants::pid::kp + robot::auton::pid::l::integral * robot::constants::pid::ki + robot::auton::pid::l::derivative * robot::constants::pid::kd) * robot::drivet::l;

    ApositiveU.spin(forward, robot::auton::pid::u::motorSpeed, percent);
    BpositiveR.spin(forward, robot::auton::pid::r::motorSpeed, percent);
    AnegativeD.spin(forward, robot::auton::pid::d::motorSpeed, percent);
    BnegativeL.spin(forward, robot::auton::pid::l::motorSpeed, percent);
  }
  ApositiveU.stop();
  BpositiveR.stop();
  AnegativeD.stop();
  BnegativeL.stop();
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
      BnegativeL.stop();
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
    BnegativeL.spin(forward, motorSpeed, percent);
    lastError = error;
  }
}

