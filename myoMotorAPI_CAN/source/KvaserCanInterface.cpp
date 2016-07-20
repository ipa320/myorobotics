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

/** @file KvaserCanInterface.cpp
 *  
 *  @date 30 Sep 2012
 *  @Author alex
 */



#include <canlib.h>
#include <stdio.h>
#include <iostream>
#include "KvaserCanInterface.h"

using namespace std;

canNotifyData rxNotifyData;

KvaserCanInterface::KvaserCanInterface(void)
{

}

STATUS_CODE KvaserCanInterface::getAvailableCanChannels(int &numberOfChannels)
{
	int channelCount;
	int status;
	status=canGetNumberOfChannels(&channelCount);
	numberOfChannels=channelCount;
	return status;
}


/*
 *
 */
 CHANNEL_HANDLE KvaserCanInterface::openCanChannel(int channelId)
 {
	 int handle;

	 //opens channel for exclusive access
	 handle =  canOpenChannel(channelId , canWANT_EXCLUSIVE);
	 return handle;
 }
/*
 *
 */
 STATUS_CODE KvaserCanInterface::configureCanChannel(int busHandle, int baudRate, int silent)
 {
	 //values below from Kvaser example
	 int canStatus;
	 long freq = baudRate;
	 unsigned int tseg1=4;
	 unsigned int tseg2=3;
	 unsigned int sjw=1;
	 unsigned int noSamp=1;
	 unsigned int syncmod=0;
	 canStatus = canSetBusParams(busHandle, freq, tseg1,tseg2, sjw, noSamp,  syncmod);

	 if (canStatus>-1)//which means we have no error
	 {
		//set bus to silent mode if this is requested
		 if (silent)
		 {
			 canStatus=canSetBusOutputControl(busHandle,canDRIVER_SILENT);
		 }
		 else
		 {
			 canStatus=canSetBusOutputControl(busHandle,canDRIVER_NORMAL);
		 }

	 }
		 return canStatus;
 }


 STATUS_CODE KvaserCanInterface::disconnectCanChannel(CHANNEL_HANDLE channelId)
 {
	 return canBusOff(channelId);
 }


 STATUS_CODE KvaserCanInterface::connectCanChannel(CHANNEL_HANDLE channelId)
 {
	return canBusOn(channelId);

 }


/*
 *
 */
 void KvaserCanInterface::getErrorText(STATUS_CODE currentStatus, string & errorString)
 {
	 char textBuffer[100];
	 canGetErrorText((canStatus)currentStatus,textBuffer,100);
	 string kvaserErrorString(textBuffer);
	 errorString=kvaserErrorString;
	 return;
 }

 STATUS_CODE KvaserCanInterface::sendCanMessage(CHANNEL_HANDLE busHandle,CAN_MESSAGE &canMessage)
 {

	 //check if extended message is requested
	 unsigned int flag;
	 if (canMessage.extended)
	 {
		 flag=canMSG_EXT;
	 }
	 else
	 {
		 flag=canMSG_STD;
	 }


	 if (canMessage.dlc>8)
	 {
		 canMessage.dlc=8;
	 }
	 return canWrite(busHandle,		  canMessage.messageID,			  &(canMessage.data8[0]),	 canMessage.dlc,  	  flag);
 }

 STATUS_CODE KvaserCanInterface::readCanMessage(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessage )
 {

	 unsigned long rxTime;
	 return canRead(busHandle,
			 &(canMessage.messageID),
			 &(canMessage.data8[0]),
	    	 &(canMessage.dlc),
	     	 &(canMessage.flags),
	     	 &rxTime);

 }


 STATUS_CODE KvaserCanInterface::setUprxCallback(CHANNEL_HANDLE busHandle, void (*callback)(canNotifyData *))
 {
	 return   canSetNotify (busHandle, callback,canNOTIFY_RX,this);
 }
