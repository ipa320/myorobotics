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

* Neither the name of MyoMotorAPI_SPI nor the names of its
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

*/

/*
 * File:   MotorDriverCAN.h
 * Author: Tobias Bodewig
 *
 * Created on 20. Mai 2016, 10:23
 */

#ifndef MOTORDRIVERSPI_H
#define MOTORDRIVERSPI_H

//#include "KvaserCanInterface.h"
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <map>
#include "ftd2xx.h"
#include "libft4222.h"

#include "MotorDriver.h"


    typedef struct struct_SPISTREAM {
      union {
        struct {
          unsigned startOfFrame :16;
          int pwmRef : 16;
          unsigned controlFlags1 : 16;
          unsigned controlFlags2 : 16;
          unsigned dummy : 16;
          long actualPosition : 32;
          int actualVelocity : 16;
          int actualCurrent : 16;
          int springDisplacement : 16;
          int sensor1 : 16;
          int sensor2 : 16;
        };
        uint8 TxBuffer[24];

      };
    }SPISTREAM;

class MotorDriverSPI : public MotorDriver
{

public:

    MotorDriverSPI();

    ~MotorDriverSPI();

    STATUS_CODE sendDutyCycle(int pwm);

    STRUCT_SENSOR_VALUES getSensorValues();

private:


    SPISTREAM spistreamRead;
    SPISTREAM spistreamWrite;
    STRUCT_SENSOR_VALUES sV;

    FT_HANDLE ftHandle;

    int dutyCycle;

    STATUS_CODE sendFrame(int pwm);

    FT_HANDLE initSPI();

    void swap(uint8 &source, uint8 &target);

    void prepareData(SPISTREAM &spistream,int datatype);

    FT_HANDLE getFT_HANDLE();

};


#endif /* MOTORDRIVERCAN_H */
