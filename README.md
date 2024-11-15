# 2024-5_10837B_robot-code
VIQRC - 10837B; Robot Code and Autonomous Code



# Robot 10837 B - VEX Robotics Driver and Autonomous Code

This repository contains the driver and autonomous control code for VEX Robotics competition robot 10837 B. The code is designed to control the robot in both autonomous mode and driver control mode. It is written in VEXcode V5 using C++.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [File Structure](#file-structure)
- [Driver Code](#driver-code)
- [Autonomous Code](#autonomous-code)
- [Setup and Installation](#setup-and-installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

Robot 10837 B is designed to compete in VEX Robotics competitions. The code supports two primary modes:

1. **Autonomous Mode**: The robot operates autonomously based on pre-programmed routines.
2. **Driver Control Mode**: The robot is manually controlled via a VEX controller.

The autonomous code is programmed to perform specific tasks, such as navigating the field, interacting with objects, or scoring points. The driver code handles manual input and outputs motor commands to drive the robot and control mechanisms such as lift, intake, or other actuators.

---

## Prerequisites

To use this code, ensure you have the following:

- VEX Robotics IQ Robot Brain and VEX V5 motors/sensors
- VEXcode IQ (VEX's official programming environment for VEX V5 robots)
- VEX IQ controller
- Basic understanding of VEX Robotics programming and mechanics

---

## File Structure

```
/Robot_10837_B
  ├── /src
      ├── driver.cpp          # Code for manual control of the robot
      ├── autonomous.cpp      # Code for autonomous routines
      ├── robot-config.cpp    # Hardware configuration and motor/sensor initialization
      ├── main.cpp            # Entry point of the program
  ├── /include
      └── robot-config.h      # Header file for hardware configuration
```

---

## Driver Code

The driver code is responsible for controlling the robot manually. The robot responds to inputs from the VEX controller and outputs corresponding motor commands.

### Key Features:
- Tank drive: Left joystick controls the left motors, right joystick controls the right motors.
- Mechanism control: Additional buttons or joystick inputs to control the lift, intake, and other robot mechanisms.

### Example Snippet (driver.cpp):

```cpp
#include "robot-config.h"

// Tank drive setup
void tankDrive() {
    // Left joystick controls left motors
    leftDriveMotor.spin(vex::directionType::fwd, Controller1.Axis3.position(), vex::velocityUnits::pct);
    
    // Right joystick controls right motors
    rightDriveMotor.spin(vex::directionType::fwd, Controller1.Axis2.position(), vex::velocityUnits::pct);
}

// Mechanism controls
void mechanismControl() {
    if (Controller1.ButtonR1.pressing()) {
        liftMotor.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);  // Lift up
    } else if (Controller1.ButtonR2.pressing()) {
        liftMotor.spin(vex::directionType::rev, 50, vex::velocityUnits::pct);  // Lift down
    } else {
        liftMotor.stop(vex::brakeType::hold);  // Stop lift
    }
}

int main() {
    while (true) {
        tankDrive();
        mechanismControl();
        vex::task::sleep(20);  // Sleep to allow time for other tasks
    }
}
```

---

## Autonomous Code

The autonomous code is responsible for controlling the robot without manual intervention. This is executed when the robot is in autonomous mode at the start of the match.

### Key Features:
- Pre-programmed routines that drive the robot, manipulate mechanisms, and complete tasks.
- Use of sensors (e.g., gyro, encoders, vision) to enhance the robot’s ability to follow specific paths and interact with the field.

### Example Snippet (Drivers.cpp):

```cpp
namespace robot {
  namespace contr {
    int a; //forwards backwards
    int b; //left right
    int c; //turning
    int d;
  }
  namespace drivet {
    double u;
    double r;
    double d;
    double l;
    double k = 1;
  }
  namespace bypass {
    bool driving = false; //bypass for driving
  }
  namespace constants {
    int maxMotorSpeed = 100;
  }
  namespace pid {
    double kp = 1;
    double ki = 1;
    double kd = 1;
  }
  namespace angl {
    double rot;
    double head;
    double limrot;
  }
  

}

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  thread myThread = thread(run);
  
  while (true) {
    robot::contr::a = Controller.AxisA.position();
    robot::contr::b = Controller.AxisB.position();
    robot::contr::c = Controller.AxisC.position();
    robot::contr::d = Controller.AxisD.position();

    if (!robot::bypass::driving) {
      robot::drivet::u = robot::contr::a + robot::contr::b + robot::contr::c;
      robot::drivet::r = robot::contr::a - robot::contr::b - robot::contr::c;
      robot::drivet::d = robot::contr::a - robot::contr::b + robot::contr::c;
      robot::drivet::l = robot::contr::a + robot::contr::b - robot::contr::c;

      if (robot::drivet::u > robot::constants::maxMotorSpeed) {robot::drivet::u = robot::constants::maxMotorSpeed; }
      if (robot::drivet::r > robot::constants::maxMotorSpeed) {robot::drivet::r = robot::constants::maxMotorSpeed; }
      if (robot::drivet::d > robot::constants::maxMotorSpeed) {robot::drivet::d = robot::constants::maxMotorSpeed; }
      if (robot::drivet::l > robot::constants::maxMotorSpeed) {robot::drivet::l = robot::constants::maxMotorSpeed; }

      ApositiveU.spin(forward, robot::drivet::u, percent);
      BpositiveR.spin(forward, robot::drivet::r, percent);
      AnegativeD.spin(forward, robot::drivet::d, percent);
      bNegativeL.spin(forward, robot::drivet::l, percent);
    }
    else {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      bNegativeL.stop();
    }
  }

}
```

---

## Setup and Installation

1. **Clone this repository** to your local machine or use the VEXcode V5 IDE to access your robot’s programming environment.
2. **Install VEXcode V5** if you haven't already.
   - Download VEXcode V5 from the [official website](https://www.vexrobotics.com/vexcode).
3. **Connect your VEX V5 robot** to your computer via USB cable or wirelessly (if applicable).
4. **Open the project** in VEXcode V5 and upload it to your robot.
5. **Test the code**: Once the code is uploaded, test the robot in both driver and autonomous modes to ensure everything is functioning as expected.

---

## Usage

1. Power on the robot and the VEX V5 Controller.
2. Switch to autonomous mode on the VEX V5 Robot Brain screen.
3. Start the match and watch the robot execute the autonomous routine.
4. Switch to driver control mode when ready to manually control the robot.

---

## Contributing

Feel free to fork this repository and submit pull requests if you'd like to contribute improvements or fixes. If you encounter bugs or have suggestions for improvements, please open an issue on this repository.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Feel free to adjust this template to match the specifics of your robot’s hardware configuration, sensors, or other requirements. Let me know if you'd like me to help with any other details!
