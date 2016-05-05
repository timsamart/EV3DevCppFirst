################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Control.cpp \
../src/ev3dev.cpp \
../src/main.cpp 

OBJS += \
./src/Control.o \
./src/ev3dev.o \
./src/main.o 

CPP_DEPS += \
./src/Control.d \
./src/ev3dev.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-linux-gnueabi-g++ -mcpu=arm1026ej-s -mthumb -O2  -g -std=gnu++11 -fabi-version=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


