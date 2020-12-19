################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../8led.c \
../botones_antirebotes.c \
../button.c \
../excepciones.c \
../jugada.c \
../latido.c \
../led.c \
../main.c \
../pila.c \
../reversi8.c \
../timer.c \
../timer1.c \
../timer2.c \
../timer4.c 

ASM_SRCS += \
../patron_volteo_arm_arm.asm \
../patron_volteo_arm_c.asm 

OBJS += \
./8led.o \
./botones_antirebotes.o \
./button.o \
./excepciones.o \
./jugada.o \
./latido.o \
./led.o \
./main.o \
./patron_volteo_arm_arm.o \
./patron_volteo_arm_c.o \
./pila.o \
./reversi8.o \
./timer.o \
./timer1.o \
./timer2.o \
./timer4.o 

C_DEPS += \
./8led.d \
./botones_antirebotes.d \
./button.d \
./excepciones.d \
./jugada.d \
./latido.d \
./led.d \
./main.d \
./pila.d \
./reversi8.d \
./timer.d \
./timer1.d \
./timer2.d \
./timer4.d 

ASM_DEPS += \
./patron_volteo_arm_arm.d \
./patron_volteo_arm_c.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\hlocal2\workspace\Placa\common" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -mapcs-frame -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\hlocal2\workspace\Placa\common" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


