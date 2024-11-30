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
motor BnegativeL = motor(PORT8, false);
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


// Declaration of Driving Functions
void drive(double dist, double k, double kp, double ki, double kd, double timeout);

void run(); // The code in this function run while the main code is running
void init();
void liftMacro();

// Namespaces for variable organization
namespace robot {
  namespace command {
    double a; //forwards backwards
    double b; //left right
    double c; //turning
    double d;
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
    double pi = 3.141592;

    namespace pid {
      double kp = 1;
      double ki = 1;
      double kd = 1;
    }
  }

  namespace angl {
    double rot;
    double head;
    double limrot;
  }
  
  namespace auton {
    namespace pid {
      double threshold = 5,  integralResetZone = 3,  maxSpeed = 100;
      double maximum = 1;
      double dirRad;
      double correctionK = 2;
      int iteration = 0;
      double correction;
      double fCorrection;

      double hRot = 0;

      double max;

      namespace r {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
      namespace l {
        double error = 0, integral = 0, derivative = 0;
        double lastError = 0;
        double motorSpeed;
      }
    }
    namespace correction {
      double u;
      double r;
      double d;
      double l;
    }
  }
}
void drive(double dist, double k, double kp, double ki, double kd, double timeout);

// Main Function (Where code is run)
int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!

  vexcodeInit();

  init();

  thread direction = thread(run);

  for (int i = 3; i >= 0; i--) {
    wait(1, seconds);
    printf("%d\n", i);
    
  }
  
  BrainInertial.setHeading(0,degrees);
  //thread lift = thread(liftMacro);
  drive(1000,100,0.4,0.1,1,0);
}

// Defenition of run()
void run() {
  while (true) {
    robot::angl::head = BrainInertial.heading(degrees);
    robot::angl::limrot = (robot::angl::head > 179.9) ? -(360 - robot::angl::head) : robot::angl::head;
    wait(20, msec);
  }
}

void init() {
  ApositiveU.setStopping(hold);
  BpositiveR.setStopping(hold);
  AnegativeD.setStopping(hold);
  BnegativeL.setStopping(hold);
  dogs.retract(cylinder1);
  BrainInertial.setRotation(0, degrees);
  BrainInertial.setHeading(0, degrees);

  printf("\033[2J\n");
}
// Definiton of drive()
void drive(double dist, double k, double kp, double ki, double kd, double timeout) {
  double startValue = Brain.Timer.value();

  while (true) {
    robot::auton::pid::l::error = dist - BnegativeL.position(degrees);
    robot::auton::pid::r::error = dist - BpositiveR.position(degrees);

    // robot::auton::pid::correction = BnegativeL.position(degrees) - BpositiveR.position(degrees);

    robot::auton::pid::correction = BrainInertial.rotation(degrees) - robot::auton::pid::hRot;


    robot::auton::pid::fCorrection = robot::auton::pid::correction * -1 * robot::auton::pid::correctionK;


    robot::auton::pid::l::integral = robot::auton::pid::l::error < robot::auton::pid::integralResetZone ? 0 : robot::auton::pid::l::error;
    robot::auton::pid::r::integral = robot::auton::pid::r::error < robot::auton::pid::integralResetZone ? 0 : robot::auton::pid::l::error;

    robot::auton::pid::l::derivative = robot::auton::pid::l::error - robot::auton::pid::l::lastError;
    robot::auton::pid::r::derivative = robot::auton::pid::r::error - robot::auton::pid::r::lastError;
    
    robot::auton::pid::l::motorSpeed = robot::auton::pid::l::error * kp + robot::auton::pid::l::integral * ki + robot::auton::pid::l::derivative * kd;
    robot::auton::pid::r::motorSpeed = robot::auton::pid::r::error * kp + robot::auton::pid::r::integral * ki + robot::auton::pid::r::derivative * kd;

    // robot::auton::pid::l::motorSpeed += robot::auton::pid::fCorrection;
    // robot::auton::pid::r::motorSpeed -= robot::auton::pid::fCorrection;

    robot::auton::correction::u = robot::auton::pid::l::motorSpeed + robot::auton::pid::fCorrection;
    robot::auton::correction::r = robot::auton::pid::l::motorSpeed - robot::auton::pid::fCorrection;
    robot::auton::correction::d = robot::auton::pid::l::motorSpeed - robot::auton::pid::fCorrection;
    robot::auton::correction::l = robot::auton::pid::l::motorSpeed + robot::auton::pid::fCorrection;

    robot::auton::pid::max = robot::auton::correction::u > robot::auton::pid::max ? robot::auton::correction::u : robot::auton::pid::max;
    robot::auton::pid::max = robot::auton::correction::r > robot::auton::pid::max ? robot::auton::correction::r : robot::auton::pid::max;
    robot::auton::pid::max = robot::auton::correction::d > robot::auton::pid::max ? robot::auton::correction::d : robot::auton::pid::max;
    robot::auton::pid::max = robot::auton::correction::l > robot::auton::pid::max ? robot::auton::correction::l : robot::auton::pid::max;

    robot::auton::correction::u /= robot::auton::pid::maxSpeed / robot::auton::pid::max;
    robot::auton::correction::r /= robot::auton::pid::maxSpeed / robot::auton::pid::max;
    robot::auton::correction::d /= robot::auton::pid::maxSpeed / robot::auton::pid::max;
    robot::auton::correction::l /= robot::auton::pid::maxSpeed / robot::auton::pid::max;

    ApositiveU.spin(forward, robot::auton::correction::u, percent);
    BnegativeL.spin(forward, robot::auton::correction::r, percent);
    
    BpositiveR.spin(forward, robot::auton::correction::d, percent);
    AnegativeD.spin(forward, robot::auton::correction::l, percent);

    robot::auton::pid::l::lastError = robot::auton::pid::l::error;
    robot::auton::pid::r::lastError = robot::auton::pid::r::error;
    if (fabs(robot::auton::pid::l::error) < robot::auton::pid::threshold) {break; }
    if (((Brain.Timer.value() - startValue) > timeout) && timeout != 0) {break; }
  }
  ApositiveU.stop();
  BpositiveR.stop();
  AnegativeD.stop();
  BnegativeL.stop();
}

// Definition of turn()
void turn(double target, double kp, double ki, double kd, double timeout) {

  robot::auton::pid::hRot += target;

  double error = 0, lastError = 0, integral = 0, derivative = 0;
  double threshold = 2.5;
  double maxIntegral = 50;
  double integralResetZone = 3;
  int maxSpeed = 100;

  while (true) {
    error = target - robot::angl::limrot;
    derivative = error-lastError;
    if (fabs(error) < threshold) {
      ApositiveU.stop();
      BpositiveR.stop();
      AnegativeD.stop();
      BnegativeL.stop();
      break; 
    }
  
    if (fabs(error) < integralResetZone) {integral += error; } else {integral = 0; }

    if (integral > maxIntegral) {integral = maxIntegral; }
    if (integral < -maxIntegral) {integral = -maxIntegral; }

    double motorSpeed = (kp * error) + (ki * integral) + (kd * derivative);

    if (motorSpeed > maxSpeed) {motorSpeed = maxSpeed; }
    if (motorSpeed < -maxSpeed) {motorSpeed = -maxSpeed; }

    ApositiveU.spin(forward, motorSpeed, percent);
    BpositiveR.spin(forward, motorSpeed, percent);
    AnegativeD.spin(forward, motorSpeed, percent);
    BnegativeL.spin(forward, motorSpeed, percent);
    lastError = error;
  }
}

void liftMacro() {
  cats.retract(cylinder2);
  dogs.extend(cylinder2);
  shooting1.spin(forward, 100, percent);
  shooting2.spin(forward, 100, percent);
  while (!(conveyerSensor.objectDistance(mm) < 40)) {wait(20, msec); }

  shooting1.stop();
  shooting2.stop();
  dogs.retract(cylinder2);
    
  wait(1500, msec);

  dogs.extend(cylinder2);

}

