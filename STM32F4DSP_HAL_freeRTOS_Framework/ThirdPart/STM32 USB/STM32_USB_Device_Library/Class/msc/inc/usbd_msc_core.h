/**
  ******************************************************************************
  * @file    usbd_msc_core.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   header for the usbd_msc_core.c file
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
#ifndef _USB_MSC_CORE_H_
#define _USB_MSC_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup USBD_MSC_BOT
  * @{
  */
  
/** @defgroup USBD_MSC
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 


/** @defgroup USBD_BOT_Exported_Defines
  * @{
  */ 


#define BOT_GET_MAX_LUN              0xFE
#define BOT_RESET                    0xFF
#define USB_MSC_CONFIG_DESC_SIZ      32

#define MSC_EPIN_SIZE                MSC_MAX_PACKET 
#define MSC_EPOUT_SIZE               MSC_MAX_PACKET 

/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Types
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_MSC_cb;
/**
  * @}
  */ 

/**
  * @}
  */ 
#endif /* _USB_MSC_CORE_H_ */
/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
