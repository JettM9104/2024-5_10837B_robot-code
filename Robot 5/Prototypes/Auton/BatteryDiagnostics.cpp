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
motor leftDrivetrain = motor(PORT1, true);
motor rightDrivetrain = motor(PORT2, false);

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



int main() {
  double x = Brain.Battery.voltage(volt);
  double y = Brain.Battery.current(amp);
  double z = Brain.Battery.capacity(percent);

  while (true) {
    Brain.Screen.print("Voltage: %f V\n", x);
    Brain.Screen.print("Current: %f A\n", y);
    Brain.Screen.print("Capacity: %f %\n", z);
    Brain.Screen.print("Wattage: %f W\n", x*y)

    while (!Brain.buttonLeft.pressing()) { wait(20, msec); }
  }
} 