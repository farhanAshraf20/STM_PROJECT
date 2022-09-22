/**
  ******************************************************************************
  * @file    vl53l0x_proximity.h
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VL53L0X_PROXIMITY_H
#define VL53L0X_PROXIMITY_H
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "vl53l0x_tof.h"
#include "vl53l0x_api.h"
/* Defines -------------------------------------------------------------------*/
extern I2C_HandleTypeDef hI2cHandler;
#define PROXIMITY_I2C_ADDRESS         ((uint16_t)0x0052)
#define VL53L0X_ID                    ((uint16_t)0xEEAA)

VL53L0X_Dev_t Dev =
{
  .I2cHandle = &hI2cHandler,
  .I2cDevAddr = PROXIMITY_I2C_ADDRESS
};

static void VL53L0X_PROXIMITY_Init(void);
static uint16_t VL53L0X_PROXIMITY_GetDistance(void);
static void VL53L0X_PROXIMITY_MspInit(void);
void Proximity_Test(void);
extern void     SENSOR_IO_Init(void);
#endif /* VL53L0X_PROXIMITY_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
