//Muhammad Ali (500828818) Section: 02
//Project 1: Solar Tracker Code
//====================================

#include<Servo.h>
#include<Stepper.h>

//declare servo pin & servo object
int servo_pin = 6;
Servo myServo;
int servo_upper_limit = 90; //test these value out to see how system behaves 
int servo_lower_limit = 0;
int servo_pos = 0;

//declare stepper pins
int stepper_pin1 = 11;
int stepper_pin2 = 10;
int stepper_pin3 = 9;
int stepper_pin4 = 8;
int stepsPerRevolution = 2048; 
int stepper_speed = 10;
int stepper_step = 32;
int total_steps_right = 0;
int total_steps_left = 0;

//declare object stepper
Stepper myStepper(stepsPerRevolution, stepper_pin4, stepper_pin2, stepper_pin3, stepper_pin1);

//declare LDR pins
int left_ldr = 0;
int right_ldr = 1;
int read_left_ldr;
int read_right_ldr;
int ldrs_difference = -1; //will be used to initiate the 2nd dof
int previous_ldrs_difference = -1;

void setup() {
  //begin communication with serial monitor
  Serial.begin(9600);
  
  //setting stepper pins as output
  pinMode (stepper_pin1, OUTPUT);
  pinMode (stepper_pin2, OUTPUT);
  pinMode (stepper_pin3, OUTPUT);
  pinMode (stepper_pin4, OUTPUT);

  //set stepper motor speed
  myStepper.setSpeed(stepper_speed);
  
  //attach servo motor
  myServo.attach(servo_pin);

  //make servo go to inital position of 0
  myServo.write (servo_pos);
}

void loop() {
  //read both left and right ldrs
  read_left_ldr = analogRead(left_ldr); //left ldr
  read_right_ldr = analogRead(right_ldr); //right ldr

  //calculate the previous difference
  previous_ldrs_difference = ldrs_difference; 
  
  //calculate the difference between the two ldrs
  ldrs_difference = read_left_ldr - read_right_ldr;

  //check if left ldr has a higher value than the right that means rotate stepper counterclockwise
  if (read_left_ldr > read_right_ldr and total_steps_right != 256) {
    myStepper.step(stepper_step); //rotate stepper by 256 steps, negative bc want stepper to rotate in other direction
    total_steps_right = total_steps_right + stepper_step;
    total_steps_left = 0;
    delay(100);
    Serial.println("Stepper rotate clockwise");
    Serial.print ("Left LDR value: ");
    Serial.println (read_left_ldr);
    Serial.print ("Total steps right: ");
    Serial.println (total_steps_right);
    //return;
  }
  //checking if the right ldr has a larger value than the right ldr
  else if (read_right_ldr > read_left_ldr and total_steps_left != 128) {
    myStepper.step(-stepper_step);
    total_steps_left = total_steps_left + stepper_step;
    total_steps_right = 0;
    delay(100);
    Serial.println("Stepper rotate counterclockwise");
    Serial.print ("Left LDR value: ");
    Serial.println (read_left_ldr);
    Serial.print ("Right LDR value: ");
    Serial.println (read_right_ldr);
    Serial.print ("Total steps left: ");
    Serial.println (total_steps_left);
    //return;
  }

  //check if the ldrs_difference is positive
  if (ldrs_difference > 0) {
    servo_pos = servo_pos + 10; 
    if (previous_ldrs_difference < ldrs_difference) { //checking if the previous position was better or not 
      servo_pos = servo_pos - 10;
    } 
    myServo.write(servo_pos);
    delay(100);
  }
  //check if the ldrs_difference is negative
  else if (ldrs_difference < 0) {
    servo_pos = servo_pos - 10;
    if (previous_ldrs_difference < ldrs_difference) { //checking if the previous position was better or not 
      servo_pos = servo_pos + 10;
    } 
    myServo.write(servo_pos);
    delay(100);
  }
  
  //ensure servo angle does not go beyond the working limit
  if (servo_pos > servo_upper_limit){
    servo_pos = servo_upper_limit;
  }
  else if (servo_pos < servo_lower_limit) {
    servo_pos = servo_lower_limit;
  }
  
  //check values of ldr
  Serial.print ("Left LDR value: ");
  Serial.println (read_left_ldr);
  Serial.print ("Right LDR value: ");
  Serial.println (read_right_ldr);
  Serial.print ("LDR's Difference: ");
  Serial.println (ldrs_difference);
  Serial.print ("LDR's Previous Difference: ");
  Serial.println (previous_ldrs_difference);
  Serial.print ("Servo position: ");
  Serial.println (servo_pos);
  Serial.println ("-------------------------------------------------");
  delay (100);
}
