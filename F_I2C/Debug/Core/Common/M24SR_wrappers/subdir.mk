################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Common/M24SR_wrappers/m24sr.c \
../Core/Common/M24SR_wrappers/m24sr_wrapper.c 

OBJS += \
./Core/Common/M24SR_wrappers/m24sr.o \
./Core/Common/M24SR_wrappers/m24sr_wrapper.o 

C_DEPS += \
./Core/Common/M24SR_wrappers/m24sr.d \
./Core/Common/M24SR_wrappers/m24sr_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Common/M24SR_wrappers/%.o: ../Core/Common/M24SR_wrappers/%.c Core/Common/M24SR_wrappers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/Components" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/STM32L475x_IoT_Discovery" -I"C:/Ashraf_MCU/F_I2C/Core/Common/M24SR_wrappers" -I"C:/Ashraf_MCU/F_I2C/Core/Common/NDEF_TagType4_lib" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/B-L475E-IOT01/vl53l0x" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/B-L475E-IOT01" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

