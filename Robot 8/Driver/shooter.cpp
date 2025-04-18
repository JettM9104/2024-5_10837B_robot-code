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
controller Controller;
motor diffLeft = motor(PORT7, true);
motor diffRight = motor(PORT4, false);
motor leftDrive = motor(PORT9, true); // confirmed
motor rightDrive = motor(PORT3, false); // confirmed
motor intake = motor(PORT11);
motor metro = motor(PORT5);
distance chassis = distance(PORT1);
touchled indicator = touchled(PORT2);
bumper catapultDetector = bumper(PORT10);


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

bool backroller = 0;

u_int8_t macrosActive = 0;

void updateMotors();
void updateBackroller();
void updateCataMotors();
void updateLED();

void checkPorts();
void init();

void backrollerSensor();
void windCata();
void shootCata();

void triggerWindCata();
void triggerShootCata();
void triggerBackrollerSensor();

void updateScreen();
void updateConsole();

bool error = false;
bool driveActive = true;

// -------------------- MAIN FUNCTION DE
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  checkPorts();
  init();

  Controller.ButtonLDown.pressed(updateMotors);
  Controller.ButtonLUp.pressed(updateMotors);
  Controller.ButtonLUp.released(updateMotors);
  Controller.ButtonLDown.released(updateMotors);

  Controller.ButtonRUp.pressed(updateCataMotors);
  Controller.ButtonRDown.pressed(updateCataMotors);
  Controller.ButtonRUp.released(updateCataMotors);
  Controller.ButtonRDown.released(updateCataMotors);

  Controller.ButtonFUp.pressed(updateBackroller);
  Controller.ButtonFUp.pressed(updateMotors);

  Controller.ButtonEDown.pressed(triggerWindCata);
  Controller.ButtonEUp.pressed(triggerShootCata);
  Controller.ButtonFDown.pressed(triggerBackrollerSensor);


  thread indic = thread(updateLED);

  thread cons = thread(updateConsole);


  indic.setPriority(1);

  indicator.setBrightness(50);

  while (true) {
    if (driveActive) {
      if ((abs(Controller.AxisA.position()) + abs(Controller.AxisC.position())) > 5) {
        leftDrive.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
        rightDrive.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent);
      }
      else {
        leftDrive.stop();
        rightDrive.stop();
      }
    }
    wait(20, msec);
  }
}


// ------------------------------ FUNCTION DEFINITIONS -----------------------------------------

// -----------------MACRO TRIGGERS ---------------------
void triggerWindCata() {
  thread thd1 = thread(windCata);
  thd1.setPriority(5);
  thd1.join();
}
void triggerShootCata() {
  thread thd2 = thread(shootCata);
  thd2.setPriority(4);
  thd2.join();
}
void triggerBackrollerSensor() {
  thread thd3 = thread(backrollerSensor);
  thd3.setPriority(4);
  thd3.join();
}

// -----------------INITILIZATIONS AND MISCELLANIOUS----------------

void init() {
  printf("%ld\n", thread::hardware_concurrency());
  leftDrive.setMaxTorque(100, percent);
  rightDrive.setMaxTorque(100, percent);
  diffLeft.setMaxTorque(100, percent);
  diffRight.setMaxTorque(100, percent);
  intake.setMaxTorque(100, percent);
  metro.setMaxTorque(100, percent);

  leftDrive.setVelocity(100, percent);
  rightDrive.setVelocity(100, percent);
  diffLeft.setVelocity(100, percent);
  diffRight.setVelocity(100, percent);
  intake.setVelocity(100, percent);
  metro.setVelocity(100, percent);
}

void checkPorts() {
  unsigned short int y = 1;
  Brain.Screen.setFont(mono15);
  Brain.Screen.setCursor(y, 1);
  if (!leftDrive.installed()) {
    y++;
    Brain.Screen.print("9-leftDrive");
  }
  Brain.Screen.setCursor(y, 1);

  if (!rightDrive.installed()) {
    y++;
    Brain.Screen.print("3-rightDrive");
  }
  Brain.Screen.setCursor(y, 1);
  if (!diffLeft.installed()) {
    y++;
    Brain.Screen.print("7-diffLeft");
  }
  Brain.Screen.setCursor(y, 1);

  if (!diffRight.installed()) {
    y++;
    Brain.Screen.print("4-diffRight");
  }
  Brain.Screen.setCursor(y, 1);

  if (!metro.installed()) {
    y++;
    Brain.Screen.print("5-metro");
  }
  Brain.Screen.setCursor(y, 1);

  if (!intake.installed()) {
    y++;
    Brain.Screen.print("11-intake");
  }
  Brain.Screen.setCursor(y, 1);

  if (!chassis.installed()) {
    y++;
    Brain.Screen.print("1-chassis");
  }
  Brain.Screen.setCursor(y, 1);

  if (!indicator.installed()) {
    y++;
    Brain.Screen.print("2-indicator");
  }
  Brain.Screen.setCursor(y, 1);

  if (!catapultDetector.installed()) {
    y++;
    Brain.Screen.print("10-catapultDetector");
  }
  Brain.Screen.setCursor(y, 1);
  if (y > 1){
    Brain.Screen.print("PLUG EM IN");
    error = true;
  } 

}
// -------------- CONINUOUS UPDATES ------------------
void updateConsole() {
  while (true) {
    //printf("%f\n", chassis.objectDistance(inches));
    wait(20, msec);
  }
}
// ------------- MOTOR UPDATES ---------------

void updateBackroller() {
  if (backroller) backroller = 0;
  else { backroller = 1; }
}

void updateMotors() {
  if (Controller.ButtonLDown.pressing()) { // INTAKE
    diffLeft.spin(forward, 100, percent);
    diffRight.spin(reverse, 100, percent);
    intake.spin(reverse, 100, percent);

    if (!backroller) {
      metro.spin(forward, 100, percent);
    }
    else {
      metro.spin(reverse, 100, percent);
    }

  }
  else if (Controller.ButtonLUp.pressing()) { // REVERSE INTAKE
    diffLeft.spin(reverse, 100, percent);
    diffRight.spin(forward, 100, percent);
    intake.spin(forward, 100, percent);
  }
  else {
    diffLeft.stop();
    diffRight.stop();
    intake.stop();
    metro.stop();
  }
}

void updateCataMotors() {
  if (Controller.ButtonRDown.pressing()) { // WIND CATA
    diffLeft.spin(reverse, 100, percent);
    diffRight.spin(reverse, 100, percent);
  }
  else if (Controller.ButtonRUp.pressing()) { // SHOOT CATA
    diffLeft.spin(forward, 100, percent);
    diffRight.spin(forward, 100, percent);
  }
  else {
    diffLeft.stop();
    diffRight.stop();
  }
}
void updateLED() {
  while (true) {
    if (!error) {
      if (backroller) {
        indicator.setColor(blue_green);
      }
      else {
        indicator.setColor(white);
      }
      if (macrosActive > 0) {
        wait(300, msec);
        indicator.setColor(yellow);
        wait(300, msec);
      }
      
    }
    else {
      indicator.setColor(red);
      wait(300, msec);
      indicator.setColor(colorType::none);
      wait(300, msec);
    }
    wait(20, msec);
  }
}


// ------------------ MACROS ---------------------
void windCata() { // E DOWN MACRO 
  macrosActive++;
  while (Controller.ButtonEDown.pressing()) wait(20, msec);

  do {
    diffLeft.spin(reverse, 100, percent);
    diffRight.spin(reverse, 100, percent);
    wait(20, msec);
  } while (!catapultDetector.pressing() && !Controller.ButtonEDown.pressing());

  diffLeft.stop();
  diffRight.stop();
  macrosActive--;
}

void shootCata() {
  diffLeft.spin(forward, 100, percent);
  diffRight.spin(forward, 100, percent);
  wait(300, msec);
  diffLeft.stop();
  diffRight.stop();
}

void backrollerSensor() { // F DOWN MACRO
  macrosActive++;
  while (Controller.ButtonFDown.pressing()) wait(20, msec);
  driveActive = false;
  while (true) {
    if (Controller.ButtonFDown.pressing()) break;
    printf("1\n");
    diffLeft.spin(forward, 100, percent);
    diffRight.spin(reverse, 100, percent);
    intake.spin(reverse, 100, percent);
    metro.spin(forward, 100, percent);
    while (chassis.objectDistance(inches) > 33 && !Controller.ButtonFDown.pressing()) wait(20, msec);
    printf("2\n");
    if (Controller.ButtonFDown.pressing()) break;
    printf("3\n");
    leftDrive.spin(reverse, -100, percent);
    rightDrive.spin(reverse, -100, percent);

    Brain.Timer.reset();
    while (Brain.Timer.value() < 0.4 && !Controller.ButtonFDown.pressing()) wait(20, msec);
    if (Controller.ButtonFDown.pressing()) break;
    printf("4\n");
    leftDrive.spin(forward, -100, percent);
    rightDrive.spin(forward, -100, percent);

    Brain.Timer.reset();
    while (Brain.Timer.value() < 0.5 && !Controller.ButtonFDown.pressing()) wait(20, msec);
    if (Controller.ButtonFDown.pressing()) break;
    printf("5\n");
    leftDrive.stop();
    rightDrive.stop();
  }
  driveActive = false;
  printf("b\n");
  diffLeft.stop();
  diffRight.stop();
  intake.stop();
  metro.stop();
  macrosActive--;
}
