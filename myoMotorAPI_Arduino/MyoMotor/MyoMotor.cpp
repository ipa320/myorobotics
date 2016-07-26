/*
  Copyright (c) 2016, Fraunhofer Institute for Manufacturing Engineering and Automation (IPA)
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of MyoMotor nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


  MyoMotor.cpp - Library for controlling MyoMotorBoards via SPI.
  Created by Tobias Bodewig, July 20, 2016.
*/

#include "Arduino.h"
#include "MyoMotor.h"

SPISettings myoMotorSPISettings(4000000, MSBFIRST, SPI_MODE1);

MyoMotor::MyoMotor(int ss_pin)
{
	pinMode(ss_pin, OUTPUT);
	_ss_pin = ss_pin;
}

SensorValues MyoMotor::sendSPIFrame(int dutyCycle){
  SensorValues sV;
  SPI.beginTransaction(myoMotorSPISettings);
  digitalWrite(2, LOW);
  // Start of Frame
  SPI.transfer16(0x8000);
  // dutyCycle
  SPI.transfer16(dutyCycle & 0x7FFF);
  // ControlFlags1
  SPI.transfer16(0x0000);
  // ControlFlags2
  SPI.transfer16(0x0000);
  // SensorValues
  sV.encoderPosition = ((long)SPI.transfer16(0x0000)) << 16;
  sV.encoderPosition |= ((long)SPI.transfer16(0x0000)) & 0x0000FFFF;
  sV.velocity = SPI.transfer16(0x0000);
  sV.current = SPI.transfer16(0x0000);
  sV.springDisplacement = SPI.transfer16(0x0000);
  sV.sensor1 = SPI.transfer16(0x0000);
  sV.sensor1 = SPI.transfer16(0x0000);
  //error Flags
  SPI.transfer16(0x0000);
  digitalWrite(2, HIGH);
  SPI.endTransaction();
  return sV;
}