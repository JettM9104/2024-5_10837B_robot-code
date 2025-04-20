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
motor mainMotor = motor(PORT1);


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
  bool done = false;
  Brain.Timer.reset();
  Brain.Screen.setFont(mono20);
  while (true) {
    Brain.Screen.setFont(mono20);
    mainMotor.spin(forward, 100, percent);
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Cur: %f", mainMotor.current(amp));

    if (Brain.Timer.value() > 15 && !done) {
      Brain.Screen.setFont(mono15);
      Brain.Screen.setCursor(4, 1);
      if (mainMotor.current(amp) >= 1) Brain.Screen.print("PSA 10 - Excellent");
      else if (mainMotor.current(amp) > 0.99 && mainMotor.current(amp) < 1) Brain.Screen.print("PSA 9.5 Almost Perfect - Excellent");
      else if (mainMotor.current(amp) > 0.96 && mainMotor.current(amp) <= 0.99) Brain.Screen.print("PSA 9 - Good");
      else if (mainMotor.current(amp) >= 0.94 && mainMotor.current(amp) <= 0.96) Brain.Screen.print("PSA 8.5 - Above Average");
      else if (mainMotor.current(amp) > 0.91 && mainMotor.current(amp) < 0.94) Brain.Screen.print("PSA 8 - Average");
      else if (mainMotor.current(amp) >= 0.89 && mainMotor.current(amp) <= 0.91) Brain.Screen.print("PSA 7.5 - Below Average");
      else if (mainMotor.current(amp) < 0.89 && mainMotor.current(amp) > 0.8) Brain.Screen.print("PSA 7 - Poor");
      else { Brain.Screen.print("PSA 6 - Horrible"); }
      done = true;
      Brain.Screen.setFont(mono20);
    }

    wait(50, msec);
    Brain.Screen.clearLine(1);    
  }
  
}
