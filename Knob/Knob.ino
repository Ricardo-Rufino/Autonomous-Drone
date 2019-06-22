/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Ricardo Rufino
*/

#include <Servo.h>

Servo myservo;                         // create servo object to control a servo
Servo oneOne, oneTwo, twoOne, twoTwo;

int potPin = A0;                       // analog pin used to connect the potentiometer
int val;                               // variables to read the value from the analog pin

void setup() {
  Serial.begin(9600);
  oneOne.attach(9);  // attaches the servo on pin 9 to the servo object
  oneTwo.attach(6);
  twoOne.attach(5);
  twoTwo.attach(3);
}

void loop() {
  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  
  oneOne.write(val);                  // sets the servo positions according to the scaled value
  oneTwo.write(val);
  twoOne.write(val);
  twoTwo.write(val);
  delay(10);

  Serial.println(val);
  
}

