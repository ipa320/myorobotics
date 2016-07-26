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

/**
 * @file   motorDriver.h
 * @Author: Tobias Bodewig
 * @brief Interface for MotorDriver Class
 * @date 19. Mai 2016
 */

#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H


#include <stdio.h>
#include <stdint.h>
#include <iostream>


/// Datasets
typedef int STATUS_CODE;

struct STRUCT_SENSOR_VALUES {
    /// Position
    int32_t position;
    /// Velocity
    int16_t velocity;
    /// current in mA
    uint16_t current;
    /// displacement on magnetic stripe
    int16_t displacement;
    /// sensor 1 data @attention not used
    uint16_t sensor1;
    /// sensor 2 data @attention not used
    uint16_t sensor2;
};

class MotorDriver {
public:

    static MotorDriver* createMotorDriver(int TXID, int RXID);

    /**
     * @brief Sends DutyCycle to motorBoard
     * @param PWM DutyCylce in Range @TODO [-4000,4000]
     * @return Status Code @TODO to be defined
     */
    virtual STATUS_CODE sendDutyCycle(int pwm)=0;

    /**
     * @brief Returns sensor values of type SENSOR_VALUES
     * @return SensorValues
     */
    virtual STRUCT_SENSOR_VALUES getSensorValues()=0;

};

#endif /* MOTORDRIVER_H */
