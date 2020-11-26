/**
  ******************************************************************************
  * @file    usbd_otp_if.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_otp_if.c file.
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
#ifndef __OTP_IF_MAL_H
#define __OTP_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_mal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OTP_START_ADD                  0x1FFF7800             
#define OTP_END_ADD                    (uint32_t)(OTP_START_ADD + 528) 

#define OTP_IF_STRING                  (uint8_t*)"@OTP Area   /0x1FFF7800/01*512 g,01*016 g"

extern DFU_MAL_Prop_TypeDef DFU_Otp_cb;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __OTP_IF_MAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
