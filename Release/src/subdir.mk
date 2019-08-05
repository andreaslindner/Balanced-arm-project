################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/calibrate.c \
../src/cr_startup_lpc11xx.c \
../src/crp.c \
../src/crypto.c \
../src/fonction.c \
../src/i2c.c \
../src/imu.c \
../src/lMotor.c \
../src/motor.c \
../src/pinint.c \
../src/rMotor.c \
../src/step4.c \
../src/sysinit.c \
../src/timer.c \
../src/uart.c 

OBJS += \
./src/calibrate.o \
./src/cr_startup_lpc11xx.o \
./src/crp.o \
./src/crypto.o \
./src/fonction.o \
./src/i2c.o \
./src/imu.o \
./src/lMotor.o \
./src/motor.o \
./src/pinint.o \
./src/rMotor.o \
./src/step4.o \
./src/sysinit.o \
./src/timer.o \
./src/uart.o 

C_DEPS += \
./src/calibrate.d \
./src/cr_startup_lpc11xx.d \
./src/crp.d \
./src/crypto.d \
./src/fonction.d \
./src/i2c.d \
./src/imu.d \
./src/lMotor.d \
./src/motor.d \
./src/pinint.d \
./src/rMotor.d \
./src/step4.d \
./src/sysinit.d \
./src/timer.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__LPC11XX__ -D__REDLIB__ -I"/home/bcoste/Stock/PRe/LCPXpresso11C24-dev/workspace/wolfssl" -I"/home/bcoste/Stock/PRe/LCPXpresso11C24-dev/workspace/CMSIS_CORE_LPC11xx/inc" -I"/home/bcoste/Stock/PRe/LCPXpresso11C24-dev/workspace/step4/inc" -I"/home/bcoste/Stock/PRe/LCPXpresso11C24-dev/workspace/lpc_chip_11cxx_lib/inc" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


