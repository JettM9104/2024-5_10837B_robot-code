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
void turnto(int heading, double kp, double ki, double kd, double timeout);
void run();
namespace robot {
  namespace contr {
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
  
  while (true) {
    robot::contr::a = Controller.AxisA.position();
    robot::contr::b = Controller.AxisB.position();
    robot::contr::c = Controller.AxisC.position();
    robot::contr::d = Controller.AxisD.position();

    if (!robot::bypass::driving) {
      robot::drivet::u = robot::contr::a + robot::contr::b + robot::contr::c;
      robot::drivet::r = robot::contr::a - robot::contr::b - robot::contr::c;
      robot::drivet::d = robot::contr::a - robot::contr::b + robot::contr::c;
      robot::drivet::l = robot::contr::a + robot::contr::b - robot::contr::c;

      if (robot::drivet::u > robot::constants::maxMotorSpeed) {robot::drivet::u = robot::constants::maxMotorSpeed; }
      if (robot::drivet::r > robot::constants::maxMotorSpeed) {robot::drivet::r = robot::constants::maxMotorSpeed; }
      if (robot::drivet::d > robot::constants::maxMotorSpeed) {robot::drivet::d = robot::constants::maxMotorSpeed; }
      if (robot::drivet::l > robot::constants::maxMotorSpeed) {robot::drivet::l = robot::constants::maxMotorSpeed; }

      ApositiveU.spin(forward, robot::drivet::u, percent);
      BpositiveR.spin(forward, robot::drivet::r, percent);
      AnegativeD.spin(forward, robot::drivet::d, percent);
      bNegativeL.spin(forward, robot::drivet::l, percent);
    }
    else {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      bNegativeL.stop();
    }
  }

}

void turnto(int heading, double kp, double ki, double kd, double timeout) {
  //
}

void run() {
  while (true) {

    robot::angl::head = BrainInertial.heading(degrees);

    if (robot::angl::head > 179.9) {
      robot::angl::limrot = -(360-robot::angl::head);
    }
    else {
      robot::angl::limrot = robot::angl::head;
    }
  }
}
