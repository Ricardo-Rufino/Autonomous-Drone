#include <IBusBM.h>
#include <Servo.h>

IBusBM IBus;                                // IBus object

Servo motor1, motor2, motor3, motor4;       // Servo objects that control the ESCs of the 4 motors.

int val_1, val_2, val_3, val_4;
int val_5, val_6, val_7, val_8;
int val_9, val_l0; 

int throttleVal;

void setup() {
  IBus.begin(Serial);                       // iBUS object connected to serial0 RX pin

  motor1.attach(3, 1000, 2000);             // (pin, min pulse width, max pulse widhth in milliseconds)
  motor2.attach(5, 1000, 2000);
  motor3.attach(6, 1000, 2000);
  motor4.attach(9, 1000, 2000);
}

void loop() {
  val_1  = IBus.readChannel(0);             // get latest value for servo channel 1
  val_2  = IBus.readChannel(1);             // get latest value for servo channel 2
  val_3  = IBus.readChannel(2);             // get latest value for servo channel 3
  val_4  = IBus.readChannel(3);             // get latest value for servo channel 4
  val_5  = IBus.readChannel(4);             // get latest value for servo channel 5
  val_6  = IBus.readChannel(5);             // get latest value for servo channel 6
  val_7  = IBus.readChannel(6);             // get latest value for servo channel 7
  val_8  = IBus.readChannel(7);             // get latest value for servo channel 8
  val_9  = IBus.readChannel(8);             // get latest value for servo channel 9
  val_l0 = IBus.readChannel(9);             // get latest value for servo channel 10

  // Controlling motion of drone.
  motionControl(val_3);
  
  Serial.print(val_1);
  Serial.print(","); Serial.print(val_2);
  Serial.print(","); Serial.print(val_3);
  Serial.print(","); Serial.print(val_4);
  Serial.print(","); Serial.print(val_5);
  Serial.print(","); Serial.print(val_6);
  Serial.print(","); Serial.print(val_7);
  Serial.print(","); Serial.print(val_8);
  Serial.print(","); Serial.print(val_9);
  Serial.print(","); Serial.print(val_l0);
  Serial.println();
  delay(20);
}

void motionControl(int val_3){
  throttleVal = map(val_3, 1000, 2000, 0, 180);

  motor1.write(throttleVal);                    // sets the servo positions according to the scaled value
  motor2.write(throttleVal);
  motor3.write(throttleVal);
  motor4.write(throttleVal);
}
