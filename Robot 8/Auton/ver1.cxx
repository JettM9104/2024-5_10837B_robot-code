
#include "vex.h"

controller Controller = controller();
class motor_group_group {
private:
  motor_group motora;
  motor_group motorb;

public:
  motor_group_group(motor_group motora, motor_group motorb): motora(motora), motorb(motorb) {}

  void spin(directionType dir, int32_t velocity, percentUnits units) {
    motora.spin(dir, velocity, units);
    motorb.spin(dir, velocity, units);
  }
  void spin(directionType dir) {
    motora.spin(dir);
    motorb.spin(dir);
  }
  void stop() {
    motora.stop();
    motorb.stop();
  }
};


// Robot configuration code.
inertial BrainInertial = inertial();
distance loadingZone = distance(PORT6);
distance chassis = distance(PORT10);
motor leftUpIntake = motor(PORT5, false);
motor leftDownIntake = motor(PORT4, false);
motor_group leftIntake = motor_group(leftUpIntake, leftDownIntake);
motor rightUpIntake = motor(PORT12, true);
motor rightDownIntake = motor(PORT11, true);
motor_group rightIntake = motor_group(rightUpIntake, rightDownIntake);
motor_group_group intake = motor_group_group(leftIntake, rightIntake);
motor leftDrive = motor(PORT3, true);
motor rightDrive = motor(PORT9, false);