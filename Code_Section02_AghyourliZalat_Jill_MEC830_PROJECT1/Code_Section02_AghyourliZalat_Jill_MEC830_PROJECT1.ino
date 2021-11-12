//Jill Aghyourli Zalat
//500811215
//MEC830 Section 02
//Project 1 Software Implementation
// =========================================================
// Include necessary header files for stepper and servo
#include <Stepper.h>
#include <Servo.h>

// Initialize stepper motor variables
#define STEPS  32
int  Steps2Take;  // 2048 = 1 Revolution
Stepper small_stepper(STEPS, 8, 10, 9, 11);

// Initialize servo motor variables
Servo myservo;
int angle = 90;
int rotation = 5;

// Initialize photoresistor variables
int photocellPin1 = 0, photocellPin2 = 1;
int photocellReading1, photocellReading2;

// Error variables
int error, prev_error;
int coarse_error_threshold = 30;
int fine_error_threshold = 5;

void setup(void) {
  Serial.begin(9600); // initialize serial monitor for debugging

  // attach servo to necessary pins and rotate to default mode
  myservo.attach(12);
  myservo.write(90);

  //    Steps2Take = -256;
  small_stepper.setSpeed(250);
  //    small_stepper.step(Steps2Take);
}

void loop(void) {

  photocellReading1 = analogRead(photocellPin1);
  photocellReading2 = analogRead(photocellPin2);
  error = photocellReading2 - photocellReading1;
  // error less than 10 when at good azimuth angle
  // error < 0 when photocell 1 is at a better place than 2
  // error > 0 when photocell 2 is at a better place than 1

  // Azimuth Sweep (TODO: map speed of stepper to error difference)
  // -----------------------------------------------------------------
  if (abs(error) >= coarse_error_threshold)
  {

    if (error < 0) // 1 has a better view of light vs 2: move ccw (towards 1's perspective)
    {
      Steps2Take = -35;
      small_stepper.step(Steps2Take);
    }
    else if (error > 0) // 2 has a better view so move cw (toward's 2's perspective)
    {
      Steps2Take = 35;
      small_stepper.step(Steps2Take);
    }
    return; // go back to start of loop()
  }

  prev_error = error;
  
  //  Altitude Sweep (finer refinement to make error smaller)
  // ------------------------------------------------------------------
  if (error < 0 && error < -fine_error_threshold)
  {
    angle = angle - rotation;
    if (angle <= 0)
      {
        angle = 0;
      }
    myservo.write(angle);
    error = photocellReading2 - photocellReading1;
    if (prev_error < error)
    {
      myservo.write(angle+rotation); // go back to previous angle if previous error was smaller
    }
  }
  else if (error >  0 && error > fine_error_threshold)
  {
    angle = angle + rotation;
    

     if (angle >= 150)
        {
          angle = 150;
        }
    myservo.write(angle);
    error = photocellReading2 - photocellReading1;
    if (prev_error < error)
    {
      myservo.write(angle-rotation); // go back to previous angle if previous error was smaller
   }
   

  }



 

  delay(100);
  Serial.print("[loop] photocellReading1 = ");
  Serial.print(String(photocellReading1) + " photocellReading2 = " + String(photocellReading2) + " " + String(error) + " " + String(angle) + "\n");

}
