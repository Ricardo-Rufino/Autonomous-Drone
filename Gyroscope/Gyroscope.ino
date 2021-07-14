
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

long timer = 0;

float temp;
float ax, ay, az;
float gx, gy, gz;
float angleX, angleY, angleZ;

void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() 
{
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
