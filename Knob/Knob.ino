/*
 Controlling brushless motor position using a potentiometer (variable resistor)
 by Ricardo Rufino
*/

#include <Servo.h>

Servo myservo;                          // create servo object to control a servo
Servo motor1, motor2, motor3, motor4;   // Servo objects that control the ESCs of the 4 motors.

int potPin = A0;                        // analog pin used to connect the potentiometer

int val;                                // variables to read the value from the analog pin

void setup() {
  Serial.begin(9600);

  motor1.attach(3, 1000, 2000);         // (pin, min pulse width, max pulse widhth in milliseconds)
  motor2.attach(5, 1000, 2000);
  motor3.attach(6, 1000, 2000);
  motor4.attach(9, 1000, 2000);
}

void loop() {
  val = analogRead(potPin);             // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 180);      // scale it to use it with the servo (value between 0 and 180)
  
  motor1.write(val);                    // sets the servo positions according to the scaled value
  motor2.write(val);
  motor3.write(val);
  motor4.write(val);
  delay(10);

  Serial.println(val);
}
