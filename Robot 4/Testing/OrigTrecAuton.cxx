#include "vex.h"

using namespace vex;

// Motor Configuration
motor frontLeft(PORT1, ratio18_1, false);
motor frontRight(PORT2, ratio18_1, true);
motor backLeft(PORT3, ratio18_1, false);
motor backRight(PORT4, ratio18_1, true);

// Sensor Configuration
gyro gyroSensor = gyro(Brain.ThreeWirePort.A);

// PID Constants
double kP_drive = 0.2, kI_drive = 0.01, kD_drive = 0.1;   // For forward/backward
double kP_strafe = 0.2, kI_strafe = 0.01, kD_strafe = 0.1; // For horizontal movement
double kP_angle = 0.5, kI_angle = 0.02, kD_angle = 0.2;   // For angular correction
double kP_turn = 0.6, kI_turn = 0.03, kD_turn = 0.4;      // For precise turning

// Reset Motor Rotations and Gyro
void resetSensors() {
  frontLeft.resetRotation();
  frontRight.resetRotation();
  backLeft.resetRotation();
  backRight.resetRotation();
  gyroSensor.resetRotation();
}

// Calculate Average Encoder Position
double verticalEncoderPosition() {
  return (frontLeft.rotation(degrees) + frontRight.rotation(degrees) +
          backLeft.rotation(degrees) + backRight.rotation(degrees)) /
         4.0 * 3.0; // Adjust for 1:3 gear ratio
}

double horizontalEncoderPosition() {
  return (-frontLeft.rotation(degrees) + frontRight.rotation(degrees) +
          backLeft.rotation(degrees) - backRight.rotation(degrees)) /
         4.0 * 3.0; // Adjust for 1:3 gear ratio
}

// Dual-Axis PID Function
void PIDDriveStrafe(double targetVertical, double targetHorizontal, double maxSpeed = 90) {
  resetSensors();

  // PID Variables for Vertical Movement
  double verticalError, prevVerticalError = 0;
  double verticalIntegral = 0, verticalDerivative = 0;

  // PID Variables for Horizontal Movement
  double horizontalError, prevHorizontalError = 0;
  double horizontalIntegral = 0, horizontalDerivative = 0;

  // PID Variables for Angle Correction
  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;

  while (true) {
    // Calculate Vertical and Horizontal Errors
    double currentVertical = verticalEncoderPosition();
    double currentHorizontal = horizontalEncoderPosition();
    verticalError = targetVertical - currentVertical;
    horizontalError = targetHorizontal - currentHorizontal;

    // Calculate Integral and Derivative for Vertical
    verticalIntegral += verticalError;
    verticalDerivative = verticalError - prevVerticalError;

    // Calculate Integral and Derivative for Horizontal
    horizontalIntegral += horizontalError;
    horizontalDerivative = horizontalError - prevHorizontalError;

    // Calculate Angle Error from Gyro
    angleError = 0 - gyroSensor.rotation(degrees);

    // Integral and Derivative for Angle
    angleIntegral += angleError;
    angleDerivative = angleError - prevAngleError;

    // Calculate Speeds
    double verticalSpeed = (verticalError * kP_drive) + (verticalIntegral * kI_drive) +
                           (verticalDerivative * kD_drive);
    double horizontalSpeed = (horizontalError * kP_strafe) +
                             (horizontalIntegral * kI_strafe) +
                             (horizontalDerivative * kD_strafe);
    double correctionSpeed = (angleError * kP_angle) +
                             (angleIntegral * kI_angle) +
                             (angleDerivative * kD_angle);

    // Limit Speeds
    if (verticalSpeed > maxSpeed) verticalSpeed = maxSpeed;
    if (verticalSpeed < -maxSpeed) verticalSpeed = -maxSpeed;

    if (horizontalSpeed > maxSpeed) horizontalSpeed = maxSpeed;
    if (horizontalSpeed < -maxSpeed) horizontalSpeed = -maxSpeed;

    // Apply Motor Speeds for Dual-Axis Correction
    frontLeft.spin(forward, verticalSpeed + horizontalSpeed + correctionSpeed, pct);
    backLeft.spin(forward, verticalSpeed - horizontalSpeed + correctionSpeed, pct);
    frontRight.spin(forward, verticalSpeed - horizontalSpeed - correctionSpeed, pct);
    backRight.spin(forward, verticalSpeed + horizontalSpeed - correctionSpeed, pct);

    // Exit Condition
    if (fabs(verticalError) < 5 && fabs(horizontalError) < 5 && fabs(angleError) < 1) break;

    // Update Previous Errors
    prevVerticalError = verticalError;
    prevHorizontalError = horizontalError;
    prevAngleError = angleError;

    wait(20, msec);
  }

  // Stop Motors
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

// PID Turn Function
void PIDTurn(double targetAngle, double maxSpeed = 50) {
  resetSensors();

  // PID Variables for Turning
  double angleError, prevAngleError = 0;
  double angleIntegral = 0, angleDerivative = 0;

  while (true) {
    // Calculate Angle Error
    angleError = targetAngle - gyroSensor.rotation(degrees);

    // Integral and Derivative for Angle
    angleIntegral += angleError;
    angleDerivative = angleError - prevAngleError;

    // Calculate Turning Speed
    double turnSpeed = (angleError * kP_turn) + (angleIntegral * kI_turn) +
                       (angleDerivative * kD_turn);

    // Limit Speed
    if (turnSpeed > maxSpeed) turnSpeed = maxSpeed;
    if (turnSpeed < -maxSpeed) turnSpeed = -maxSpeed;

    // Apply Motor Speeds for Turning
    frontLeft.spin(forward, turnSpeed, pct);
    backLeft.spin(forward, turnSpeed, pct);
    frontRight.spin(reverse, turnSpeed, pct);
    backRight.spin(reverse, turnSpeed, pct);

    // Exit Condition
    if (fabs(angleError) < 1) break;

    // Update Previous Error
    prevAngleError = angleError;

    wait(20, msec);
  }

  // Stop Motors
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}

// Main Function
int main() {
  // Initialize Robot Configuration
  vexcodeInit();

  // Calibrate Gyro
  gyroSensor.startCalibration();
  wait(2000, msec); // Wait for calibration to complete

  // Example Usage
  PIDDriveStrafe(500, 300); // Move 500 units forward and 300 units right
  PIDTurn(90);              // Turn 90 degrees clockwise
  PIDDriveStrafe(300, -200); // Move 300 units forward and 200 units left
  PIDTurn(-90);             // Turn 90 degrees counterclockwise
}
