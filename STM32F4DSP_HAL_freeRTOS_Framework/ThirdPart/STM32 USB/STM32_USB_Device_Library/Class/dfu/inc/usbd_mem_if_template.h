/**
  ******************************************************************************
  * @file    usbd_mem_if_template.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   Header for usbd_mem_if_template.c file.
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
#ifndef __MEM_IF_MAL_H
#define __MEM_IF_MAL_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

#include "usbd_dfu_mal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define MEM_START_ADD                 0x00000000 /* Dummy start address */
#define MEM_END_ADD                   (uint32_t)(MEM_START_ADD + (5 * 1024)) /* Dummy Size = 5KB */

#define MEM_IF_STRING                 "@Dummy Memory   /0x00000000/01*002Kg,03*001Kg"

extern DFU_MAL_Prop_TypeDef DFU_Mem_cb;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MEM_IF_MAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
