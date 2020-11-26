/**
  ******************************************************************************
  * @file    usbd_msc_data.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   header for the usbd_msc_data.c file
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

#ifndef _USBD_MSC_DATA_H_
#define _USBD_MSC_DATA_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USB_INFO
  * @brief general defines for the usb device library file
  * @{
  */ 

/** @defgroup USB_INFO_Exported_Defines
  * @{
  */ 
#define MODE_SENSE6_LEN			 8
#define MODE_SENSE10_LEN		 8
#define LENGTH_INQUIRY_PAGE00		 7
#define LENGTH_FORMAT_CAPACITIES    	20

/**
  * @}
  */ 


/** @defgroup USBD_INFO_Exported_TypesDefinitions
  * @{
  */
/**
  * @}
  */ 



/** @defgroup USBD_INFO_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_INFO_Exported_Variables
  * @{
  */ 
extern const uint8_t MSC_Page00_Inquiry_Data[];  
extern const uint8_t MSC_Mode_Sense6_data[];
extern const uint8_t MSC_Mode_Sense10_data[] ;

/**
  * @}
  */ 

/** @defgroup USBD_INFO_Exported_FunctionsPrototypes
  * @{
  */ 

/**
  * @}
  */ 

#endif /* _USBD_MSC_DATA_H_ */

/**
  * @}
  */ 

/**
* @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
