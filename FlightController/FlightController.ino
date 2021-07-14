/*
 * Author: Ricardo Rufino
 */

#include <IBusBM.h>
#include <Servo.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

IBusBM IBus;                                // IBus object.

Servo motor1, motor2, motor3, motor4;       // Servo objects that control the ESCs of the 4 motors. 

MPU6050 mpu6050(Wire);                      // Gyroscope object.

// Variables used to store values of RC controller's channels.--------------------------------------
/* ch1: left-right, low=1000, high=2000, idle=1498
 * ch2: bottom-top, low=1000, high=2000, idle=1497 
 * ch3: throttle, low=1000, high=2000, idle=n/a
 * ch4: yaw, low=1000, high=2000, idle=1500
 * ch5:
 * ch6:
 * ch7:
 * ch8:
 * ch9:
 * ch10:
 */
int ch1, ch2, ch3, ch4;
int ch5, ch6, ch7, ch8;
int ch9, chl0; 

int ch1_dead_lo = 1494, ch1_dead_hi = 1502, ch1_center=1498;
int ch2_dead_lo = 1493, ch2_dead_hi = 1501, ch2_center=1497;
int ch4_dead_lo = 1484, ch4_dead_hi = 1516, ch4_center=1500;

int xRollCorrection, yRollCorrection;

//Varibales used to storve values of Gyroscope readings. --------------------------------------------
float temp;
float ax, ay, az;
float gx, gy, gz;
float angleX, angleY, angleZ;

float xAngleCorrection;
float yAngleCorrection;

// Variables used to control serial trouble shooting. -----------------------------------------------
bool troubleShootRC         = false;
bool troubleShootGyro       = false;
bool troubleShootStabilizer = false;

// Variables used to start the drone.
bool start1 = true;

// Additional variables. ----------------------------------------------------------------------------
int throttleValController;                  // Used to control the throttle of the drone.
int throttleValMotor1;                      // Used to control the throttle of motor1 (front-left).
int throttleValMotor2;                      // Used to control the throttle of motor2 (front-right).
int throttleValMotor3;                      // Used to control the throttle of motor3 (back-left).
int throttleValMotor4;                      // Used to control the throttle of motor4 (back-right).

void setup() 
{
  IBus.begin(Serial);                       // iBUS object connected to serial0 RX pin

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  
  //(pin, min pulse width, max pulse widhth in milliseconds)
  motor1.attach(9, 1000, 2000);             // Top-left
  motor2.attach(6, 1000, 2000);             // Top-right
  motor3.attach(5, 1000, 2000);             // Bottom-left
  motor4.attach(3, 1000, 2000);             // Bottom-right
  
  
}

void loop() 
{
  
  // Gathering information from RC controller. --------------------------------------------------------
  ch1  = IBus.readChannel(0);             // get latest value for servo channel 1
  ch2  = IBus.readChannel(1);             // get latest value for servo channel 2
  ch3  = IBus.readChannel(2);             // get latest value for servo channel 3
  ch4  = IBus.readChannel(3);             // get latest value for servo channel 4
  ch5  = IBus.readChannel(4);             // get latest value for servo channel 5
  ch6  = IBus.readChannel(5);             // get latest value for servo channel 6
  ch7  = IBus.readChannel(6);             // get latest value for servo channel 7
  ch8  = IBus.readChannel(7);             // get latest value for servo channel 8
  ch9  = IBus.readChannel(8);             // get latest value for servo channel 9
  chl0 = IBus.readChannel(9);             // get latest value for servo channel 10

  // Gathering information from gyroscope. -------------------------------------------------------------
  mpu6050.update();
  
  temp = mpu6050.getTemp();

  ax = mpu6050.getAccX();
  ay = mpu6050.getAccY();
  az = mpu6050.getAccZ();

  gx = mpu6050.getGyroX();
  gy = mpu6050.getGyroY();
  gz = mpu6050.getGyroZ();

  angleX = mpu6050.getAngleX();
  angleY = mpu6050.getAngleY();
  angleZ = mpu6050.getAngleZ();

  // Controlling motion of drone.
  MotionControl();

  // Will print information about RC controller.
  if (troubleShootRC)
  {
    Serial.print(ch1);
    Serial.print(","); Serial.print(ch2);
    Serial.print(","); Serial.print(ch3);
    Serial.print(","); Serial.print(ch4);
    Serial.print(","); Serial.print(ch5);
    Serial.print(","); Serial.print(ch6);
    Serial.print(","); Serial.print(ch7);
    Serial.print(","); Serial.print(ch8);
    Serial.print(","); Serial.print(ch9);
    Serial.print(","); Serial.print(chl0);
    Serial.println();
    delay(100);
  }

  // Will print information about gyroscope.
  if (troubleShootGyro)
  {
    Serial.print("Temp: "); Serial.print(temp); Serial.print(", ");
    Serial.print("AccX: "); Serial.print(ax);   Serial.print(", ");
    Serial.print("AccY: "); Serial.print(ay);   Serial.print(", ");
    Serial.print("AccZ: "); Serial.print(az);   Serial.print(", ");
    Serial.print("GyroX: "); Serial.print(gx);   Serial.print(", ");
    Serial.print("GyroY: "); Serial.print(gy);   Serial.print(", ");
    Serial.print("GyroZ: "); Serial.print(gz);   Serial.print(", ");
    Serial.print("AngleX: "); Serial.print(angleX);   Serial.print(", ");
    Serial.print("AngleY: "); Serial.print(angleY);   Serial.print(", ");
    Serial.print("AngleZ: "); Serial.print(angleZ);   Serial.print(", ");
    Serial.println();
    delay(100);
  }
}

/*****************************************************************************************************
 * Functions
 ****************************************************************************************************/

/*
 * 
 */
void Stabilizer()
{ 
  xAngleCorrection = 15*angleX;
  yAngleCorrection = 15*angleY;

  xRollCorrection = ch2 + xAngleCorrection;
  yRollCorrection = ch1 - yAngleCorrection;

  if (troubleShootStabilizer)
  {
    Serial.print("xCorr: "); Serial.print(xRollCorrection); Serial.print(" , ");
    Serial.print("yCorr: "); Serial.print(yRollCorrection); Serial.print(" , ");
    Serial.print("Motor 3: "); Serial.print(throttleValMotor3);
    Serial.println();
    delay(100);
  }

  // Adjusting drone's x-axis roll. --------------------------------------------------------------------
  if (yRollCorrection >= ch1_center)
  {
    if (xRollCorrection >= ch2_center)
    {
      throttleValMotor3 += 5;
      if (throttleValMotor3 > 175)
      {
        throttleValMotor3 = 175;
      }
      motor4.write(throttleValMotor3);
    }
  }
}

/*
 * 
 */
void MotionControl()
{
  /*
   * Starting drone throttle... At this point all motors start to spin at the
   * same speed depending on the position of the thre throttle joystick (ch3).
   */
  if (ch3 < 1100)
  {
    throttleValController = map(ch3, 1000, 2000, 0, 180);

    //*****REMOVE THIS IF STATEMENT WHEN CALIBRATING ESC*****
    if (throttleValController > 135)
    {
      throttleValController = 135;
    }
  
    throttleValMotor1 = throttleValController;
    throttleValMotor2 = throttleValController;
    throttleValMotor3 = throttleValController;
    throttleValMotor4 = throttleValController;
  
    motor1.write(throttleValMotor1);                    
    motor2.write(throttleValMotor2);
    motor3.write(throttleValMotor3);
    motor4.write(throttleValMotor4);
  }
  else
  {
    Stabilizer();
  }
}








 
