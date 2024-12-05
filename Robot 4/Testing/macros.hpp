class macros {
    private:
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
    public:
        void windPuncher() {
            unsigned int tick = 0;
            cats.retract(cylinder1);
            dogs.extend(cylinder1);
            wait(2000, msec);
            cats.retract(cylinder1);
            dogs.retract(cylinder1);
            

            while (true){
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
            wait(100, msec);
            shooting1.spin(forward, 100, percent);
            shooting2.spin(forward, 100, percent);
            wait(700, msec);

            cats.extend(cylinder1);
            dogs.extend(cylinder1);

            shooting1.spin(reverse, 100, percent);
            shooting2.spin(reverse, 100, percent);

            wait(200, msec);
            
            shooting1.stop();
            shooting2.stop();
        }

        void autoMT() {
            bool quit;
            while (true) {
                if (Controller.ButtonEUp.pressing()) {
                cats.retract(cylinder2);
                dogs.extend(cylinder2);


                while (true) {
                    
                    robot::bypass::shooting = true;
                    robot::bypass::pneum1 = true;

                    shooting1.spin(forward, 100, percent);
                    shooting2.spin(forward, 100, percent);
                    if (Controller.ButtonEDown.pressing()) {quit = true; break; }
                    if (conveyerSensor.objectDistance(mm) < 40) {break; }
                    wait(20, msec);
                }
                if (!quit) {
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

        void twoBallz() {
            bool quit;
            while (true) {
                if (Controller.ButtonEUp.pressing()) {
                    while (true) {
                        
                        robot::bypass::shooting = true;
                        robot::bypass::pneum1 = true;

                        shooting1.spin(forward, 100, percent);
                        shooting2.spin(forward, 100, percent);
                        if (Controller.ButtonEDown.pressing()) {quit = true; break; }
                        if (conveyerSensor.objectDistance(mm) < 40) {break; }
                        wait(20, msec);
                    }
                    if (!quit) {
                        shooting1.stop();
                        shooting2.stop();

                        dogs.retract(cylinder2);
                        
                        wait(1500, msec);

                        dogs.extend(cylinder2);
                    }
                    quit = false;

                }
                wait(20, msec);
            }            
        }



}