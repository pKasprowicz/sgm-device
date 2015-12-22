################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTT/Clients.c \
../MQTT/Heap.c \
../MQTT/LinkedList.c \
../MQTT/Log.c \
../MQTT/MQTTAsync.c \
../MQTT/MQTTClient.c \
../MQTT/MQTTPacket.c \
../MQTT/MQTTPacketOut.c \
../MQTT/MQTTPersistence.c \
../MQTT/MQTTPersistenceDefault.c \
../MQTT/MQTTProtocolClient.c \
../MQTT/MQTTProtocolOut.c \
../MQTT/MQTTVersion.c \
../MQTT/Messages.c \
../MQTT/SSLSocket.c \
../MQTT/Socket.c \
../MQTT/SocketBuffer.c \
../MQTT/StackTrace.c \
../MQTT/Thread.c \
../MQTT/Tree.c \
../MQTT/utf-8.c 

OBJS += \
./MQTT/Clients.o \
./MQTT/Heap.o \
./MQTT/LinkedList.o \
./MQTT/Log.o \
./MQTT/MQTTAsync.o \
./MQTT/MQTTClient.o \
./MQTT/MQTTPacket.o \
./MQTT/MQTTPacketOut.o \
./MQTT/MQTTPersistence.o \
./MQTT/MQTTPersistenceDefault.o \
./MQTT/MQTTProtocolClient.o \
./MQTT/MQTTProtocolOut.o \
./MQTT/MQTTVersion.o \
./MQTT/Messages.o \
./MQTT/SSLSocket.o \
./MQTT/Socket.o \
./MQTT/SocketBuffer.o \
./MQTT/StackTrace.o \
./MQTT/Thread.o \
./MQTT/Tree.o \
./MQTT/utf-8.o 

C_DEPS += \
./MQTT/Clients.d \
./MQTT/Heap.d \
./MQTT/LinkedList.d \
./MQTT/Log.d \
./MQTT/MQTTAsync.d \
./MQTT/MQTTClient.d \
./MQTT/MQTTPacket.d \
./MQTT/MQTTPacketOut.d \
./MQTT/MQTTPersistence.d \
./MQTT/MQTTPersistenceDefault.d \
./MQTT/MQTTProtocolClient.d \
./MQTT/MQTTProtocolOut.d \
./MQTT/MQTTVersion.d \
./MQTT/Messages.d \
./MQTT/SSLSocket.d \
./MQTT/Socket.d \
./MQTT/SocketBuffer.d \
./MQTT/StackTrace.d \
./MQTT/Thread.d \
./MQTT/Tree.d \
./MQTT/utf-8.d 


# Each subdirectory must supply rules for building sources it contributes
MQTT/%.o: ../MQTT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4.8.3 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -I"/home/pkasprow/git/sgmeteo/misc/MQTT" -I/home/pkasprow/openssl-cross/final/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


