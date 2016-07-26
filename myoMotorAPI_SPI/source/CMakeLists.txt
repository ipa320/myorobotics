PROJECT(MyoMotorAPI_SPI)

ADD_LIBRARY(MyoMotorAPI_SPI SHARED 
	MotorDriverSPI.cpp
	MotorDriverSPI.h
	MotorDriver.cpp
	MotorDriver.h
	ftd2xx.h
	libft4222.h
)

TARGET_LINK_LIBRARIES(MyoMotorAPI_SPI libftd2xx.so libft4222.so)