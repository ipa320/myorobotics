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
 *  @file   CanInterface.h
 *  @date   30 Sep 2012
 *  @Author Alexander Lenz
 */

#ifndef CANINTERFACE_H_
#define CANINTERFACE_H_
#include <stdint.h>
#include <stdio.h>
#include <iostream>
using namespace std;


/**
 * @struct
 * @brief Handle used by CAN handler
 */
typedef int CHANNEL_HANDLE;

/**
 * @struct
 * @brief Status code returned by CAN handler 
 */
typedef int STATUS_CODE;


/**
 * @brief CAN Message struct
 */
typedef struct STRUCT_CAN_MESSAGE
		{
                        /// Unique Identifier (ID) of CAN Message. Either 11bit (standard) or 29bit (extended)
			long messageID;
                        /// Flag for sending Extended CAN Messages
			unsigned int extended;
                        /** @brief CAN Flags
                         *         Use Kvaser-based Messages when implementing please.
                         *         Set according Bit to high:
                         *           
                         *         -----------------|---------------
                         *           canMSG_MASK    |   0x00ff
                         *         -----------------|---------------
                         *           canMSG_RTR     |   0x0001
                         *         -----------------|---------------
                         * 
                         * 
                         * 
                         * 
                         * @see <a href="http://www.kvaser.com/canlib-webhelp/canstat_8h.html#canMSG_xxx"> Kvaser Dokumentation</a>
                         * */
			unsigned int flags;
                        /// The 4-bit data length code (DLC) contains the number of bytes of data being transmitted.
			unsigned int dlc;
                        /// Timestamp of Message
			unsigned long time;
			
                        /// Data sent (up to 64 bits/message )
                        union{
                                        /// Either 16*4 = 64 bits
					int16_t data16[4];
                                        /// Or 8*8= 64 bits
					unsigned char data8[8];
			};

		}CAN_MESSAGE;

/**
 * @brief Abstract class for generic CAN interface
 */
class CanInterface
{
public:
	
    
        /**
         * @brief Print CAN Message to Console
         * @param canMessage Adress to CAN Message of type CAN_MESSAGE
         */
	 void printCanMessage(const CAN_MESSAGE &canMessage);
         
         
	/**
         * @brief Return the total number of available Channels
         * @param numberOfChannels Adress for storage of number of Channels
         * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
         */
	virtual STATUS_CODE getAvailableCanChannels(int &numberOfChannels){};

	
       /**
        * @brief Open CAN Channel
        * @param channelId ID of CAN channel to be opend
        * @return Handle to open Channel
        */
	virtual CHANNEL_HANDLE openCanChannel(int channelId){};


	/** @brief Disconnect can transceiver from bus.
	*  @param channelId Channelhandle as defined by openCanChannel(int channelId)
        *  @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	*/
	virtual STATUS_CODE disconnectCanChannel(CHANNEL_HANDLE channelId){};


	/** @brief Connects can transceiver to bus.
	 *  @param channelId Channelhandle as defined by openCanChannel(int channelId)
         *  @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	 */
	virtual STATUS_CODE connectCanChannel(CHANNEL_HANDLE channelId){};
        /**
         * @brief Close CAN Channel
         * @param channelId Channelhandle as defined by openCanChannel(int channelId)
         * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
         */

	virtual STATUS_CODE closeCanChannel(CHANNEL_HANDLE channelId){};
	
        
        /**
	 * @brief configures CAN Connection
         * @param busHandle Handle to the CAN Bus, defined by openCanChannel(int channelId)
         * @param baudRate canBITRATE [10k, 10M]
         * @param silent Silentmode on (1) or off (2)
         * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	 */
	virtual STATUS_CODE configureCanChannel(CHANNEL_HANDLE busHandle, int baudRate, int silent){};
	
        
        /** @brief Translates STATUS_CODE to an Error String
         *  @param currentStatus STATUS_CODE from transmission
         *  @param errorString Adress where to place the error code string
         *  @return void
	 *
	 */
	virtual void getErrorText(STATUS_CODE currentStatus, string & errorString){};

	/** @brief Send CAN Message over Handle
          *  @param busHandle Channel handle as defined by openCanChannel(int channelId)
	  *  @param canMessage Adress to CAN Message struct to be send
          * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	  */
	virtual STATUS_CODE sendCanMessage(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessage){};

	 /**@brief Waits until message is queued in Tx buffer or timeOut is exceeded
          * @param busHandle Channel handle as defined by openCanChannel(int channelId)
          * @param canMessage Adress to CAN Message struct to be send
          * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	 */
	virtual STATUS_CODE sendCanMessageWait(CHANNEL_HANDLE busHandle,const CAN_MESSAGE &canMessage, long timeOut){};

	/**@brief Waits until all CAN messages for the specified handle are sent, or the timeout period expires.
	  * @param busHandle Channel handle as defined by openCanChannel(int channelId)
          * @param timeOut Timeout time in 
          * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	 */
	virtual STATUS_CODE sendCanMessageSync(CHANNEL_HANDLE busHandle,long timeOut){};

        /**
          * @brief Reads CAN message and puts it to buffer
          * @param busHandle Channel handle as defined by openCanChannel(int channelId)
          * @param canMessage Adress where to store recieved CAN message
          * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
          */
	virtual STATUS_CODE readCanMessage(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessage ){};
        
        /**@brief Waits until message is queued in Rx buffer or timeOut is exceeded
          * @param busHandle Channel handle as defined by openCanChannel(int channelId)
          * @param canMessage Adress to CAN Message struct to be stored
          * @return STATUS_CODE translate via getErrorText(STATUS_CODE currentStatus, string & errorString)
	 *
	 */
	virtual STATUS_CODE readCanMessageWait(CHANNEL_HANDLE busHandle, CAN_MESSAGE &canMessag, unsigned long timeOut ){};
        
        
        /// Deconstructor
	virtual ~CanInterface(){};
};




#endif /* CANINTERFACE_H_ */
