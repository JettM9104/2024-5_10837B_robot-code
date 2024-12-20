# Code v4, v5
The following is a description and explanation of the v4 and v5 driving and autonomous code of the VIQRC Team 10837B. All the code may be split but are in order by line number unless otherwise listed.

## Code v4
### All files
#### VEXcode Configuration
The following is the VEXcode for both Auton and Driving Code
```cpp
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
#define waitUntil(condition)
  do {                      
    wait(5, msec);
  } while (!(condition))

#define repeat(iterations)
  for (int iterator = 0; iterator < iterations; iterator++)
// END IQ MACROS

```
#### Ports Configuration
The following is still *inside* the VEXcode Configuration, but listed seperately because this was written seperately
```cpp
// Robot configuration code.
inertial BrainInertial = inertial();
controller Controller = controller();
motor ApositiveU = motor(PORT1, true);
motor BpositiveR = motor(PORT2, false);
motor AnegativeD = motor(PORT7, true);
motor bNegativeL = motor(PORT8, false);
motor shooting1 = motor(PORT4, false);
motor shooting2 = motor(PORT10, true);
pneumatic cats = pneumatic(PORT5);
pneumatic dogs = pneumatic(PORT11);
gyro turning = gyro(PORT3);
distance conveyerSensor = distance(PORT9);
distance closerSensor = distance(PORT6);
touchled indicator = touchled(PORT12);
```
#### Continue VEXcode Configuration
A continuation of the VEXcode configuration above.
```cpp
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
//    Author:       Jett Mu                                                 
//    Created:      2024-12-03      
//    Description:  IQ project                                                
//                                                                            
//----------------------------------------------------------------------------

// Include the IQ Library
#include "iq_cpp.h"


// Allows for easier use of the VEX Library
using namespace vex;
```

### Driving Code
#### Functions, Variables, and Namespace Initalization
```cpp
// Functions that will become threads
void run();
void mt();
void pu();
void autoMT();
void macroLED();

// Macros
void rat();
void windPuncher();
void sqbl();

// Others
void init();


// Namespaces for Variable Organization
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
    double k = 0.1;
  }
  namespace bypass {
    bool driving = false; //bypass for driving
    bool shooting = false; //bypass for conveyer-catapult motorshare
    bool pneum1 = false; //bypass for pneumatic
    bool pneum2 = false; // bypass for second pneumatic
    bool pneum3 = false;

  }
  namespace constants {
    int maxMotorSpeed = 100;
  }
  namespace pid {
    double kp = 1;
    double ki = 1;
    double kd = 1;
    double kr = 0.5;
    double kri = 0.1;
  }
  namespace angl {
    double rot;
    double head;
    double limrot;
    double save = 0;
  }
  namespace toggle {
    int pt = 1;
    int ra = 0;

    namespace mt {
      int puncher = 0;
      int lift = 0;
    }
  }
  namespace auton {
    namespace pid {
      double maximum = 1;
    }
  }

  namespace util {
    bool macroOn;
  }
}
```

We also include a namespace tree to incorporate better organization:
```
namespace robot 
├── namespace contr 
│   ├── int a;
│   ├── int b;
│   ├── int c; 
│   └── int d;
├── namespace drivet 
│   ├── double u;
│   ├── double r;
│   ├── double d;
│   ├── double l;
│   └── double k = 0.1;
├── namespace bypass 
│   ├── bool driving = false;
│   ├── bool shooting = false; 
│   ├── bool pneum1 = false;
│   ├── bool pneum2 = false; 
│   └── bool pneum3 = false;
├── namespace constants 
│   └── int maxMotorSpeed = 100;
├── namespace pid 
│   ├── double kp = 1;
│   ├── double ki = 1;
│   ├── double kd = 1;
│   ├── double kr = 0.5;
│   └── double kri = 0.1;
├── namespace angl 
│   ├── double rot;
│   ├── double head;
│   ├── double limrot;
│   └── double save = 0;
├── namespace toggle 
│   ├── int pt = 1;
│   ├── int ra = 0;
│   └── namespace mt 
│       ├── int puncher = 0;
│       └── int lift = 0;
├── namespace auton 
│   └── namespace pid 
│       └── double maximum = 1;
└── namespace util 
    └── bool macroOn;
```

(Note: We did not incorporate this into our auton because we did not have many variables)

#### Main Function and Init Function
Code runs inside the main() function and the init() function is created to organize our code.
```cpp
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  // Threads run while the code is running so we are able to have wait statements
  thread rath = thread(rat);
  thread anglese = thread(run);
  thread mtLift = thread(mt);
  thread puncher = thread(pu);
  thread autom = thread(autoMT);
  thread squeezer = thread(sqbl);
  thread autoload = thread(windPuncher);
  thread led = thread(macroLED);
  
  init();

  while (true) {
    robot::contr::a = Controller.AxisA.position() * -1;
    robot::contr::b = Controller.AxisB.position();
    robot::contr::c = Controller.AxisC.position();
    robot::contr::d = Controller.AxisD.position();

    if (!robot::bypass::driving) {
      // if (robot::contr::c == 0) {robot::contr::c = -(robot::angl::rot - robot::angl::save) * robot::pid::kr; }
      robot::drivet::u = robot::contr::a + robot::contr::b + robot::contr::c;
      robot::drivet::r = robot::contr::a - robot::contr::b - robot::contr::c;
      robot::drivet::d = robot::contr::a - robot::contr::b + robot::contr::c;
      robot::drivet::l = robot::contr::a + robot::contr::b - robot::contr::c;
      
      // robot::auton::pid::maximum = robot::drivet::u > robot::auton::pid::maximum ? robot::drivet::u : robot::auton::pid::maximum;
      // robot::auton::pid::maximum = robot::drivet::r > robot::auton::pid::maximum ? robot::drivet::r : robot::auton::pid::maximum;
      // robot::auton::pid::maximum = robot::drivet::d > robot::auton::pid::maximum ? robot::drivet::d : robot::auton::pid::maximum;
      // robot::auton::pid::maximum = robot::drivet::l > robot::auton::pid::maximum ? robot::drivet::l : robot::auton::pid::maximum;

      // robot::drivet::u /= robot::auton::pid::maximum / 100;
      // robot::drivet::r /= robot::auton::pid::maximum / 100;
      // robot::drivet::d /= robot::auton::pid::maximum / 100;
      // robot::drivet::l /= robot::auton::pid::maximum / 100;

      printf("%f\n", robot::auton::pid::maximum);
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

    if (Controller.AxisC.position() != 0) {robot::angl::save = robot::angl::limrot; }

    if (!robot::bypass::shooting) {
      if (Controller.ButtonLUp.pressing() && !(Controller.ButtonLUp.pressing() && Controller.ButtonLDown.pressing())) {
        shooting1.spin(reverse, 100, percent);
        shooting2.spin(reverse, 100, percent);
      }
      else if (Controller.ButtonLDown.pressing() && !(Controller.ButtonLUp.pressing() && Controller.ButtonLDown.pressing())) {
        shooting1.spin(forward, 100, percent);
        shooting2.spin(forward, 100, percent);
      }
      else if (Controller.ButtonLUp.pressing() && Controller.ButtonLDown.pressing()) {
        Brain.playSound(fillup);
      }
      else {
        shooting1.stop();
        shooting2.stop();
      }
    }

    if (!robot::bypass::pneum1) {
      if (robot::toggle::mt::puncher % 2) {
        cats.extend(cylinder2);
      }
      else {
        cats.retract(cylinder2);
      }
      if (robot::toggle::mt::lift % 2) {
        dogs.retract(cylinder2);
      }
      else {
        dogs.extend(cylinder2);
      }
    }

    if (!robot::bypass::pneum2) {
      if (robot::toggle::pt % 2) {
        dogs.extend(cylinder1);
      }
      else {
        dogs.retract(cylinder1);
      }
    }

    if (!robot::bypass::pneum3) {
      if (robot::toggle::ra % 2) {
        cats.extend(cylinder1);
      }
      else {
        cats.retract(cylinder1);
      }
    }
  }
    wait(20, msec);
}



void init() { // Code in here runs in the beginning
  shooting1.resetPosition();
  shooting2.resetPosition();
  ApositiveU.setMaxTorque(100, percent);
  AnegativeD.setMaxTorque(100, percent);
  BpositiveR.setMaxTorque(100, percent);
  bNegativeL.setMaxTorque(100, percent);
  shooting1.setStopping(hold);
  shooting2.setStopping(hold);
}
```
#### Function Definitions
The code here is what runs while the main() function is running.

```cpp
void run() {
  while (true) {
    
    
    robot::angl::head = BrainInertial.heading(degrees);
    robot::angl::rot = BrainInertial.rotation(degrees);


    if (robot::angl::head > 179.9) {
      robot::angl::limrot = -(360-robot::angl::head);
    }
    else {
      robot::angl::limrot = robot::angl::head;
    }
    wait(20, msec);
  }
}

void mt() {
  while (true) {
    if (Controller.ButtonRUp.pressing()) {
      //pumcher
      robot::toggle::mt::puncher++;
      while (Controller.ButtonRUp.pressing()) {wait(20, msec); }
    }
    if (Controller.ButtonRDown.pressing()) { 
      //lift
      robot::toggle::mt::lift++;
      while (Controller.ButtonRDown.pressing()) {wait(20, msec); }
    }
    wait(20, msec);
  }
}

void pu() {
  // while (true) {
  //   if (Controller.ButtonFUp.pressing()) {
  //     robot::toggle::pt++;
  //     while (Controller.ButtonFUp.pressing()) {wait(20, msec); }
  //   }
  //   wait(20, msec);
  // }
}

void autoMT() {
  bool quit;
  while (true) {
    if (Controller.ButtonEUp.pressing() && !robot::util::macroOn) {
      robot::util::macroOn = true;
      cats.retract(cylinder2);
      dogs.extend(cylinder2);

      while (Controller.ButtonEUp.pressing()) {wait (20, msec); }

      while (true) {
        
        robot::bypass::shooting = true;
        robot::bypass::pneum1 = true;

        shooting1.spin(forward, 100, percent);
        shooting2.spin(forward, 100, percent);
        if (Controller.ButtonEUp.pressing()) {quit = true; break; }
        if (conveyerSensor.objectDistance(mm) < 40) { break; }
        wait(20, msec);
      }
      if (!quit) {
        shooting1.stop();
        shooting2.stop();

        dogs.retract(cylinder2);
        
        wait(2000, msec);

        dogs.extend(cylinder2);
      }
      quit = false;
      robot::bypass::shooting = false;
      robot::bypass::pneum1 = false;
      robot::util::macroOn = false;


    }

    while (Controller.ButtonEUp.pressing()) {wait (20, msec); }
    wait(20, msec);
  }
  
}

void windPuncher() {
  while (true) {
    if (Controller.ButtonEDown.pressing() && !robot::util::macroOn) {
      robot::util::macroOn = true;
      robot::bypass::shooting = true;
      robot::toggle::pt = 0;
      robot::toggle::ra = 0;

      unsigned int tick = 0;
      while (Controller.ButtonEDown.pressing()) {wait(20, msec);}


      while (true){
        printf("%d\n", tick);
        tick++;
        shooting1.spin(forward, 100, percent);
        shooting2.spin(forward, 100, percent);
        printf("%f\n", shooting1.current(amp));

        if (tick > 10) {
          if (shooting1.velocity(percent) < 2) {
            break;
          }
        }
        if (Controller.ButtonEDown.pressing()) {break; }
        wait(20, msec);
      }
      robot::toggle::pt = 1;
      robot::toggle::ra = 1;

      shooting1.spin(reverse, 100, percent);
      shooting2.spin(reverse, 100, percent);

      wait(250, msec);

      shooting1.stop();
      shooting2.stop();

      robot::bypass::shooting = false;
      robot::util::macroOn = false;

    }
    wait(20, msec);
  }
}

void rat() {
  while (true) {
    printf("%d\n", robot::toggle::ra);
    if (Controller.ButtonFDown.pressing()) {
      robot::toggle::ra++;
      while (Controller.ButtonFDown.pressing()) {wait(20, msec); }
    }
    wait(20, msec);
  }
}

void sqbl() {
  bool quit;
  while (true) {
    if (Controller.ButtonFUp.pressing() && !robot::util::macroOn) {

      robot::util::macroOn = true;
      while (Controller.ButtonFUp.pressing()) {wait (20, msec); }

      while (true) {
        
        robot::bypass::shooting = true;


        shooting1.spin(forward, 100, percent);
        shooting2.spin(forward, 100, percent);
        if (Controller.ButtonFUp.pressing()) {quit = true; break; }
        if (closerSensor.objectDistance(mm) < 40) { wait(100, msec); break; }
        wait(20, msec);
      }
      robot::toggle::mt::lift = 1;
      shooting1.stop();
      shooting2.stop();
      quit = false;

      robot::util::macroOn = false;
      robot::bypass::shooting = false;

    }
    while (Controller.ButtonL3.pressing()) {wait (20, msec); }
    wait(20, msec);
  }
  
}

void macroLED() {
  while (true) {
    if (robot::util::macroOn) {
      indicator.setColor(yellow);
    }
    // else if (indicator.pressing()) {
    //   indicator.setColor(orange);
    // }
    else {
      indicator.setColor(blue_green);
    }

    wait(20, msec);
  }
}
```

### Autonomous Code

The following is the autonomous code where the robot drives by itself;

#### Variable and Function Initalization

The following is where is functions and variables are declared.

```cpp
// Global PID Coefficients
double kP_drive = 2, kI_drive = 0.01, kD_drive = 0.3;    // For forward/backward
double kP_strafe = 0.2, kI_strafe = 0.01, kD_strafe = 0.1; // For horizontal movement
double kP_angle_strafe = 1.7, kI_angle_strafe = 0.01, kD_angle_strafe = 1;    // For angular correction
double kP_angle_drive = 4.82, kI_angle_drive = 0.018, kD_angle_drive = 0.6;
double kP_turn = 0.5, kI_turn = 0.012, kD_turn = 1.2;       // For precise turning

// References to access PID Coefficients more easily
double& kpD = kP_drive, kiD = kI_drive, kdD = kD_drive;
double& kpS = kP_strafe, kiS = kI_strafe, kdS = kD_strafe;
double& kpAS = kP_angle_strafe, kiAS = kI_angle_strafe, kdAS = kD_angle_strafe;
double& kpAD = kP_angle_drive, kiAD = kP_angle_drive, kdAD = kD_angle_drive;
double& kpT = kP_turn, kiT = kI_turn, kdT = kD_turn;

// Empty global variable (will be changed in pid() function)
double kP_angle, kI_angle, kD_angle;

// Variables
const double pi = 3.141592;
unsigned short int puncher = 0;

// Motor Functions
void resetAll(); // Resets all the Encoder Positions
double vertEC(); // Returns the average of the Vertical Encoder Positions
double horzEC(); // Returns the average of the Horizontal Encoder Positions
double turnEC();

// Drive Functions
void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed = 90); // Strafe and Drive into one function
void pidTurn(double targetAngle, double timeout, double maxSpeed = 50);

// Macros
void windPuncher();
void shootPuncher();
void squeezeBall();
void ballSwing();

// Init() Function
void init();
```

#### Main and Init Function
The code in the main() function runs after the init() function.
```cpp

int main() {
  // Initialize Robot Configuration
  vexcodeInit();
  init();

  windPuncher();

  for (int i = 5; i > 0; --i) {
    wait(1000, msec);
    printf("%d\n", i);
  }

  Brain.Timer.reset();

  dogs.retract(cylinder2);
  cats.retract(cylinder2);

  pid(5000, 0, 3);

  dogs.extend(cylinder2);
  cats.extend(cylinder2);
  pid(-100, 0, 1);

  pid(0, -1000, 1.3);

  pid(1000, 0, 1.3);
  thread wind0 = thread(windPuncher);
  wait(300, msec);
  pid(-500, 0, 1);
  pid(0, 10000, 2);
  pid(800, 0, 2);

  thread wind1 = thread(windPuncher);

  pid(-2000, 0, 2);

  ballSwing();
  pid(1900, 0, 1.9);

  thread wind2 = thread(windPuncher);

  while (true) {
    pid(-2200, 0, 2);
    pid(0, 4000, 1);
    do {
      if (puncher == 2) {
        shooting1.spin(reverse);
        shooting2.spin(reverse);
      }
      if (puncher == 3) {
        shooting1.stop();
        shooting2.stop();
      }
      wait(20, msec);
    } while (puncher != 3)

    shooting1.stop();
    shooting2.stop();
    
    wait(1500, msec);

    squeezeBall();
    if (Brain.Timer.value() >= 50.0) break;

    shooting1.spin(forward);
    shooting2.spin(forward);
    wait(1000, msec);

    shooting1.stop();
    shooting2.stop();
    pid(10000, 0, 3);
    pid(0, 4000, 1);
    dogs.extend(cylinder2);
    shooting1.spin(forward);
    shooting2.spin(forward);
    wait(1, seconds);
    shooting1.stop();
    shooting2.stop();
  }
  pid(0, -4000, 2);
  pid(2300, 0, 2.2);
  shooting1.spin(forward);
  shooting2.spin(forward);
}


void init() { // Runs in the beginning of the code
  frontLeft.setStopping(hold);
  frontRight.setStopping(hold);
  backRight.setStopping(hold);
  backLeft.setStopping(hold);
  dogs.extend(cylinder2);
  cats.extend(cylinder2);
}
```

#### Macros Definition
When any of the functions below are called, the robot will perform the action asscoated with the function below
```cpp
void windPuncher() { // Winds the puncher
  unsigned int tick = 0;
  puncher = 0;
  cats.retract(cylinder1);
  dogs.extend(cylinder1);
  wait(1000, msec);
  cats.retract(cylinder1);
  dogs.retract(cylinder1);
  

  while (true){
    puncher = 1
    tick++;
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    printf("%f\n", shooting1.current(amp));

    if (tick > 10) {
      if (shooting1.velocity(percent) < 2) {
        break;
      }
    }
    wait(20, msec);
  }
  puncher = 2;
  wait(100, msec);
  shooting1.spin(forward, 100, percent);
  shooting2.spin(forward, 100, percent);
  wait(700, msec);

  cats.extend(cylinder1);
  dogs.extend(cylinder1);

  shooting1.spin(reverse, 100, percent);
  shooting2.spin(reverse, 100, percent);

  wait(1500, msec);
  printf("END\n");
  shooting1.stop();
  shooting2.stop();
  puncher = 3;
}

void ballSwing() { // Automatically lifts the mt
  cats.retract(cylinder2);
  dogs.extend(cylinder2);

  while (true) {
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    if (conveyerSensor.objectDistance(mm) < 40) { break; }
    wait(20, msec);
  }
  shooting1.stop();
  shooting2.stop();

  dogs.retract(cylinder2);
  
  wait(2000, msec);

  dogs.extend(cylinder2);
}

void squeezeBall() {
  while (true) {
    shooting1.spin(forward, 100, percent);
    shooting2.spin(forward, 100, percent);
    if (closerSensor.objectDistance(mm) < 40) { wait(100, msec); break; }
    wait(20, msec);
  }
  dogs.retract(cylinder2);
  shooting1.stop();
  shooting2.stop();
}
```
The following code resets the motor encoder and BrainInertial values.
```cpp
void resetAll() { // Resets all encoder positions
  frontLeft.resetPosition();
  frontRight.resetPosition();
  backLeft.resetPosition();
  backRight.resetPosition();
  BrainInertial.setRotation(0, degrees);
  BrainInertial.setHeading(0, degrees);
}
```


#### Non-void functions
The following functions return something rather than the ``void`` functions. The type they return is all ``double``. 3 is multiplied to the value because the gear ratio *(as of time of code creation)* is ``3:1`` = ``3/1`` = ``3``.
```cpp
double vertEC() { return (frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) + backRight.position(degrees)) / 4.0 * 3.0; }

double horzEC() { return (-frontLeft.position(degrees) + frontRight.position(degrees) + backLeft.position(degrees) - backRight.position(degrees)) / 4.0 * 3.0; }

double turnEC() { return (frontLeft.position(degrees) - frontRight.position(degrees) - backLeft.position(degrees) + backRight.position(degrees)) / 4.0; }
```
#### Void functions with Parameters
The following functions do not return something but have parameters *(i.e. distance to travel)*. They are all functions that move the robot in some way.

```cpp
void pid(double targetVertical, double targetHorizontal, double timeout, double maxSpeed) {
  resetAll();
  unsigned int tick = 0;
  int direction;

  double kP_drive_local = kP_drive;
  double kI_drive_local = kI_drive;
  double kD_drive_local = kD_drive;

  double kP_strafe_local = kP_strafe; 
  double kI_strafe_local = kI_strafe;
  double kD_strafe_local = kD_strafe;

  
  if (targetVertical != 0 && targetHorizontal == 0) {
    kP_angle = kP_angle_drive;
    kI_angle = kI_angle_drive;
    kD_angle = kD_angle_drive;
    
    direction = 0;
  }
  else if (targetVertical == 0 && targetHorizontal != 0) {
    kP_angle = kP_angle_strafe;
    kI_angle = kI_angle_strafe;
    kD_angle = kD_angle_strafe;

    direction = 1;
  }
  else {
    kP_angle = 0;
    kI_angle = 0;
    kD_angle = 0;

    direction = 2;
  }
  double verticalError, prevVerticalError = 0;
  double verticalIntegral = 0, verticalDerivative = 0;

  double horizontalError, prevHorizontalError = 0;
  double horizontalIntegral = 0, horizontalDerivative = 0;

  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;
  
  float bT = Brain.Timer.value();

  bool drive_completed = false;
  bool strafe_completed = false;
  
  while (true) {
    
    tick++;

    double currentVertical = vertEC();
    double currentHorizontal = horzEC();
    verticalError = targetVertical - currentVertical;
    horizontalError = targetHorizontal - currentHorizontal;

    verticalIntegral += verticalError;
    verticalDerivative = verticalError - prevVerticalError;

    horizontalIntegral += horizontalError;
    horizontalDerivative = horizontalError - prevHorizontalError;

    angleError = 0 - BrainInertial.rotation(degrees);

    angleIntegral += angleError;
    angleDerivative = angleError - prevAngleError;

    if (fabs(verticalError) <= 1) verticalIntegral = 0;
    if (fabs(horizontalError) <= 1) horizontalError = 0;
    if (fabs(angleError) <= 1) angleIntegral = 0;
    
    double verticalSpeed = (verticalError * kP_drive_local) + (verticalIntegral * kI_drive_local) +
                           (verticalDerivative * kD_drive_local);
    double horizontalSpeed = ((horizontalError * kP_strafe_local) +
                             (horizontalIntegral * kI_strafe_local) +
                             (horizontalDerivative * kD_strafe_local)) * -1;
    double correctionSpeed = (angleError * kP_angle) +
                             (angleIntegral * kI_angle) +
                             (angleDerivative * kD_angle);

    if (verticalSpeed > maxSpeed) verticalSpeed = maxSpeed;
    if (verticalSpeed < -maxSpeed) verticalSpeed = -maxSpeed;

    if (horizontalSpeed > maxSpeed) horizontalSpeed = maxSpeed;
    if (horizontalSpeed < -maxSpeed) horizontalSpeed = -maxSpeed;

    frontLeft.spin(forward, verticalSpeed + horizontalSpeed + correctionSpeed, pct);
    backLeft.spin(forward, verticalSpeed + horizontalSpeed - correctionSpeed, pct);
    frontRight.spin(forward, verticalSpeed - horizontalSpeed - correctionSpeed, pct);
    backRight.spin(forward, verticalSpeed - horizontalSpeed + correctionSpeed, pct);

    if (fabs(verticalError) < 20 && fabs(horizontalError) < 20 && fabs(angleError) < 3) { printf("break by threshold\n"); break; }

    if (drive_completed && direction == 0) { printf("break by drive"); break; }

    if (strafe_completed && direction == 1) { printf("break by strafe"); break; }

    if (tick > 20) {
      if (fabs(verticalError) < 20) { kP_drive_local = 0, kI_drive_local = 0, kD_drive_local = 0; drive_completed = true; }
      if (fabs(horizontalError) < 20) { kP_strafe_local = 0, kI_strafe_local = 0, kD_strafe_local = 0; strafe_completed = true; }
    }
    if (((Brain.Timer.value() - bT) > timeout) && (timeout != 0)) { printf("break by timeout"); break; }

    prevVerticalError = verticalError;
    prevHorizontalError = horizontalError;
    prevAngleError = angleError;

    wait(20, msec);
  }
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}



void pidTurn(double targetAngle, double timeout, double maxSpeed) {
  resetAll();

  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;
  double maxIntegral = 100; 
  double bT = Brain.Timer.value();
  double targetEncoder = pi * 10 * targetAngle / 7.874 / 3;

  while (true) {
    angleError = targetEncoder - turnEC();
    printf("Angle Error: %f\n", angleError);
    printf("\033[34m");
    printf("%f\n", BrainInertial.rotation(degrees));

    printf("\033[30m");
    angleIntegral += angleError;
    
    if (angleIntegral > maxIntegral) angleIntegral = maxIntegral;
    if (angleIntegral < -maxIntegral) angleIntegral = -maxIntegral;
    
    printf("Angle Integral: %f\n", angleIntegral);

    angleDerivative = angleError - prevAngleError;
    printf("Angle Derivative: %f\n", angleDerivative);

    double turnSpeed = (angleError * kP_turn) + (angleIntegral * kI_turn) +
                       (angleDerivative * kD_turn);
    printf("Turn Speed (before limiting): %f\n", turnSpeed);

    if (turnSpeed > maxSpeed) turnSpeed = maxSpeed;
    if (turnSpeed < -maxSpeed) turnSpeed = -maxSpeed;
    printf("Turn Speed (after limiting): %f\n", turnSpeed);
  
    frontLeft.spin(forward, turnSpeed, pct);
    backLeft.spin(reverse, turnSpeed, pct);
    frontRight.spin(reverse, turnSpeed, pct);
    backRight.spin(forward, turnSpeed, pct);

    if (fabs(angleError) < 2) angleIntegral = 0;
    printf("Angle Integral (after reset if error < 2): %f\n", angleIntegral);

    if (fabs(angleError) < 5) break;
    if ((Brain.Timer.value() - bT) > timeout && timeout != 0) break;

    prevAngleError = angleError;
    printf("Previous Angle Error: %f\n\n\n", prevAngleError);

    wait(20, msec);
  }
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}
```

This concludes the v4 description.

Thank you for https://markdownlivepreview.com/ for allowing us to preview this file before publishing this to GitHub.