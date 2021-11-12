//Fazleabbas Janmohamed
//500724981
//MEC830 Section 02
//Project 1 - solar tracking 2DOF system

//Define libraries being used
#include <Stepper.h>
#include <Servo.h>

//initialize variables
#define steps 32
int Steps; //1 Rev = 2048 steps
Stepper myStepper(steps, 9, 10, 11, 12);

Servo myServo;
int angle = 0;
int rotate = 5;

int photores_1 = 0, photores_2 = 1;
int photoread_1, photoread_2;

int error_1, error_2;

//Initialize set up
void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);  //Begin serial monitor at 9600 baudrate
  myServo.attach(3);    //connect servo to pin 3
  myServo.write(0);    //Set servo position to 90 deg

  myStepper.setSpeed(300);  //Set stepper speed
}

void loop() {
  // put your main code here, to run repeatedly:
  photoread_1 = analogRead(photores_1);     //Read photoresistor 1
  photoread_2 = analogRead(photores_2);     //Read photoresistor 2
  error_1 = photoread_2 - photoread_1;        //calculate error

  //Azimuth sweep motion
  if (abs(error_1) >= 20) {
    if (error_1 < 0) {
      Steps = -25;
      myStepper.step(Steps);
    } else if (error_1 > 0) {
      Steps = 25;
      myStepper.step(Steps);
    }
    return;     //Reapeat loop
  }

  error_2 = error_1;

  //Altitude sweep motion
  if (error_1 < -20) {            //error_1 < 0 &&
    angle = angle - rotate;
    if (angle <= 0) {
      angle = 0;
    }
    myServo.write(angle);
    error_1 = photoread_2 - photoread_1;
    if (error_2 < error_1) {
      myServo.write(angle + rotate);
    }
  } else if (error_1 > 20) {          //error_1 > 0 &&
    angle = angle + rotate;
    if (angle >= 150) {
      angle = 150;
    }
    myServo.write(angle);
    error_1 = photoread_2 - photoread_1;
    if (error_2 < error_1) {
      myServo.write(angle - rotate);
    }
  }
  delay (100);        //delay to allow motion

  //Print values
  Serial.print("Photoresistor reading 1 = " + String(photoread_1) + " photoresistor reading 2 = " + String(photoread_2) + " " + String(error_1) + " " + String(error_2) + "\n");
}
