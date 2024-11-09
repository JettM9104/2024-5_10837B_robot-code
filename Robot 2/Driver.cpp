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
motor LeftDriveSmart = motor(PORT7, 4.5, false);
motor RightDriveSmart = motor(PORT1, 4.5, true);

smartdrive Drivetrain = smartdrive(LeftDriveSmart, RightDriveSmart, BrainInertial, 200);

pneumatic Pneumatic6 = pneumatic(PORT6);
motor motorShareAMotorA = motor(PORT3, false);
motor motorShareAMotorB = motor(PORT9, false);
motor_group motorShareA = motor_group(motorShareAMotorA, motorShareAMotorB);

controller Controller = controller();
touchled TouchLED2 = touchled(PORT2);
touchled TouchLED8 = touchled(PORT8);


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

bool vexcode_initial_drivetrain_calibration_completed = false;
void calibrateDrivetrain() {
  if (!vexcode_initial_drivetrain_calibration_completed) {
    Pneumatic6.pumpOff();
  }
  wait(200, msec);
  Brain.Screen.print("Calibrating");
  Brain.Screen.newLine();
  Brain.Screen.print("Inertial");
  BrainInertial.calibrate();
  while (BrainInertial.isCalibrating()) {
    wait(25, msec);
  }
  if (!vexcode_initial_drivetrain_calibration_completed) {
    Pneumatic6.pumpOn();
  }
  vexcode_initial_drivetrain_calibration_completed = true;
  // Clears the screen and returns the cursor to row 1, column 1.
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
}

void vexcodeInit() {

  // Calibrate the Drivetrain
  calibrateDrivetrain();

  // Initializing random seed.
  initializeRandomSeed(); 
}


// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool ControllerLeftShoulderControlMotorsStopped = true;
bool DrivetrainLNeedsToBeStopped_Controller = true;
bool DrivetrainRNeedsToBeStopped_Controller = true;

// define a task that will handle monitoring inputs from Controller
int rc_auto_loop_function_Controller() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      
      // calculate the drivetrain motor velocities from the controller joystick axies
      // left = AxisA + AxisC
      // right = AxisA - AxisC
      int drivetrainLeftSideSpeed = Controller.AxisA.position() + Controller.AxisC.position();
      int drivetrainRightSideSpeed = Controller.AxisA.position() - Controller.AxisC.position();
      
      // check if the value is inside of the deadband range
      if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
        // check if the left motor has already been stopped
        if (DrivetrainLNeedsToBeStopped_Controller) {
          // stop the left drive motor
          LeftDriveSmart.stop();
          // tell the code that the left motor has been stopped
          DrivetrainLNeedsToBeStopped_Controller = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
        DrivetrainLNeedsToBeStopped_Controller = true;
      }
      // check if the value is inside of the deadband range
      if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
        // check if the right motor has already been stopped
        if (DrivetrainRNeedsToBeStopped_Controller) {
          // stop the right drive motor
          RightDriveSmart.stop();
          // tell the code that the right motor has been stopped
          DrivetrainRNeedsToBeStopped_Controller = false;
        }
      } else {
        // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
        DrivetrainRNeedsToBeStopped_Controller = true;
      }
      
      // only tell the left drive motor to spin if the values are not in the deadband range
      if (DrivetrainLNeedsToBeStopped_Controller) {
        LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
        LeftDriveSmart.spin(forward);
      }
      // only tell the right drive motor to spin if the values are not in the deadband range
      if (DrivetrainRNeedsToBeStopped_Controller) {
        RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
        RightDriveSmart.spin(forward);
      }
      // check the ButtonLUp/ButtonLDown status to control motorShareA
      if (Controller.ButtonLUp.pressing()) {
        motorShareA.spin(forward);
        ControllerLeftShoulderControlMotorsStopped = false;
      } else if (Controller.ButtonLDown.pressing()) {
        motorShareA.spin(reverse);
        ControllerLeftShoulderControlMotorsStopped = false;
      } else if (!ControllerLeftShoulderControlMotorsStopped) {
        motorShareA.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        ControllerLeftShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller(rc_auto_loop_function_Controller);

#pragma endregion VEXcode Generated Robot Configuration

// Include the IQ Library
#include "vex.h"
  
// Allows for easier use of the VEX Library
using namespace vex;

// User defined function
void myblockfunction_rbw1_latency(double myblockfunction_rbw1_latency__latency);

float tankState, motorShare, platformAnge, LED8Mode, rbmd;

// User defined function
void myblockfunction_rbw1_latency(double myblockfunction_rbw1_latency__latency) {
  TouchLED8.setFade(slow);
  TouchLED8.setColor(red);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(red_orange);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(orange);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(yellow_orange);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(yellow);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(yellow_green);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(green);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(blue_green);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(blue);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(blue_violet);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(violet);
  wait(myblockfunction_rbw1_latency__latency, seconds);
  TouchLED8.setColor(red_violet);
  wait(myblockfunction_rbw1_latency__latency, seconds);
}

// "when Controller ButtonRUp pressed" hat block
void onevent_ControllerButtonRUp_pressed_0() {
  motorShare = motorShare + 1.0;
}

// "when started" hat block
int whenStarted1() {
  motorShare = 0.0;
  platformAnge = 0.0;
  LED8Mode = 0.0;
  tankState = 0.0;
  motorShareA.setVelocity(100.0, percent);
  motorShareA.setMaxTorque(100.0, percent);
  while (true) {
    if (fmod(tankState,2.0) == 1.0) {
      Pneumatic6.pumpOn();
    }
    else {
      Pneumatic6.pumpOff();
    }
    if (fmod(motorShare,2.0) == 1.0) {
      Pneumatic6.extend(cylinder2);
    }
    else {
      Pneumatic6.retract(cylinder2);
    }
    if (fmod(platformAnge,2.0) == 1.0) {
      Pneumatic6.extend(cylinder1);
    }
    else {
      Pneumatic6.retract(cylinder1);
    }
  wait(20, msec);
  }
  return 0;
}

// "when Controller ButtonRDown pressed" hat block
void onevent_ControllerButtonRDown_pressed_0() {
  platformAnge = platformAnge + 1.0;
}

// "when Controller ButtonL3 pressed" hat block
void onevent_ControllerButtonL3_pressed_0() {
  tankState = tankState + 1.0;
}

// "when started" hat block
int whenStarted2() {
  while (true) {
    if (fmod(tankState,2.0) == 1.0) {
      TouchLED2.setColor(green);
      wait(0.5, seconds);
      TouchLED2.setColor(blue_green);
      wait(0.5, seconds);
    }
    else {
      TouchLED2.setColor(green);
    }
  wait(20, msec);
  }
  return 0;
}

// "when started" hat block
int whenStarted3() {
  while (true) {
    if (fmod(LED8Mode,3.0) == 1.0) {
      rbmd = 0.0;
      TouchLED8.setColor(colorType::none);
    } else if (fmod(LED8Mode,3.0) == 2.0) {
      rbmd = 1.0;
    } else {
      rbmd = 0.0;
      TouchLED8.setColor(white);
    }
  wait(20, msec);
  }
  return 0;
}

// "when TouchLED8 pressed" hat block
void onevent_TouchLED8_pressed_0() {
  LED8Mode = LED8Mode + 1.0;
}

// "when started" hat block
int whenStarted4() {
  while (true) {
    if (rbmd == 1.0) {
      myblockfunction_rbw1_latency(1.0);
    }
  wait(20, msec);
  }
  return 0;
}


int main() {
  // Calibrate the Drivetrain Gyro
  calibrateDrivetrain();

  // register event handlers
  Controller.ButtonRUp.pressed(onevent_ControllerButtonRUp_pressed_0);
  Controller.ButtonRDown.pressed(onevent_ControllerButtonRDown_pressed_0);
  Controller.ButtonL3.pressed(onevent_ControllerButtonL3_pressed_0);
  TouchLED8.pressed(onevent_TouchLED8_pressed_0);

  wait(15, msec);
  vex::task ws1(whenStarted2);
  vex::task ws2(whenStarted3);
  vex::task ws3(whenStarted4);
  whenStarted1();
}