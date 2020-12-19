################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../button.c \
../led.c \
../main.c \
../reversi8.c \
../timer.c \
../timer2.c 

ASM_SRCS += \
../patron_volteo_arm_arm.asm \
../patron_volteo_arm_c.asm 

OBJS += \
./8led.o \
./button.o \
./led.o \
./main.o \
./patron_volteo_arm_arm.o \
./patron_volteo_arm_c.o \
./reversi8.o \
./timer.o \
./timer2.o 

C_DEPS += \
./8led.d \
./button.d \
./led.d \
./main.d \
./reversi8.d \
./timer.d \
./timer2.d 

ASM_DEPS += \
./patron_volteo_arm_arm.d \
./patron_volteo_arm_c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\hlocal2\workspace\Placa\common" -O3 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\hlocal2\workspace\Placa\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


