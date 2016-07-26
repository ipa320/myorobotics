/*
  MyoMotor.cpp - Library for controlling MyoMotorBoards via SPI.
  Created by Tobias Bodewig, July 20, 2016.
*/

#include <MyoMotor.h>

MyoMotor motor(2);

SensorValues senVal;

void setup() {
  // Setup Serial
  Serial.begin(9600);
  SPI.begin();
}

void loop() {
  // Send a dutyCycle of 200 (in Range +-4000) and recive SensorValues
  senVal = motor.sendSPIFrame(200);
  // Output SensorValues
  Serial.print(senVal.encoderPosition);
  Serial.print(",");
  Serial.print(senVal.velocity);
  Serial.print(",");
  Serial.print(senVal.current);
  Serial.print(",");
  Serial.print(senVal.springDisplacement);
  Serial.print(",");
  Serial.print(senVal.sensor1);
  Serial.print(",");
  Serial.print(senVal.sensor2);
  Serial.println();
}



