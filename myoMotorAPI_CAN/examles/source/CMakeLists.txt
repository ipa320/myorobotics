PROJECT(example)

ADD_EXECUTABLE( example
	main.cpp
	MotorDriver.h
)

TARGET_LINK_LIBRARIES(example MyoMotorAPI_CAN.so)