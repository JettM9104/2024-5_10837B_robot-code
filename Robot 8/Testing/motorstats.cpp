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
  float av5[5];
  float a10[10];
  float a20[20];

  int av5c = 0;
  int a10c = 0;
  int a20c = 0;

  float av5f;
  float a10f;
  float a20f;

  while (true) {
    mainMotor.spin(forward, 100, percent);
    Brain.Screen.print("Vel: %f", mainMotor.velocity(percent));
    Brain.Screen.setCursor(2, 1);
    Brain.Screen.print("Vel5: %f", av5f);
    Brain.Screen.setCursor(3, 1);
    Brain.Screen.print("Vel10: %f", a10f);
    Brain.Screen.setCursor(4, 1);
    Brain.Screen.print("Vel20: %f", a20f);

    Brain.Screen.setCursor(5, 1);
    Brain.Screen.print("Cur: %f", mainMotor.current(amp));



    av5[av5c] = mainMotor.velocity(percent);
    a10[a10c] = mainMotor.velocity(percent);
    a20[a20c] = mainMotor.velocity(percent);


    av5c++;
    a10c++;
    a20c++;
    if (av5c >= 5) {
      for (int i = 0; i < 5; i++) {
        av5f += av5[i];
      }
      av5f /= 6;
      av5c = 0;
    }

    if (a10c >= 10) {
      for (int i = 0; i < 10; i++) {
        a10f += a10[i];
      }
      a10f /= 11;
      a10c = 0;
    }

    if (a20c >= 20) {
      for (int i = 0; i < 20; i++) {
        a20f += a20[i];
      }
      a20f /= 21;
      a20c = 0;
    }
    wait(40, msec);
    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.setFont(mono20);
    
  }
  
}
