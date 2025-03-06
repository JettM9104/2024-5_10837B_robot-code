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
motor PDSGleft = motor(PORT3, false);
motor PDSGright = motor(PORT6, true);
motor Metroleft = motor(PORT1, false);
motor metroright = motor(PORT5, true);
pneumatic shares = pneumatic(PORT8);
pneumatic back = pneumatic(PORT9);
touchled led = touchled(PORT7);
distance Catadist = distance(PORT10);
motor driveleft = motor(PORT2, true);
motor driveright = motor(PORT4, false);
distance Distance12 = distance(PORT12);


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

// Include the IQ Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

// User defined function
void myblockfunction_Drive();
// User defined function
void myblockfunction_Setup();
// User defined function
void myblockfunction_Pneu();
// User defined function
void myblockfunction_Relay();
// User defined function
void myblockfunction_Cata();

int Brain_precision = 0, Console_precision = 0;

float myVariable, Error, Output, LastError, Derivative, Integral, KPD, KDD, KID;

// User defined function
void myblockfunction_Drive() {
  led.setColor(blue);
  Metroleft.spin(forward);
  metroright.spin(forward);
  driveleft.spinFor(forward, 1.525, turns, false);
  driveright.spinFor(forward, 1.525, turns, true);
  metroright.stop();
  Metroleft.stop();
  led.setColor(blue);
  driveright.stop();
  PDSGright.stop();
  driveleft.spinFor(forward, 0.7, turns, false);
  PDSGleft.spinFor(forward, 0.7, turns, true);
  led.setColor(red);
  driveleft.spinFor(reverse, 2.4, turns, false);
  driveright.spinFor(reverse, 2.4, turns, false);
  PDSGleft.spinFor(reverse, 2.4, turns, false);
  PDSGright.spinFor(reverse, 2.4, turns, true);
}

// User defined function
void myblockfunction_Setup() {
  led.setColor(red);
  back.retract(cylinder1);
  shares.extend(cylinder1);
  Metroleft.spinFor(forward, 2.0, turns, false);
  metroright.spinFor(forward, 2.0, turns, true);
  back.extend(cylinder1);
}

// User defined function
void myblockfunction_Pneu() {
  PDSGleft.stop();
  PDSGright.stop();
  Metroleft.stop();
  metroright.stop();
  driveleft.stop();
  driveright.stop();
  back.retract(cylinder2);
}

// User defined function
void myblockfunction_Relay() {
  PDSGleft.spin(reverse);
  PDSGright.spin(reverse);
  Metroleft.spin(forward);
  metroright.spin(forward);
  driveleft.setTimeout(0.7, seconds);
  driveright.setTimeout(0.7, seconds);
  while (true) {
    // rapid load
    driveright.spinFor(forward, 0.25, turns, false);
    driveleft.spinFor(forward, 0.2, turns, true);
    // start to drive back
    driveright.spinFor(reverse, 0.4, turns, false);
    driveleft.spinFor(reverse, 0.4, turns, true);
    Brain.Timer.reset();
    while (!(Distance12.objectDistance(mm) > 200.0)) {
      if (200.0 > Distance12.objectDistance(mm) && Brain.Timer.value() > 2.0) {
        shares.extend(cylinder2);
        wait(1.0, seconds);
        shares.retract(cylinder2);
      }
    wait(20, msec);
    }
  wait(20, msec);
  }
}

// User defined function
void myblockfunction_Cata() {
  back.extend(cylinder1);
  metroright.spin(reverse);
  Metroleft.spin(reverse);
  wait(0.4, seconds);
  metroright.stop();
  Metroleft.stop();
  wait(0.8, seconds);
  metroright.spin(reverse);
  Metroleft.spin(reverse);
  wait(0.2, seconds);
  metroright.stop();
  Metroleft.stop();
  back.retract(cylinder1);
}

// Used to find the format string for printing numbers with the
// desired number of decimal places
const char* printToConsole_numberFormat() {
  // look at the current precision setting to find the format string
  switch(Console_precision){
    case 0:  return "%.0f"; // 0 decimal places (1)
    case 1:  return "%.1f"; // 1 decimal place  (0.1)
    case 2:  return "%.2f"; // 2 decimal places (0.01)
    case 3:  return "%.3f"; // 3 decimal places (0.001)
    default: return "%f"; // use the print system default for everthing else
  }
}

// "when started" hat block
int whenStarted1() {
  while (true) {
    printf(printToConsole_numberFormat(), static_cast<float>(Distance12.objectDistance(mm)));
    printf("\n");
    if (led.pressing()) {
      myblockfunction_Setup();
      waitUntil(led.pressing());
      myblockfunction_Drive();
      myblockfunction_Pneu();
      myblockfunction_Cata();
      myblockfunction_Relay();
    }
  wait(20, msec);
  }
  return 0;
}

// "when started" hat block
int whenStarted2() {
  driveright.setStopping(hold);
  driveleft.setStopping(hold);
  back.retract(cylinder1);
  led.setColor(red);
  back.extend(cylinder2);
  led.setColor(green);
  PDSGleft.setVelocity(1000.0, percent);
  PDSGright.setVelocity(1000.0, percent);
  Metroleft.setVelocity(1000.0, percent);
  metroright.setVelocity(1000.0, percent);
  PDSGleft.setMaxTorque(1000.0, percent);
  PDSGright.setMaxTorque(1000.0, percent);
  Metroleft.setMaxTorque(1000.0, percent);
  metroright.setMaxTorque(1000.0, percent);
  BrainInertial.calibrate();
  while (BrainInertial.isCalibrating()) { task::sleep(50); }
  return 0;
}


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  vex::task ws1(whenStarted2);
  whenStarted1();
}