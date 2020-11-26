/**
  ******************************************************************************
  * @file    usbd_flash_if.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_flash_if.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_IF_MAL_H
#define __FLASH_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_mal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_START_ADD                  0x08000000

#ifdef STM32F2XX
 #define FLASH_END_ADD                   0x08100000
 #define FLASH_IF_STRING                 (unsigned char *)"@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg"
#elif defined(STM32F40_41xxx)
 #define FLASH_END_ADD                   0x08100000
 #define FLASH_IF_STRING                 (unsigned char *) "@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg"

#elif defined(STM32F429_439xx)
 #define FLASH_END_ADD                   0x08200000
 #define FLASH_IF_STRING                  (unsigned char *) "@Internal Flash   /0x08000000/03*016Ka,01*016Kg,01*064Kg,07*128Kg,04*016Kg,01*064Kg,07*128Kg"


#elif defined(STM32F10X_CL)
 #define FLASH_END_ADD                   0x08040000
 #define FLASH_IF_STRING                 (unsigned char *) "@Internal Flash   /0x08000000/06*002Ka,122*002Kg"
#endif /* STM32F2XX */


extern DFU_MAL_Prop_TypeDef DFU_Flash_cb;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __FLASH_IF_MAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
