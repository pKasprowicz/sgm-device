################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MQTT/samples/MQTTAsync_publish.c \
../MQTT/samples/MQTTAsync_subscribe.c \
../MQTT/samples/pahopub.c \
../MQTT/samples/pubasync.c \
../MQTT/samples/pubsync.c \
../MQTT/samples/stdinpub.c \
../MQTT/samples/stdinpuba.c \
../MQTT/samples/stdoutsub.c \
../MQTT/samples/stdoutsuba.c \
../MQTT/samples/subasync.c 

OBJS += \
./MQTT/samples/MQTTAsync_publish.o \
./MQTT/samples/MQTTAsync_subscribe.o \
./MQTT/samples/pahopub.o \
./MQTT/samples/pubasync.o \
./MQTT/samples/pubsync.o \
./MQTT/samples/stdinpub.o \
./MQTT/samples/stdinpuba.o \
./MQTT/samples/stdoutsub.o \
./MQTT/samples/stdoutsuba.o \
./MQTT/samples/subasync.o 

C_DEPS += \
./MQTT/samples/MQTTAsync_publish.d \
./MQTT/samples/MQTTAsync_subscribe.d \
./MQTT/samples/pahopub.d \
./MQTT/samples/pubasync.d \
./MQTT/samples/pubsync.d \
./MQTT/samples/stdinpub.d \
./MQTT/samples/stdinpuba.d \
./MQTT/samples/stdoutsub.d \
./MQTT/samples/stdoutsuba.d \
./MQTT/samples/subasync.d 


# Each subdirectory must supply rules for building sources it contributes
MQTT/samples/%.o: ../MQTT/samples/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 4.8.3 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc -I"/home/pkasprow/git/sgmeteo/misc/MQTT" -I/home/pkasprow/openssl-cross/final/include/ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


