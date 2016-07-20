
PROJECT(MyoMotorAPI_CAN)

ADD_LIBRARY(MyoMotorAPI_CAN SHARED
	CanInterface.h 
	KvaserCanInterface.h 
	KvaserCanInterface.cpp
	MotorDriverCAN.cpp 
	MotorDriverCAN.h 
	MotorDriver.h 
	MotorDriver.cpp 
)

TARGET_LINK_LIBRARIES(MyoMotorAPI_CAN canlib)