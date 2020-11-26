/**
  ******************************************************************************
  * @file    usbd_hid_core.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   header file for the usbd_hid_core.c file.
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

#ifndef __USB_HID_CORE_H_
#define __USB_HID_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */ 
#define USB_HID_CONFIG_DESC_SIZ       34
#define USB_HID_DESC_SIZ              9
#define HID_MOUSE_REPORT_DESC_SIZE    74

#define HID_DESCRIPTOR_TYPE           0x21
#define HID_REPORT_DESC               0x22

#define HID_HS_BINTERVAL              0x07
#define HID_FS_BINTERVAL              0x0A

#define HID_REQ_SET_PROTOCOL          0x0B
#define HID_REQ_GET_PROTOCOL          0x03

#define HID_REQ_SET_IDLE              0x0A
#define HID_REQ_GET_IDLE              0x02

#define HID_REQ_SET_REPORT            0x09
#define HID_REQ_GET_REPORT            0x01
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

extern USBD_Class_cb_TypeDef  USBD_HID_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE  *pdev, 
                                 uint8_t *report,
                                 uint16_t len);

uint32_t USBD_HID_GetPollingInterval (USB_OTG_CORE_HANDLE *pdev);

/**
  * @}
  */ 

#endif  /*  __USB_HID_CORE_H_ */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
