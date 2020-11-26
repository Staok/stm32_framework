/**
  ******************************************************************************
  * @file    usbd_cdc_core.h
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    17-March-2018
  * @brief   header file for the usbd_cdc_core.c file.
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

#ifndef __USB_CDC_CORE_H_
#define __USB_CDC_CORE_H_

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup usbd_cdc
  * @brief This file is the Header file for USBD_cdc.c
  * @{
  */ 


/** @defgroup usbd_cdc_Exported_Defines
  * @{
  */ 
#define USB_CDC_CONFIG_DESC_SIZ                (67)
#define USB_CDC_DESC_SIZ                       (67-9)

#define DEVICE_CLASS_CDC                        0x02
#define DEVICE_SUBCLASS_CDC                     0x00

#define CDC_DATA_IN_PACKET_SIZE                CDC_DATA_MAX_PACKET_SIZE
        
#define CDC_DATA_OUT_PACKET_SIZE               CDC_DATA_MAX_PACKET_SIZE

/**
  * @}
  */ 
/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct _CDC_IF_PROP
{
  uint16_t (*pIf_DataTx)   (void);
  uint16_t (*pIf_DataRx)   (uint32_t Len);
}
CDC_IF_Prop_TypeDef;
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

extern USBD_Class_cb_TypeDef  USBD_CDC_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/**
  * @}
  */ 

#endif  /* __USB_CDC_CORE_H_ */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
