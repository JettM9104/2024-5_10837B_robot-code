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

controller Controller;

// START IQ MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS

typedef enum{ MODE_1, MODE_2, MODE_3, MODE_4} device;

class differential {
private:
  motor motor1;
  motor motor2;
  int predetermined_velocity;

public:
  differential(motor motor1, motor motor2): motor1(motor1), motor2(motor2) {}
  

  void spin(device component, int velocity, percentUnits units) {
    if (component == MODE_1) {
      motor1.spin(forward, velocity, units);
      motor2.spin(forward, velocity, units);
    }
    else if (component == MODE_2) {
      motor1.spin(forward, velocity, units);
      motor2.spin(reverse, velocity, units);
    }
    else if (component == MODE_3) {
      motor1.spin(reverse, velocity, units);
      motor2.spin(forward, velocity, units);
    }
    else {
      motor1.spin(reverse, velocity, units);
      motor2.spin(reverse, velocity, units);
    }
  }
  void spin(device component) {
    if (component == MODE_1) {
      motor1.spin(forward, predetermined_velocity, percent);
      motor2.spin(forward, predetermined_velocity, percent);
    }
    else if (component == MODE_2) {
      motor1.spin(forward, predetermined_velocity, percent);
      motor2.spin(reverse, predetermined_velocity, percent);
    }
    else if (component == MODE_3) {
      motor1.spin(reverse, predetermined_velocity, percent);
      motor2.spin(forward, predetermined_velocity, percent);
    }
    else {
      motor1.spin(reverse, predetermined_velocity, percent);
      motor2.spin(reverse, predetermined_velocity, percent);
    }
  }
  void stop() {
    motor1.stop();
    motor2.stop();
  }

  void setVelocity(int speed, percentUnits unit) {
    if (unit == percent) {
      predetermined_velocity = speed;
    }
    else {
      predetermined_velocity = speed * 5 / 6;
    }
  }

  void setMaxTorque(int value, percentUnits unit) {
    if (unit == percent) {
      motor1.setMaxTorque(value, percent);
    }
    else {
      //error
    }
  }
};
// Robot configuration code.
inertial BrainInertial = inertial();
motor leftDiff = motor(PORT1, true);
motor rightDiff = motor(PORT6, false);
differential ohio = differential(leftDiff, rightDiff);


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

// Include the IQ Library
#include "iq_cpp.h"

// Allows for easier use of the VEX Library
using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  // Begin project code

  ohio.spin(MODE_1, 100, percent);
  
}
