void updateDrive() {
  leftDrivetrain.spin(forward, Controller.AxisA.position() - Controller.AxisC.position(), percent); 
  rightDrivetrain.spin(forward, Controller.AxisA.position() + Controller.AxisC.position(), percent);
}

int main() {
  Controller.AxisA.changed(updateDrive);
  Controller.AxisC.changed(updateDrive);
}
