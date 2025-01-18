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
motor leftDrivetrain = motor(PORT9, true);
motor rightDrivetrain = motor(PORT3, false);
pneumatic jett = pneumatic(PORT1);
pneumatic grayson = pneumatic(PORT7);
motor ptoLeft = motor(PORT6, false);
motor ptoRight = motor(PORT12, true);
motor conveyerLeft = motor(PORT11, false);
motor conveyerRight = motor(PORT5, true);
motor_group conveyer = motor_group(conveyerLeft, conveyerRight);
distance ballDetector = distance(PORT10);

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

// Function Declaration
void init();

// Notes on the code: All units are in mm. 
// Enum for curve function
enum dire { l, r };

// Movement Functions
void drive(double distance, double timeout = 0, directionType dir = forward); // Distance in Units Declared in function, Timeout in Seconds
void turn(double angle, double timeout = 0, directionType dir = forward); // Angle in Degrees, Timeout in Seconds
void curve(double theta, double radius, double timeout = 0, directionType rotation = forward, dire dir = r); 
void slow_drive(double distance, double timeout = 0, directionType dir = forward, bool twoMotor = false);
void slow_turn(double angle, double timeout = 0, directionType dir = forward);

// Macros
void liftMacro();
void shootPuncher();
void windPuncher();

// Namespaces for organization of PID Coefficients
namespace pid
{
    namespace drive { float kP = 0.4, kI = 0.2, kD = 0.3; }
    namespace turn { float kP = 0.35, kI = 0.1, kD = 1.2; }
    namespace correction { float kP = 0.1, kI = 0.1, kD = 0.1; }
    namespace curve { float kP = 0.1, kI = 0.1, kD = 0.1; }
    namespace decelerate { 
      namespace drive { float kP = 0.1, kI = 0.01, kD = 0.5; }
      namespace turn { float kP = 1, kI = 0.01, kD = 0.7; }
    }
}

// Refrences for ease of access of variables
float& dkP = pid::drive::kP, dkI = pid::drive::kI, dkD = pid::drive::kD;
float& tkP = pid::turn::kP, tkI = pid::turn::kI, tkD = pid::turn::kD;
float& ckP = pid::correction::kP, ckI = pid::correction::kI, ckD = pid::correction::kD;
float& ukP = pid::curve::kP, ukI = pid::curve::kI, ukD = pid::curve::kD;
float& ekP = pid::decelerate::drive::kP, ekI = pid::decelerate::drive::kI, ekD = pid::decelerate::drive::kD;
float& fkP = pid::decelerate::turn::kP, fkI = pid::decelerate::turn::kI, fkD = pid::decelerate::turn::kD;

// PIEEEEEEEEEEEE!!
const double pi = 3.1415926;

int main() {
  vexcodeInit();
  init();

  windPuncher();
  ptoLeft.spin(forward, 20, percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2); 


  wait(500, msec);
  ptoLeft.spin(reverse, 20, percent);
  ptoRight.spin(reverse, 20, percent);

  wait(200, msec);
  
  jett.extend(cylinder2);
  wait(500, msec);

  ptoLeft.stop();
  ptoRight.stop();


  jett.retract(cylinder2); 

  windPuncher();

  jett.extend(cylinder2);

  wait(2, seconds);

  drive(-3200);

  turn(-90);

  slow_drive(3700, 3);

  wait(500, msec);

  ptoLeft.spin(forward, 20,percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2);

  wait(800, msec);

  jett.retract(cylinder2);

  slow_drive(500, 0.75, forward, true);


  ptoLeft.spin(forward, 20,percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2);

  wait(1000, msec);

  ptoLeft.stop();
  ptoRight.stop();

  shootPuncher();

  wait(900, msec);

  conveyer.spin(forward);
  ptoLeft.spin(forward);
  ptoRight.spin(forward);

  wait(4, seconds);

  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();

  windPuncher();
  ptoLeft.spin(forward, 20, percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2); 


  wait(500, msec);
  ptoLeft.spin(reverse, 20, percent);
  ptoRight.spin(reverse, 20, percent);

  wait(200, msec);
  
  jett.extend(cylinder2);
  wait(500, msec);

  ptoLeft.stop();
  ptoRight.stop();


  jett.retract(cylinder2); 

  windPuncher();

  jett.extend(cylinder2);

  wait(2, seconds);

  drive(-3200);

  turn(90);

  slow_drive(3700, 3);

  wait(500, msec);

  ptoLeft.spin(forward, 20,percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2);

  wait(800, msec);

  jett.retract(cylinder2);

  slow_drive(500, 0.75, forward, true);


  ptoLeft.spin(forward, 20,percent);
  ptoRight.spin(forward, 20, percent);

  jett.retract(cylinder2);

  wait(1000, msec);

  ptoLeft.stop();
  ptoRight.stop();

  shootPuncher();

  wait(900, msec);

  conveyer.spin(forward);
  ptoLeft.spin(forward);
  ptoRight.spin(forward);

  wait(4, seconds);

  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();
}

void init() {
  // initalize stuff here, for example, setstopping to hold, coast, or brake
  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  jett.pumpOn();
  rightDrivetrain.setStopping(hold);
  leftDrivetrain.setStopping(hold);
  ptoLeft.setStopping(hold);
  ptoRight.setStopping(hold);
  ptoLeft.setVelocity(100, percent);
  ptoRight.setVelocity(100, percent);
  conveyer.setVelocity(100, percent);
  ptoLeft.setMaxTorque(100, percent);
  ptoRight.setMaxTorque(100, percent);
  conveyer.setMaxTorque(100, percent);

}

void drive(double distance, double timeout, directionType dir) { // Drive Function
  // Direction Parameter
  if (dir == reverse) { distance *= 1; }

  // Coefficients for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double leftSpeed, rightSpeed;

  // Coefficients for PID Correcion System
  double correctionError, correctionIntegral = 0, correctionDerivative, correctionLastError = 0;
  double correctionFactor;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();
  double beginInertial = BrainInertial.value();

  // Wheel Distance Calculation
  double wheelCircum = 200; // The units of double "distance" will be the same as wheelCircum
  double gearRatio = 2 / 1;
  double goalDegrees =  distance * (wheelCircum / 360) / gearRatio;

  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  // Iteration Count
  unsigned int i = 0;

  while (true) {
    // Increment the Iteration
    i++;

    // Calculate PID Values
    error = goalDegrees - ((leftDrivetrain.position(degrees) + rightDrivetrain.position(degrees)) / 2);
    integral = error < integralResetZone ? 0 : integral + error;
    derivative = error - lastError;

    if (fabs(error) < 3) { integral = 0; } // Reset integral when target is almost met

    // Correction Calculation
    correctionError = (rightDrivetrain.position(degrees) - leftDrivetrain.position(degrees)) + (BrainInertial.rotation(degrees) - beginInertial);
    correctionIntegral += correctionError;
    correctionDerivative = correctionError - correctionLastError;

    // Calculate Correction Factor
    correctionFactor = (correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD);
    
    // Calculate Motor Speed
    leftSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkD));
    rightSpeed = ((error * dkP) + (integral * dkI) + (derivative * dkD));

    // Spin Motors
    leftDrivetrain.spin(forward, leftSpeed, percent);
    ptoLeft.spin(forward, leftSpeed, percent);
    rightDrivetrain.spin(forward, rightSpeed, percent);
    ptoRight.spin(forward, rightSpeed, percent);

    // Exit Conditions
    if (fabs(error) < threshold && i >= 10) [[unlikely]] { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }

    // Set lastError Values
    correctionLastError = correctionError;
    lastError = error;

    wait(20, msec); // Wait so the brain doesnt explode
  }
  rightDrivetrain.stop();
  leftDrivetrain.stop();
  ptoRight.stop();
  ptoLeft.stop();
}

void turn(double angle, double timeout, directionType dir) {
  // Direction Parameter
  //if (dir == reverse) { angle *= 1; }

  // Variables for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double motorSpeed;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();
  double beginInertial = BrainInertial.rotation(degrees);

  // Wheel Distance Calculation
  double wheelCircum = 200;
  double gearRatio = 2 / 1;
  double trackDiam = 200; // Must be same units as wheelCircum
  double goalDegrees =  (angle / 360) * 4 * pi * trackDiam * wheelCircum / 360 / gearRatio;

  printf("GOAL DEGREES EQN %f\n", goalDegrees);
  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  while (true) {
    // Calculate PID Values
    error = (goalDegrees - (leftDrivetrain.position(degrees) - rightDrivetrain.position(degrees)) / 2) + (angle - BrainInertial.rotation(degrees) + beginInertial) / 2;
    integral = error < integralResetZone ? 0 : integral + error;

    printf("%f\n", error);
    derivative = error - lastError;

    if (fabs(error) < 3) [[unlikely]] { integral = 0; } // Reset integral when target is almost met

    // Calculate Motor Speed
    motorSpeed = ((error * tkP) + (integral * tkI) + (derivative * tkD));

    // Spin Motors
    leftDrivetrain.spin(forward, motorSpeed, percent);
    rightDrivetrain.spin(reverse, motorSpeed, percent);
    ptoLeft.spin(forward, motorSpeed, percent);
    ptoRight.spin(reverse, motorSpeed , percent);

    // Exit Conditions
    if (fabs(error) < threshold) [[unlikely]] { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }

    // Set lastError Variable
    lastError = error;
    printf("DERIV %f\n", tkD);

    wait(20, msec); // Wait so the brain doesnt explode
  }
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  ptoLeft.stop();
  ptoRight.stop();
}

void curve(double theta, double radius, double timeout, directionType rotation, dire dir) {
  // Direction Parameter
  if (dir == l) { radius *= -1; }
  if (rotation == reverse) { theta *= -1; }

  // Coefficients for PID Drive System
  double Lerror, Lintegral = 0, Lderivative;
  double LlastError = 0;
  double LmotorSpeed;

  double Rerror, Rintegral = 0, Rderivative;
  double RlastError = 0;
  double RmotorSpeed;

  double threshold = 10, integralResetZone = 3;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();

  // Wheel Distance Calculation
  double wheelCircum = 200;
  double gearRatio = 2 / 1;
  double trackWidth = 254; // Must be same units as wheelCircum
  double Lgoal = (theta / 360) * (pi * 2 * (radius - (trackWidth / 2))) * 360 / wheelCircum / gearRatio; // Refrencing Formula D_{L} = \frac{\Theta}{360}\times \left ( 2\times \pi \times \left ( r-\frac{\textrm{wheelbase}}{2} \right ) \right )
  double Rgoal = (theta / 360) * (pi * 2 * (radius + (trackWidth / 2))) * 360 / wheelCircum / gearRatio; 

  // Making sure the values will max out at different values so the robot will not drive straight
  double Lspeed;
  double Rspeed;
  
  if (Lgoal < Rgoal) {
    Lspeed = Rgoal / Lgoal * 100;
    Rspeed = 100;
  }
  else {
    Rspeed = 100;
    Lspeed = Lgoal / Rgoal * 100;
  }
 // printf("the max speed for right is %f and \nthe max speed for left is %f.", Lspeed, Rspeed);

  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  while (true) {
    // Calculate PID Values
    Lerror = Lgoal - (leftDrivetrain.position(degrees));
    Lintegral = Lerror < integralResetZone ? 0 : Lintegral + Lerror;
    Lderivative = Lerror - LlastError;

    Rerror = Rgoal - (rightDrivetrain.position(degrees));
    Rintegral = Rerror < integralResetZone ? 0 : Rintegral + Rerror;
    Rderivative = Rerror - RlastError;
    printf("r error: %f\n", Rerror);

    if (fabs(Lerror) < 3) { Lintegral = 0; } // Reset integral when target is almost met
    if (fabs(Rerror) < 3) { Rintegral = 0; }

    // Calculate Motor Speed
    LmotorSpeed = ((Lerror * ukP) + (Lintegral * ukI) + (Lderivative * ukD)) > 50 ? 50 : ((Lerror * ukP) + (Lintegral * ukI) + (Lderivative * ukD));
    RmotorSpeed = ((Rerror * ukP) + (Rintegral * ukI) + (Rderivative * ukD)) > 100 ? 100 : ((Rerror * ukP) + (Rintegral * ukI) + (Rderivative * ukD));

    printf("L motor speed: %f\nR motor speed: %f\n\n", LmotorSpeed, RmotorSpeed);
    // Spin Motors
    leftDrivetrain.spin(forward, LmotorSpeed, percent);
    ptoLeft.spin(forward, LmotorSpeed, percent);
    rightDrivetrain.spin(forward, RmotorSpeed, percent);
    ptoRight.spin(forward, RmotorSpeed, percent);

    // Exit Conditions
    if ((fabs(Lerror) < threshold) && (fabs(Rerror) < threshold)) [[unlikely]] { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }

    // Set lastError Variable
    LlastError = Lerror;
    RlastError = Rerror;

    wait(20, msec); // Wait so the brain doesnt explod
  }
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  ptoLeft.stop();
  ptoRight.stop();
}

void slow_drive(double distance, double timeout, directionType dir, bool twoMotor) { // Drive Function
  // Direction Parameter
  if (dir == reverse) { distance *= 1; }

  // Coefficients for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double leftSpeed, rightSpeed;

  // Coefficients for PID Correcion System
  double correctionError, correctionIntegral = 0, correctionDerivative, correctionLastError = 0;
  double correctionFactor;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();
  double beginInertial = BrainInertial.value();

  // Wheel Distance Calculation
  double wheelCircum = 200; // The units of double "distance" will be the same as wheelCircum
  double gearRatio = 2 / 1;
  double goalDegrees =  distance * (wheelCircum / 360) / gearRatio;

  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  // Iteration Count
  unsigned int i = 0;

  while (true) {
    // Increment the Iteration
    i++;

    // Calculate PID Values
    error = goalDegrees - ((leftDrivetrain.position(degrees) + rightDrivetrain.position(degrees)) / 2);
    integral = error < integralResetZone ? 0 : integral + error;
    derivative = error - lastError;

    if (fabs(error) < 3) { integral = 0; } // Reset integral when target is almost met

    // Correction Calculation
    correctionError = (rightDrivetrain.position(degrees) - leftDrivetrain.position(degrees)) + (BrainInertial.rotation(degrees) - beginInertial);
    correctionIntegral += correctionError;
    correctionDerivative = correctionError - correctionLastError;

    // Calculate Correction Factor
    correctionFactor = (correctionError * ckP) + (correctionIntegral * ckI) + (correctionDerivative * ckD);
    
    // Calculate Motor Speed
    leftSpeed = ((error * ekP) + (integral * ekI) + (derivative * ekD));
    rightSpeed = ((error * ekP) + (integral * ekI) + (derivative * ekD));

    // Spin Motors
    leftDrivetrain.spin(forward, leftSpeed, percent);
    if (!twoMotor) { ptoLeft.spin(forward, leftSpeed, percent); ptoRight.spin(forward, rightSpeed, percent); }
    rightDrivetrain.spin(forward, leftSpeed, percent);
    

    // Exit Conditions
    if (fabs(error) < threshold && i >= 10) [[unlikely]] { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }

    // Set lastError Values
    correctionLastError = correctionError;
    lastError = error;

    wait(20, msec); // Wait so the brain doesnt explode
  }
  rightDrivetrain.stop();
  leftDrivetrain.stop();
  ptoRight.stop();
  ptoLeft.stop();
}

void slow_turn(double angle, double timeout, directionType dir) {
  // Direction Parameter
  //if (dir == reverse) { angle *= 1; }

  // Variables for PID Drive System
  double threshold = 5, integralResetZone = 3;
  double error, integral = 0, derivative;
  double lastError = 0;
  double motorSpeed;

  // Saving the values so we don't have to reset the values in the beginning
  double beginTimer = Brain.Timer.value();

  // Wheel Distance Calculation
  double wheelCircum = 200;
  double gearRatio = 2 / 1;
  double trackDiam = 200; // Must be same units as wheelCircum
  double goalDegrees =  (angle / 360) * 4 * pi * trackDiam * wheelCircum / 360 / gearRatio;

  printf("GOAL DEGREES EQN %f\n", goalDegrees);
  // Reset Motor Encoder Positions
  leftDrivetrain.resetPosition();
  rightDrivetrain.resetPosition();

  while (true) {
    // Calculate PID Values
    error = (goalDegrees - (leftDrivetrain.position(degrees) - rightDrivetrain.position(degrees)) / 2);// + (angle - BrainInertial.rotation(degrees) + beginInertial)) / 2;
    integral = error < integralResetZone ? 0 : integral + error;

    printf("%f\n", error);
    derivative = error - lastError;

    if (fabs(error) < 3) [[unlikely]] { integral = 0; } // Reset integral when target is almost met

    // Calculate Motor Speed
    motorSpeed = ((error * fkP) + (integral * fkI) + (derivative * fkD));

    // Spin Motors
    leftDrivetrain.spin(forward, motorSpeed, percent);
    rightDrivetrain.spin(reverse, motorSpeed, percent);
    ptoLeft.spin(forward, motorSpeed, percent);
    ptoRight.spin(reverse, motorSpeed , percent);

    // Exit Conditions
    if (fabs(error) < threshold) [[unlikely]] { break; }
    if ((((Brain.Timer.value()) - beginTimer) > timeout) && (timeout != 0)) [[unlikely]] { break; }

    // Set lastError Variable
    lastError = error;
    printf("DERIV %f\n", tkD);

    wait(20, msec); // Wait so the brain doesnt explode
  }
  leftDrivetrain.stop();
  rightDrivetrain.stop();
  ptoLeft.stop();
  ptoRight.stop();
}
void liftMacro() {

  conveyer.spin(forward, 100, percent);

  wait(200, msec);

  
  conveyer.stop(); 
  ptoLeft.stop();
  ptoRight.stop(); 
  grayson.extend(cylinder1);
  wait(2, seconds);
  grayson.retract(cylinder1);

}

void shootPuncher() {
  grayson.extend(cylinder2);


  jett.retract(cylinder1);

  jett.retract(cylinder2);
  


  conveyer.spin(forward, 100, percent);
  ptoLeft.spin(forward, 100, percent);
  ptoRight.spin(forward, 100, percent);
  wait(500, msec);
  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();

  wait(200, msec);
  grayson.retract(cylinder2);


}

void windPuncher() {
  grayson.extend(cylinder2);


  jett.extend(cylinder1);
  
  jett.retract(cylinder2);


  
  unsigned int x = 0;
  do {
    conveyer.spin(forward, 100, percent);
    ptoLeft.spin(forward, 100, percent);
    ptoRight.spin(forward, 100, percent);
    x++;
    wait(20, msec);
    printf("b\n");
  } while ((conveyerLeft.velocity(percent) > 15) || (x < 50));
  conveyer.stop();
  ptoLeft.stop();
  ptoRight.stop();
  printf("c\n");

  grayson.retract(cylinder2);


}

