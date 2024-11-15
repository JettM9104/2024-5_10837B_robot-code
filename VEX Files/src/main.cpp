/*----------------------------------------------------------------------------------------*/
/*                                                                                        */
/*    Project:          Split Arcade                                                      */
/*    Module:           main.cpp                                                          */
/*    Author:           VEX                                                               */
/*    Created:          Fri Aug 05 2022                                                   */
/*    Description:      The Left up/down Controller Axis (A) will drive the robot         */
/*                      forward and backwards.                                            */
/*                      The Right left/right Controller Axis (C) will turn the robot      */
/*                      left and right.                                                   */
/*                      The deadband variable prevents drift when the Controller's        */
/*                      joystick is released.                                             */
/*                                                                                        */
/*    Configuration:    Left Motor in Port 1                                              */
/*                      Right Motor in Port 6                                             */
/*                      Controller                                                        */
/*                                                                                        */
/*----------------------------------------------------------------------------------------*/

// Include the IQ Library
#include "vex.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Brain should be defined by default
brain Brain;

// Robot configuration code.
inertial BrainInertial = inertial();
motor LeftMotor = motor(PORT1, false);
motor RightMotor = motor(PORT6, true);
controller Controller = controller();

int main() {
  // Begin project code
  
  // Set the deadband variable
  float deadband = 5.0;

  // Main controller loop to set motor velocities to controller axis positions
  while (true) {
    if (fabs(static_cast<float>(Controller.AxisA.position())) + fabs(static_cast<float>(Controller.AxisC.position())) > deadband) {
      LeftMotor.setVelocity((Controller.AxisA.position() + Controller.AxisC.position()), percent);
      RightMotor.setVelocity((Controller.AxisA.position() - Controller.AxisC.position()), percent);
    } else {
      LeftMotor.setVelocity(0, percent);
      RightMotor.setVelocity(0, percent);
    }
    LeftMotor.spin(forward);
    RightMotor.spin(forward);

    wait(20, msec);
  }
}
