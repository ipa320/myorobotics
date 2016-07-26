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

#include "MotorDriver.h"
#include <unistd.h>
#include <math.h>
#include <iomanip>

// Prints the SensorValues to the Console
void display(STRUCT_SENSOR_VALUES &sV)
{
  std::cout  << "sV.position "<< std::dec << std::setfill(' ')<< std::setw(10) << sV.position <<
  "| vel: "   << std::setfill(' ')<< std::setw(5) << sV.velocity <<
  " | curr: " << std::setfill(' ')<< std::setw(4) << sV.current <<
  " | dis: "  << std::setfill(' ')<< std::setw(4) << sV.displacement<<
  std::endl;
}


int main()
{
    // Creates a struct to store the SensorValues
    STRUCT_SENSOR_VALUES sV;
    // Creates a MotorDriver and establishes the communication
    MotorDriver* md = MotorDriver::createMotorDriver(0,0);
    // Sends a DutyCycle of 0 (Stops the Motor)
    md->sendDutyCycle(0);
    // Recive Sensor Values from the MotorDriver
    sV = md->getSensorValues();
    // Prints the SensorValues to the Console
    display(sV);
    // Sends a DutyCycle of 300
    md->sendDutyCycle(300);
    //Displays the Sensor Values 20 times per second for 10 seconds
    for(long i = 0; i < 10*20; i++){
        // Recive Sensor Values from the MotorDriver
        sV = md->getSensorValues();
        // Prints the SensorValues to the Console
        display(sV);
        // waits for 50ms
        usleep(50*1000);
    }   
    // Sends a DutyCycle of 0 (Stops the Motor)
    md->sendDutyCycle(0);
}
