################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_calibration.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_core.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_ranging.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_strings.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_platform_log.c \
../Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_tof.c 

OBJS += \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_calibration.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_core.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_ranging.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_strings.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_platform_log.o \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_tof.o 

C_DEPS += \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_calibration.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_core.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_ranging.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_api_strings.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_platform_log.d \
./Drivers/BSP/B-L475E-IOT01/vl53l0x/vl53l0x_tof.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/B-L475E-IOT01/vl53l0x/%.o: ../Drivers/BSP/B-L475E-IOT01/vl53l0x/%.c Drivers/BSP/B-L475E-IOT01/vl53l0x/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/Components" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/STM32L475x_IoT_Discovery" -I"C:/Ashraf_MCU/F_I2C/Core/Common/M24SR_wrappers" -I"C:/Ashraf_MCU/F_I2C/Core/Common/NDEF_TagType4_lib" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/B-L475E-IOT01/vl53l0x" -I"C:/Ashraf_MCU/F_I2C/Drivers/BSP/B-L475E-IOT01" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

