################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/APSensor.cpp \
../src/SensorManager.cpp \
../src/TemperatureSensor.cpp \
../src/main.cpp 

OBJS += \
./src/APSensor.o \
./src/SensorManager.o \
./src/TemperatureSensor.o \
./src/main.o 

CPP_DEPS += \
./src/APSensor.d \
./src/SensorManager.d \
./src/TemperatureSensor.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	i586-poky-linux-g++ -std=c++0x -I/home/pkasprow/iotdk-ide-linux/./devkit-x86/sysroots/i586-poky-linux/usr/include/upm -I"/home/pkasprow/git/sgmeteo/meteo/inc" -I/home/pkasprow/iotdk-ide-linux/./devkit-x86/sysroots/i586-poky-linux/usr/include/mraa -O0 -g3 -Wall -c -fmessage-length=0 --sysroot="/home/pkasprow/iotdk-ide-linux/./devkit-x86/sysroots/i586-poky-linux" -m32 -march=i586 -c -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


