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

/**
 * 
 *  @interface KvaserCanInterface.h
 *  @file KvaserCanInterface.h
 *  @date 30 Sep 2012
 *  @Author Alexander Lenz
 */

#ifndef KVASERCANINTERFACE_H_
#define KVASERCANINTERFACE_H_

#include "CanInterface.h"
#include "canlib.h" //include the KVASER an library
#include "canstat.h"


/*Definition of  possible bitrates based on KVASER definition
 */
#define CAN_BAUDRATE__1M   canBITRATE_1M
#define CAN_BAUDRATE__500K canBITRATE_500K
#define CAN_BAUDRATE__250K canBITRATE_250K
#define CAN_BAUDRATE__125K canBITRATE_125K
#define CAN_BAUDRATE__100K canBITRATE_100K
#define CAN_BAUDRATE__62K  canBITRATE_62K
#define CAN_BAUDRATE__50K  canBITRATE_50K
#define CAN_BAUDRATE__83K  canBITRATE_83K
#define CAN_BAUDRATE__10K  canBITRATE_10K



/** 
 * @brief encapsulates the Kvaser CAN API, implements CanInterface
 */
class KvaserCanInterface : public CanInterface
{
public:
	KvaserCanInterface(void);
        
	STATUS_CODE getAvailableCanChannels(int &numberOfChannels);

	CHANNEL_HANDLE openCanChannel(int channelId);

	STATUS_CODE configureCanChannel(int busHandle, int baudRate, int silent);
	
        STATUS_CODE disconnectCanChannel(CHANNEL_HANDLE channelId);
	
	STATUS_CODE connectCanChannel(CHANNEL_HANDLE channelId);

	void getErrorText(STATUS_CODE currentStatus, string & errorString);

	STATUS_CODE sendCanMessage(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessage);

	STATUS_CODE readCanMessage(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessage);
         
	static void rxCallback(void);

        /**
         * @brief Set which function is called upon recieving CAN data
         * @param busHandle Channel handle as defined by openCanChannel(int channelId)
         * @param callback Pointer to callback function
         * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
         * @see <a href="http://www.kvaser.com/canlib-webhelp/group___c_a_n.html#ga7d2b3b2381f355b0fa4a0a561068bcdb">Kvasar CAN Library Documentation</a>
         */
	STATUS_CODE setUprxCallback(CHANNEL_HANDLE busHandle, void (*callback)(canNotifyData *));

private:


};



#endif /* KVASERCANINTERFACE_H_ */
