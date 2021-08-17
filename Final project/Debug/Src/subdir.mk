################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/app_x-cube-ai.c \
../Src/main.c \
../Src/network.c \
../Src/network_data.c \
../Src/stm32l4xx_hal_msp.c \
../Src/stm32l4xx_it.c \
../Src/syscalls.c \
../Src/system_stm32l4xx.c 

OBJS += \
./Src/app_x-cube-ai.o \
./Src/main.o \
./Src/network.o \
./Src/network_data.o \
./Src/stm32l4xx_hal_msp.o \
./Src/stm32l4xx_it.o \
./Src/syscalls.o \
./Src/system_stm32l4xx.o 

C_DEPS += \
./Src/app_x-cube-ai.d \
./Src/main.d \
./Src/network.d \
./Src/network_data.d \
./Src/stm32l4xx_hal_msp.d \
./Src/stm32l4xx_it.d \
./Src/syscalls.d \
./Src/system_stm32l4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 '-D__FPU_PRESENT=1' -I"D:/微處理機系統實驗/Final project/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/Application/Validation/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/SupportFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/MatrixFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/BasicMathFunctions" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/AI/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


