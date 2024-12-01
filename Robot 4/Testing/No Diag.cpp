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

int roundn(double roundee, int mult);
void run();
void mt();
void pu();
void autoMT();
void rat();

void init();

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
    int pt = 0;
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
  

}

// U   R
//   X
// L   D


int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  thread rath = thread(rat);
  thread anglese = thread(run);
  thread mtLift = thread(mt);
  thread puncher = thread(pu);
  thread autom = thread(autoMT);
  init();

  while (true) {
    robot::contr::a = roundn(Controller.AxisA.position(), 50);
    robot::contr::b = roundn(Controller.AxisB.position(), 50);
    robot::contr::c = roundn(Controller.AxisC.position(), 50);
    robot::contr::d = roundn(Controller.AxisD.position(), 50);

    if (!robot::bypass::driving) {
      // if (robot::contr::c == 0) {robot::contr::c = -(robot::angl::rot - robot::angl::save) * robot::pid::kr; }

     
      if (abs(robot::contr::a) < 10)
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

      printf("%d\n", robot::toggle::ra);
    }
  }
    wait(20, msec);
}


void init() {
  shooting1.resetPosition();
  shooting2.resetPosition();
  ApositiveU.setMaxTorque(100, percent);
  AnegativeD.setMaxTorque(100, percent);
  BpositiveR.setMaxTorque(100, percent);
  bNegativeL.setMaxTorque(100, percent);
  shooting1.setStopping(hold);
  shooting2.setStopping(hold);
}

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
  while (true) {
    if (Controller.ButtonFUp.pressing()) {
      robot::toggle::pt++;
      while (Controller.ButtonFUp.pressing()) {wait(20, msec); }
    }
    wait(20, msec);
  }
}

void autoMT() {
  bool quit;
  while (true) {
    if (Controller.ButtonEUp.pressing()) {
      robot::bypass::shooting = true;
      robot::bypass::pneum1 = true;
      cats.retract(cylinder2);
      dogs.extend(cylinder2);


      while (true) {
        


        shooting1.spin(forward, 100, percent);
        shooting2.spin(forward, 100, percent);
        if (Controller.ButtonEDown.pressing()) {quit = true; break; }
        if (conveyerSensor.objectDistance(mm) < 40) {break; }
        wait(20, msec);
      }
      if (!quit) {
        wait(40, msec);
        shooting1.stop();
        shooting2.stop();

        dogs.retract(cylinder2);
        
        wait(1500, msec);

        dogs.extend(cylinder2);
      }
      quit = false;
      robot::bypass::shooting = false;
      robot::bypass::pneum1 = false;


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

int round(double roundee, int mult) {
  if ((static_cast<int>(floor(roundee)) % mult) >= (mult/2)) {return ((floor(roundee / mult)) + 1) * mult; }
  else {return ((floor(roundee / mult)) + 1) * mult; }
}
