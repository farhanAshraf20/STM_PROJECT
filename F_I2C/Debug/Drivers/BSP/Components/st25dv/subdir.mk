################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/st25dv/st25dv.c \
../Drivers/BSP/Components/st25dv/st25dv_reg.c 

OBJS += \
./Drivers/BSP/Components/st25dv/st25dv.o \
./Drivers/BSP/Components/st25dv/st25dv_reg.o 

C_DEPS += \
./Drivers/BSP/Components/st25dv/st25dv.d \
./Drivers/BSP/Components/st25dv/st25dv_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/st25dv/%.o: ../Drivers/BSP/Components/st25dv/%.c Drivers/BSP/Components/st25dv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/Components" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/B-L475E-IOT01" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

