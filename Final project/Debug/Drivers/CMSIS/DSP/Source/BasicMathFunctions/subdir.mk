################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.c \
D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.c \
D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.c 

OBJS += \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.o: D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 '-D__FPU_PRESENT=1' -I"D:/微處理機系統實驗/Final project/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/Application/Validation/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/SupportFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/MatrixFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/BasicMathFunctions" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/AI/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_dot_prod_f32.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.o: D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 '-D__FPU_PRESENT=1' -I"D:/微處理機系統實驗/Final project/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/Application/Validation/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/SupportFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/MatrixFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/BasicMathFunctions" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/AI/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.o: D:/Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 '-D__FPU_PRESENT=1' -I"D:/微處理機系統實驗/Final project/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/Application/Validation/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/SupportFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/MatrixFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/BasicMathFunctions" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/AI/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q7.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


