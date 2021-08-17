################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/Application/Validation/Src/aiPbMgr.c \
../Middlewares/ST/Application/Validation/Src/aiTestUtility.c \
../Middlewares/ST/Application/Validation/Src/aiValidation.c \
../Middlewares/ST/Application/Validation/Src/pb_common.c \
../Middlewares/ST/Application/Validation/Src/pb_decode.c \
../Middlewares/ST/Application/Validation/Src/pb_encode.c \
../Middlewares/ST/Application/Validation/Src/stm32msg.pb.c 

OBJS += \
./Middlewares/ST/Application/Validation/Src/aiPbMgr.o \
./Middlewares/ST/Application/Validation/Src/aiTestUtility.o \
./Middlewares/ST/Application/Validation/Src/aiValidation.o \
./Middlewares/ST/Application/Validation/Src/pb_common.o \
./Middlewares/ST/Application/Validation/Src/pb_decode.o \
./Middlewares/ST/Application/Validation/Src/pb_encode.o \
./Middlewares/ST/Application/Validation/Src/stm32msg.pb.o 

C_DEPS += \
./Middlewares/ST/Application/Validation/Src/aiPbMgr.d \
./Middlewares/ST/Application/Validation/Src/aiTestUtility.d \
./Middlewares/ST/Application/Validation/Src/aiValidation.d \
./Middlewares/ST/Application/Validation/Src/pb_common.d \
./Middlewares/ST/Application/Validation/Src/pb_decode.d \
./Middlewares/ST/Application/Validation/Src/pb_encode.d \
./Middlewares/ST/Application/Validation/Src/stm32msg.pb.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/Application/Validation/Src/%.o: ../Middlewares/ST/Application/Validation/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32L476xx -DARM_MATH_CM4 '-D__FPU_PRESENT=1' -I"D:/微處理機系統實驗/Final project/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Device/ST/STM32L4xx/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/Application/Validation/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/SupportFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/MatrixFunctions" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/DSP/Source/BasicMathFunctions" -I"D:/微處理機系統實驗/Final project/Middlewares/ST/AI/Inc" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include" -I"D:/微處理機系統實驗/Final project/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


