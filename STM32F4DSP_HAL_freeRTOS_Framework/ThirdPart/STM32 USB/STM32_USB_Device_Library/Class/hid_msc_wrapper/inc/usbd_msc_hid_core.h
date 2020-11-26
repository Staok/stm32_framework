/**
  ******************************************************************************
  * @file    usbd_msc_hid_core.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   header file for the usbd_msc_hid_core.c file.
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

/* Includes ------------------------------------------------------------------*/

#ifndef __USB_MSC_HID_CORE_H_
#define __USB_MSC_HID_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 
#define HID_INTERFACE 0x0
#define MSC_INTERFACE 0x1

/** @defgroup USBD_HID_Exported_Defines
  * @{
  */ 
#define USB_MSC_HID_CONFIG_DESC_SIZ  (USB_HID_CONFIG_DESC_SIZ -9 + USB_MSC_CONFIG_DESC_SIZ)
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */


/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_MSC_HID_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 

/**
  * @}
  */ 

#endif  /* __USB_HID_CORE_H_ */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
