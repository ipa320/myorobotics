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

#include "MotorDriverSPI.h"

#define SLAVE_SELECT(x) (1 << (x))
#define WRITE_DATA      1
#define READ_DATA       0


using namespace std;

MotorDriverSPI::MotorDriverSPI() {
    dutyCycle = 0;
    ftHandle = getFT_HANDLE();
}

MotorDriverSPI::~MotorDriverSPI() {
    sendDutyCycle(0);
}

FT_HANDLE MotorDriverSPI::getFT_HANDLE(){
    static FT_HANDLE handle = initSPI();
    return handle;
}

FT_HANDLE MotorDriverSPI::initSPI() {
    cout << "init spi" << endl;
    FT_STATUS ftStatus;
    FT4222_STATUS ft4222Status;
    FT_HANDLE handle;
    ftStatus = FT_Open(0, &handle);
    if(ftStatus == FT_OK) {
        cout<<"SPI-Port open"<<endl;
    } else {
        cout<<"Error while opening SPI-Port. Errorcode: "<<ftStatus<<endl;
        return NULL;
    }
    cout << "setting up FT4222 Device as Master... " << endl;
    ft4222Status = FT4222_SPIMaster_Init(
                        ftHandle,
                        SPI_IO_SINGLE, // 1 channel
                        CLK_DIV_16, // 60 MHz / 32 == 1.875 MHz
                        CLK_IDLE_LOW, // clock idles at logic 0
                        CLK_TRAILING,
                        SLAVE_SELECT(0)); // Use SS0O for slave-select
    if(ft4222Status != 0){
      cout<<"Error while stting up Master. Errorcode: "<<ft4222Status<<endl;
    }
    return handle;
}

STATUS_CODE MotorDriverSPI::sendDutyCycle(int pwm) {
    dutyCycle = pwm;
    return sendFrame(pwm);
}

STRUCT_SENSOR_VALUES MotorDriverSPI::getSensorValues() {
    sendFrame(dutyCycle);
    return MotorDriverSPI::sV;
}

STATUS_CODE MotorDriverSPI::sendFrame(int pwm) {
    FT4222_STATUS  ft4222Status;
    FT_STATUS ftStatus;
    uint16 BytesWritten;


    for(int i= 0; i<24;i++)
      spistreamWrite.TxBuffer[i] = 0;

    MotorDriverSPI::spistreamWrite.pwmRef = pwm;

    MotorDriverSPI::prepareData(MotorDriverSPI::spistreamWrite,WRITE_DATA);

    ft4222Status = FT4222_SPIMaster_SingleReadWrite(MotorDriverSPI::ftHandle,
                                                    MotorDriverSPI::spistreamRead.TxBuffer , MotorDriverSPI::spistreamWrite.TxBuffer, sizeof(spistreamWrite.TxBuffer),
                                                    &BytesWritten, true);

    MotorDriverSPI::prepareData(spistreamRead, READ_DATA);
    MotorDriverSPI::sV.position = MotorDriverSPI::spistreamRead.actualPosition;
    MotorDriverSPI::sV.velocity = MotorDriverSPI::spistreamRead.actualVelocity;
    MotorDriverSPI::sV.current = MotorDriverSPI::spistreamRead.actualCurrent;
    MotorDriverSPI::sV.displacement = MotorDriverSPI::spistreamRead.springDisplacement;
}

void MotorDriverSPI::prepareData(SPISTREAM &spistream, int datatype)
{

  if(datatype == WRITE_DATA)
    {
    spistream.TxBuffer[0] = 0x80;
    spistream.TxBuffer[1] = 0x00;

    for(int i = 2; i<6; i= i+2)
    {
      MotorDriverSPI::swap(spistream.TxBuffer[i], spistream.TxBuffer[i+1]);
    }

    for(int i =2; i< 8; i = i+2)
      spistream.TxBuffer[i] &= ~(1<<7);
  }
  else if(datatype == READ_DATA)
  {

    int tmp = 0;

    MotorDriverSPI::swap(spistream.TxBuffer[10],spistream.TxBuffer[13]);
    MotorDriverSPI::swap(spistream.TxBuffer[11],spistream.TxBuffer[12]);

    for(int i = 14; i< 24; i= i+2)
      MotorDriverSPI::swap(spistream.TxBuffer[i],spistream.TxBuffer[i+1]);
  }
}

void MotorDriverSPI::swap(uint8 &source, uint8 &target)
{
  uint8 buffer = 0;
  buffer = target;
  target = source;
  source = buffer;
}
