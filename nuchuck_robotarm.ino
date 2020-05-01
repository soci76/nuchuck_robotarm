// Nunchuk controlled robotic arm by Igor Fonseca Albuquerque
// Original Nunchuk library by Robert Eisele: https://www.xarg.org/2016/12/using-a-wii-nunchuk-with-arduino/

//KIEGÉSZÍTŐ információk:
//        Y: jobbra ->180 fölé, bal ->-180 fölé
//        Y: előre ->180 fölé, hátra ->-180 fölé
//        Z: ???

//Include libraries
#include <Nunchuk.h>
#include <Wire.h>
#include <Servo.h>

//define variables
#define SERV1 12    //servo 1 on digital port 12
#define SERV2 11    //servo 2 on digital port 11
#define SERV3 10   //servo 3 on digital port 10
#define SERV4 9   //servo 4 on digital port 9
#define SERV5 6   //servo 5 on digital port 6
#define SERV6 5   //servo 6 on digital port 5
#define SERV7 3   //servo 6 on digital port 3

Servo s1; //servo 1 - kez
Servo s2; //servo 2 - fcsuklo
Servo s3; //servo 3 - hcsuklo
Servo s4; //servo 4 - hkonyok
Servo s5; //servo 5 - hvall1
Servo s6; //servo 6 - hvall2
Servo s7; //servo 6 - fvall

//define starting angle for each servo
//choose a safe position to start from
//it will try to move instantaniously to that position when powered up!
//those angles will depend on the angle of each servo during the assemble
int angle1 = 45; //servo 1 current angle
int angle2 = 78; //servo 2 current angle
int angle3 = 10;  //servo 3 current angle
int angle4 = 10; //servo 4 current angle
int angle56 = 90; //servo 5 and 6 current angle
int angle7 = 45; //servo 6 current angle
int servo1_speed = 1; //servo 1 speed
int servo2_speed = 10; //servo 2 speed
int servo3_speed = 3; //servo 3 speed
int servo4_speed = 3; //servo 4 speed
int servo56_speed = 3; //servo 5 and 6 speed
int servo7_speed = 3; //servo 7 speed

//define restrictions for each servo
//those angles will depend on the angle of each servo during the assemble
int angle1min = 45;   //servo 1 minimum angle
int angle1max = 75; //servo 1 maximum angle
int angle2min = 10;   //servo 2 minimum angle
int angle2max = 170; //servo 2 maximum angle
int angle3min = 10;   //servo 3 minimum angle
int angle3max = 170; //servo 3 maximum angle
int angle4min = 10;   //servo 4 minimum angle
int angle4max = 170; //servo 4 maximum angle
int angle56min = 10;   //servo 5 and 6 minimum angle
int angle56max = 170; //servo 5 and 6 maximum angle
int angle7min = 10;   //servo 7 minimum angle
int angle7max = 170; //servo 7 maximum angle

boolean display_angles = true; //boolean used to update the angle of each servo on Serial Monitor

//SETUP
void setup() {
    //attach each servo to a pin and start its position
    s1.attach(SERV1);
    s1.write(angle1);
    s2.attach(SERV2);
    s2.write(angle2);
    s3.attach(SERV3);
    s3.write(angle3);
    s4.attach(SERV4);
    s4.write(angle4);
    s5.attach(SERV5);
    s5.write(angle56);
    s6.attach(SERV6);
    s6.write(angle56);
    s7.attach(SERV7);
    s7.write(angle7);

    //start serial communication
    Serial.begin(9600);

    //start Nunchuk
    Wire.begin();
    nunchuk_init();
}

void loop() {
    //read Nunchuk sensors
    if (nunchuk_read()) {
        
        int x = nunchuk_joystickX();    //joystick X position
        int y = nunchuk_joystickY();    //joystick Y position
        int acx = nunchuk_accelX();    //joystick X acceleration
        int acy = nunchuk_accelY();    //joystick Y acceleration
        int acz = nunchuk_accelZ();    //joystick Y acceleration
        boolean z = nunchuk_buttonZ();  //z button status
        boolean c = nunchuk_buttonC();  //c button status
        float pitch = nunchuk_pitch();  //pitch angle
        float roll = nunchuk_roll();    //roll angle

        //Move up/down (at a fixed speed)
        //Move up
        if (y > 90) {
          angle7 += servo7_speed;
          display_angles = true;
          if (angle7 > angle7max) {
            angle7 = angle7max;
          }
        }
        //Move down
        if (y < -90) {
          angle7 -= servo7_speed;
          display_angles = true;
          if (angle7 < angle7min) {
            angle7 = angle7min;
          }
        }
        s7.write(angle7);  //update servo position

        //--------------------------------------------------------

        //Turn left/right (at a fixed speed)
        //Turn left
        if (x > 90) {
          angle56 -= servo56_speed;
          display_angles = true;
          if (angle56 < angle56min) {
            angle56 = angle56min;
          }
        }
        //Turn right
        if (x < -90) {
          angle56 += servo56_speed;
          display_angles = true;
          if (angle56 > angle56max) {
            angle56 = angle56max;
          }
        }
        s5.write(angle56); //update servo position
        s6.write(angle56); //update servo position

        //--------------------------------------------------------

        //Turn left/right (at a fixed speed)
        //Turn left
        if (acy > 130) {
          angle4 -= servo4_speed;
          display_angles = true;
          if (angle4 < angle4min) {
            angle4 = angle4min;
          }
        }
        //Turn right
        if (acy < -130) {
          angle4 += servo4_speed;
          display_angles = true;
          if (angle4 > angle4max) {
            angle4 = angle4max;
          }
        }
        s4.write(angle4); //update servo position
        
        //--------------------------------------------------------
        
        //Move up/down (at a fixed speed)
        //Move up
        if (acx > 130) {
          angle3 += servo3_speed;
          display_angles = true;
          if (angle3 > angle3max) {
            angle3 = angle3max;
          }
        }
        //Move down
        if (acx < -130) {
          angle3 -= servo3_speed;
          display_angles = true;
          if (angle3 < angle3min) {
            angle3 = angle3min;
          }
        }
        s3.write(angle3);  //update servo position

        //--------------------------------------------------------

        //Move gripper up/down (only C button pressed)
        if (z || c) {
          
          if ( z ) {
            angle2 += servo2_speed;
            display_angles = true;
            if (angle2 > angle2max) {
              angle2 = angle2max;
            }
          }
          if ( c ) {
            angle2 -= servo2_speed;
            display_angles = true;
            if (angle2 < angle2min) {
              angle2 = angle2min;
            }
          }
          s2.write(angle2); //update servo position
        }

      //--------------------------------------------------------

      //Open/close gripper (both buttons pressed)
      if(z && c) {
        s1.write(angle1max); //close gripper
        display_angles = true;
      }
      else {
        s1.write(angle1min); //open gripper
      }
    }

    //Show position of each servo
    if (display_angles) {
      Serial.println("//Current angles:");
      Serial.print("Servo #1: ");
      if (angle1 == angle1max) {
        Serial.println("gripper closed");
      }
      else {
        Serial.println("gripper opened");
      }
      Serial.print("Servo #2: ");
      Serial.println(angle2);
      Serial.print("Servo #3: ");
      Serial.println(angle3);
      Serial.print("Servo #4: ");
      Serial.println(angle4);
      Serial.print("Servo #5 and #6: ");
      Serial.println(angle56);
      Serial.print("Servo #7: ");
      Serial.println(angle7);
      display_angles = false;
    }
}
