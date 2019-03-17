################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/home/kbisland/TailController/SW4STM32/startup_stm32l432xx.s 

C_SRCS += \
/home/kbisland/TailController/SW4STM32/syscalls.c 

OBJS += \
./Application/SW4STM32/startup_stm32l432xx.o \
./Application/SW4STM32/syscalls.o 

C_DEPS += \
./Application/SW4STM32/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32l432xx.o: /home/kbisland/TailController/SW4STM32/startup_stm32l432xx.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/SW4STM32/syscalls.o: /home/kbisland/TailController/SW4STM32/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L432xx -I"/home/kbisland/TailController/Core/Inc" -I"/home/kbisland/TailController/Drivers/STM32L4xx_HAL_Driver/Inc" -I"/home/kbisland/TailController/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"/home/kbisland/TailController/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"/home/kbisland/TailController/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


