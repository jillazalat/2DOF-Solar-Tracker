#include <Stepper.h>
#include <Servo.h>
Servo myservo;

#define p1 A0
#define p2 A1

#define STEPS 2048
Stepper stepper(STEPS, 8, 10, 9, 11);

int valL=0, valR=0, prevVal, pos = 90;

int sweepVal[40], i = 0, maxVal, maxi, flag = 0;

void setup() {
  Serial.begin(9600);
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);

  myservo.attach(3);
  myservo.write(pos);
}

void loop() {
  pos = constrain(pos, 50, 90);
  int valR = analogRead(p1); //read between (0-1023)
  int value2 = analogRead(p2);
  int valL = map(value2, 10, 280, 0, 412);
  int diff = valL - valR;

  //Serial.println(diff);

  if (diff >= 100) { //if p1 gets more light, rotate towards p2
    stepper.setSpeed(8);
    stepper.step(1);
    flag=1;
  }

  if (diff <= -100) { //if p2 gets more light, rotate towards p1
    stepper.setSpeed(8);
    stepper.step(-1);
    flag=1;
  }

    if((diff <= 100) && (diff >= -100)){
      if(flag==1) {
      i = 0;
      maxVal = 0;
      maxi = 0;
      for (pos = 90; i < 40; i++) {
        sweepVal[i] = analogRead(p1);
        pos -= 1;
        myservo.write(pos);
        delay(50);
      }
      for (i = 0; i < 40; i++) {
        if (sweepVal[i] > maxVal) {
          maxVal = sweepVal[i];
          maxi = i;

        }
      }
      flag=0;
    }
    myservo.write(90 - maxi);
    delay(400);
    valR = analogRead(p1); //read between (0-1023)
    value2 = analogRead(p2);
    valL = map(value2, 10, 280, 0, 412);
    diff = valL - valR;
    Serial.print(diff);
    Serial.print(",");
  }

  Serial.print(flag);
  Serial.println(",");

}
