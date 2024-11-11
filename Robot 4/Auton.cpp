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
motor ApositiveU = motor(PORT1, false);
motor BpositiveR = motor(PORT6, false);
motor AnegativeD = motor(PORT7, false);
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

void driveDiagonal(double direction, double distance, double speed);
const double pi = 3.1415926;
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
}

void driveDiagonal(double direction, double distance, double speed) {
    double rotation = distance / (pi * 10);
    double x_speed = speed * cos(direction);
    double y_speed = speed * sin(direction);

    double target_x = current_x + distance * cos(direction);
    double target_y = current_y + distance * sin(direction);

    double traveled_distance = 0;
    double target_distance = rotation * (M_PI * 10);
    double start_time = Brain.Timer.system();

    ApositiveU.spin(forward, x_speed, percent);
    BpositiveR.spin(forward, y_speed, percent);
    AnegativeD.spin(reverse, y_speed, percent);
    bNegativeL.spin(reverse, x_speed, percent);

    while (traveled_distance < target_distance) {
        double current_time = Brain.Timer.system();
        traveled_distance = speed * (current_time - start_time);

        double current_x = start_x + traveled_distance * cos(current_direction);
        double current_y = start_y + traveled_distance * sin(current_direction);

        double current_heading = BrainInertial.heading(degrees);
        double desired_heading = direction * (180.0 / M_PI);

        double heading_error = desired_heading - current_heading;

        double correction_factor = 1.0 + (heading_error * 0.1);
        ApositiveU.spin(forward, x_speed * correction_factor, percent);
        BpositiveR.spin(forward, y_speed * correction_factor, percent);
        AnegativeD.spin(reverse, y_speed * correction_factor, percent);
        bNegativeL.spin(reverse, x_speed * correction_factor, percent);
    }

    ApositiveU.stop();
    BpositiveR.stop();
    AnegativeD.stop();
    bNegativeL.stop();
}


