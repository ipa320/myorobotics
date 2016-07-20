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

* Neither the name of MyoMotorAPI_CAN nor the names of its
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
 * @File   MotorDriverCAN.h
 * @Author Tobias Bodewig
 * @date   20. Mai 2016
 *
 */

#ifndef MOTORDRIVERCAN_H
#define MOTORDRIVERCAN_H

// standard libs
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <map>

// internal 
#include "MotorDriver.h"
#include "KvaserCanInterface.h"

typedef int CHANNEL_HANDLE;
typedef int STATUS_CODE;

/**
 * @brief Auxilary data from motor
 */
union motorDataAux {
	char data[8];
    struct {
                /// current in mA
		int16_t current;
                /// displacement on magnetic stripe
		int16_t displacement;
                /// unused
		uint16_t free1;
                /// unused
		uint16_t free2;
    } s;

} ;

/**
 * @brief MainData from motorboard
 */
union motorDataMain {
	char data[8];
    struct {
           /// Rotational velocity based on encoderPosition per millisecond
	  float omega;
          ///  Encoder Position
	  int32_t encoderPosition;
    } s;

} ;

/**
 * @brief Datastructure to hold motor Commands
 */
union motorControlCommand{
	char data[8];
	struct{
                /// Motor dutyCycle [-4000;4000]
		float dutyCycle;
                /// unused
		float free;
	}s;

};
/**
 * @brief  CAN implementation of MotorDriver
 */
class MotorDriverCAN : public MotorDriver
{
    
public:
    
    
    /// @brief constructor
    MotorDriverCAN(int motorRXID, int motorTXID);
    
    // Methods
    STATUS_CODE sendDutyCycle(int pwm);
    STRUCT_SENSOR_VALUES getSensorValues();
    
    
    // static methods

    /**
     * @brief Method to handle incoming data
     *        Incoming data will be buffered and written to internal STRUCT_SENSOR_VALUES, which will be returned by getSensorValues()
     * @param rxNotifyData
     */
    static void handleRXData(canNotifyData * rxNotifyData);
    
    /**
     * @brief Destructor for MotorDriverCAN
     *        Sets dutyCycle to 0
     */
    ~MotorDriverCAN();
        
private:
    
    // variables
    /// CAN Message ID for sending data to motor board
    int motorRXID;
    /// CAN Message ID for recieving data from motor board
    int motorTXID;
    /// struct for buffering auxilary motor data
    motorDataAux dats;
    /// struct for buffering main motor data
    motorDataMain dats2;
    /// struct to hold motor commands
    motorControlCommand motorCommand;
    /// struct for holding all sensor values returned by the motor board. Will be returned by getSensorValues()
    struct STRUCT_SENSOR_VALUES sV;
    
    // static variables
    /// Handle for CAN Controller
    static CHANNEL_HANDLE busHandle;
    /// Pointer to open KVASER CAN device
    static KvaserCanInterface* canInterface;
    /// mapping of recieving CAN message IDs to the corresponding MotorDriverCAN object
    static std::map<int, MotorDriverCAN*> canMessageMap;
    
    // methods
    /// Method to set CAN message IDs, includes them into local map canMessageMap
    void setMessageIDs(int motorTXID,int motorRXID);
    
    // static methods
    /// uses canMessageMap to find the MotorDriverCAN object which correspons to the message ID
    static MotorDriverCAN* getObjectFromMessageID(int ID);
    
    /**
     * @brief Initializes CAN interface
     *          Opens a new CAN interface if a CAN controller is available
     * @return 
     */
    static STATUS_CODE initCAN();
    
    
    
    

   
    
    
};


#endif /* MOTORDRIVERCAN_H */

