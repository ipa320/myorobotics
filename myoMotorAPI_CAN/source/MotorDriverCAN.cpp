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

#include "MotorDriverCAN.h"
#include "KvaserCanInterface.h"

KvaserCanInterface* MotorDriverCAN::canInterface;
std::map<int, MotorDriverCAN*> MotorDriverCAN::canMessageMap;
CHANNEL_HANDLE MotorDriverCAN::busHandle;

MotorDriverCAN::MotorDriverCAN(int motorRXID, int motorTXID) {


    if (canInterface == NULL) {
        cout << "init can" << endl;
        initCAN();
        
    } 
    setMessageIDs(motorRXID, motorTXID);


}

void MotorDriverCAN::setMessageIDs(int motorRXID, int motorTXID) {
    // create new entry in canMessageMap for motorTXID
    canMessageMap.insert(std::pair<int, MotorDriverCAN*>(motorTXID, this));
    MotorDriverCAN::motorRXID = motorRXID;
    MotorDriverCAN::motorTXID = motorTXID;
}

MotorDriverCAN* MotorDriverCAN::getObjectFromMessageID(int ID) {
    return canMessageMap[ID];
}

STATUS_CODE MotorDriverCAN::initCAN() {
    canInterface = new KvaserCanInterface();
    string errorStr;
    STATUS_CODE status;
    
    // open new CAN channel
    CHANNEL_HANDLE busHandle = canInterface->openCanChannel(0);

    if (busHandle > -1) {
        cout << "CAN Bus available" << endl;
        // if channel available, configure CAN interface to use 1M Baudrate and non-silent mode
        status = canInterface->configureCanChannel(busHandle, CAN_BAUDRATE__1M, 0);
        // print errors if applicable and start CAN channel
        canInterface->getErrorText(status, errorStr);
        canInterface->connectCanChannel(busHandle);
        cout << "CAN configured, 1Mbit/s, status is: " << errorStr << endl;

        // Set method to handle incoming data
        status = canInterface->setUprxCallback(busHandle, MotorDriverCAN::handleRXData);
        canInterface->getErrorText(status, errorStr);
        cout << "Rx call-back set-up: " << errorStr << endl;

    } else
        cout << "not available CAN" << endl;


}

void MotorDriverCAN::handleRXData(canNotifyData * rxNotifyData) {
    
    int handle;
    long id;
    char data[8];
    unsigned int dlc, flags;
    unsigned long timestamp;
    STATUS_CODE stat;
        
      
    // Read incoming CAN Data and proceed if no error
    while (canERR_NOMSG != canRead(MotorDriverCAN::busHandle, &id, data, &dlc, &flags, &timestamp))
    {
        // check if recieved CAN mesage ID corresponds to a motorTXID - corresponds to motorDataMain
        if(canMessageMap[id] !=NULL)
        {
            // Buffer data into motoDataMain buffer
            for(int i=0;i<8;i++)
            {
                canMessageMap[id]->dats2.data[i]= data[i];
            }
            // write buffer to STRUCT_SENSOR_VALUES
            canMessageMap[id]->sV.position = canMessageMap[id]->dats2.s.encoderPosition;
            canMessageMap[id]->sV.velocity = canMessageMap[id]->dats2.s.omega;
          
        }
        // check if recieved CAN mesage ID corresponds to a motorTXID + 1 - corresponds to motorDataAux
        else if(canMessageMap[id-1] !=NULL)
        {
            // Buffer data into motoDataAux buffer
            for(int i=0;i<8;i++)
            {
                canMessageMap[id-1]->dats.data[i]= data[i];
            }
            // write buffer to STRUCT_SENSOR_VALUES
            canMessageMap[id-1]->sV.current = canMessageMap[id-1]->dats.s.current;
            canMessageMap[id-1]->sV.displacement = canMessageMap[id-1]->dats.s.displacement;
          
        }
    }
    
}

STATUS_CODE MotorDriverCAN::sendDutyCycle(int dutyCycle) {
    CAN_MESSAGE canSentMessage;
    
    // set parameters for CAN message - see generic CAN protocol
    canSentMessage.dlc=8;
    canSentMessage.messageID=motorRXID;
    canSentMessage.extended=0;
    
    // setup motorCommand struct for sending over CAN
    MotorDriverCAN::motorCommand.s.dutyCycle = dutyCycle;
    MotorDriverCAN::motorCommand.s.free = 0;
    
    // write motorCommand struct to canMessage data structure
    for (int j=0;j<8;j++)
    {
	canSentMessage.data8[j]=MotorDriverCAN::motorCommand.data[j];
    }
    
    // send canMessage
    MotorDriverCAN::canInterface->sendCanMessage(MotorDriverCAN::busHandle,canSentMessage);
			 
}

STRUCT_SENSOR_VALUES MotorDriverCAN::getSensorValues() {
    return sV;
}

MotorDriverCAN::~MotorDriverCAN()
{
    MotorDriverCAN::sendDutyCycle(0);
}