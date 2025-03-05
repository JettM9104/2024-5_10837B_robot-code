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
sonar Distance12 = sonar(PORT12);


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

void Driving();
void P();




// User defined function
void Driving() {
  shares.extend(cylinder1);
  led.setColor(blue);
  driveleft.spinFor(forward, 1.7, turns, false);
  driveright.spinFor(forward, 1.7, turns, true);
  led.setColor(blue);
  driveright.stop();
  PDSGright.stop();
  driveleft.spinFor(forward, 0.67, turns, false);
  PDSGleft.spinFor(forward, 0.67, turns, true);
  led.setColor(red);
  driveleft.spinFor(reverse, 2.4, turns, false);
  driveright.spinFor(reverse, 2.4, turns, false);
  PDSGleft.spinFor(reverse, 2.4, turns, false);
  PDSGright.spinFor(reverse, 2.4, turns, true);
}

// User defined function
void P() {
  PDSGleft.stop();
  PDSGright.stop();
  Metroleft.stop();
  metroright.stop();
  driveleft.stop();
  driveright.stop();
  back.retract(cylinder2);
}


// "when led pressed" hat block
void auton() {
  Driving();
  P();
  back.extend(cylinder1);
  metroright.spin(reverse);
  Metroleft.spin(reverse);
  wait(0.7, seconds);
  metroright.stop();
  Metroleft.stop();
  back.retract(cylinder1);
  wait(0.5, seconds);
  PDSGleft.spin(reverse);
  PDSGright.spin(reverse);
  Metroleft.spin(forward);
  metroright.spin(forward);
  driveleft.setTimeout(0.7, seconds);
  driveright.setTimeout(0.7, seconds);
  back.retract(cylinder2);
  wait(0.5, seconds);
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
    waitUntil((200.0 > Distance12.distance(mm)));
    waitUntil((Distance12.distance(mm) > 200.0));
  wait(20, msec);
  }
}


void init() {
  driveright.setStopping(hold);
  driveleft.setStopping(hold);
  back.extend(cylinder2);
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
  vexcodeInit();

  init();
  led.pressed(auton);
}