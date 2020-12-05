/**
  ******************************************************************************
  * @file    usbd_desc.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file provides the USBD descriptors and string formating method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

#include "usbd_desc.h"
#include "usbd_usr.h"

/*
TODO：下面各个描述符具体会显示在那里，试验试验，然后在这里记录注释一下，在docs.bin记录一下
*/

#define USBD_VID_msc                   	  0x0483
#define USBD_PID_msc                   	  0x5720
#define USBD_LANGID_STRING_msc         	  0x409
#define USBD_MANUFACTURER_STRING_msc	  "STMicroelectronics"
#define USBD_PRODUCT_HS_STRING_msc        "Mass Storage in HS Mode"
#define USBD_PRODUCT_FS_STRING_msc        "Mass Storage in FS Mode"
#define USBD_CONFIGURATION_HS_STRING_msc  "MSC Config"
#define USBD_INTERFACE_HS_STRING_msc      "MSC Interface"
#define USBD_CONFIGURATION_FS_STRING_msc  "MSC Config"
#define USBD_INTERFACE_FS_STRING_msc      "MSC Interface"

#define USBD_VID_vcp                        0x0483
#define USBD_PID_vcp                        0x5740
#define USBD_LANGID_STRING_vcp              0x409
#define USBD_MANUFACTURER_STRING_vcp        "STMicroelectronics"
#define USBD_PRODUCT_HS_STRING_vcp          "STM32 Virtual ComPort in HS mode"
#define USBD_PRODUCT_FS_STRING_vcp          "STM32 Virtual ComPort in FS Mode"
#define USBD_CONFIGURATION_HS_STRING_vcp    "VCP Config"
#define USBD_INTERFACE_HS_STRING_vcp        "VCP Interface"
#define USBD_CONFIGURATION_FS_STRING_vcp    "VCP Config"
#define USBD_INTERFACE_FS_STRING_vcp        "VCP Interface"


USBD_DEVICE USR_desc =
{
  USBD_USR_DeviceDescriptor,
  USBD_USR_LangIDStrDescriptor, 
  USBD_USR_ManufacturerStrDescriptor,
  USBD_USR_ProductStrDescriptor,
  USBD_USR_SerialStrDescriptor,
  USBD_USR_ConfigStrDescriptor,
  USBD_USR_InterfaceStrDescriptor,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceDesc_msc[USB_SIZ_DEVICE_DESC] __ALIGN_END =
{
  0x12,                       /*bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
  0x00,                       /*bcdUSB */
  0x02,
  0x00,                       /*bDeviceClass*/
  0x00,                       /*bDeviceSubClass*/
  0x00,                       /*bDeviceProtocol*/
  USB_OTG_MAX_EP0_SIZE,      /*bMaxPacketSize*/
  LOBYTE(USBD_VID_msc),           /*idVendor*/
  HIBYTE(USBD_VID_msc),           /*idVendor*/
  LOBYTE(USBD_PID_msc),           /*idVendor*/
  HIBYTE(USBD_PID_msc),           /*idVendor*/
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
  USBD_IDX_PRODUCT_STR,       /*Index of product string*/
  USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
  USBD_CFG_MAX_NUM            /*bNumConfigurations*/
} ; /* USB_DeviceDescriptor */

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceDesc_vcp[USB_SIZ_DEVICE_DESC] __ALIGN_END = 
{
  0x12,                         /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,   /* bDescriptorType */
  0x00,                         /* bcdUSB */
  0x02,
  0x02,                         /* bDeviceClass */
  0x02,                         /* bDeviceSubClass */
  0x00,                         /* bDeviceProtocol */
  USB_OTG_MAX_EP0_SIZE,         /* bMaxPacketSize */
  LOBYTE(USBD_VID_vcp),             /* idVendor */
  HIBYTE(USBD_VID_vcp),             /* idVendor */
  LOBYTE(USBD_PID_vcp),             /* idVendor */
  HIBYTE(USBD_PID_vcp),             /* idVendor */
  0x00,                         /* bcdDevice rel. 2.00 */
  0x02,
  USBD_IDX_MFC_STR,             /* Index of manufacturer string */
  USBD_IDX_PRODUCT_STR,         /* Index of product string */
  USBD_IDX_SERIAL_STR,          /* Index of serial number string */
  USBD_CFG_MAX_NUM              /* bNumConfigurations */
};                              /* USB_DeviceDescriptor */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */ /*这个只在HOST会用到*/
__ALIGN_BEGIN uint8_t USBD_DeviceQualifierDesc_msc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_DeviceQualifierDesc_vcp[USB_LEN_DEV_QUALIFIER_DESC]
  __ALIGN_END = {
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_LangIDDesc[USB_SIZ_STRING_LANGID] __ALIGN_END =
{
  USB_SIZ_STRING_LANGID,
  USB_DESC_TYPE_STRING,
  LOBYTE(USBD_LANGID_STRING_msc),
  HIBYTE(USBD_LANGID_STRING_msc), 
};

uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] =
{
  USB_SIZ_STRING_SERIAL,      
  USB_DESC_TYPE_STRING,    
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ] __ALIGN_END ;

/**
  * @}
  */ 


/** @defgroup USBD_DESC_Private_FunctionPrototypes
  * @{
  */ 
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
static void Get_SerialNum(void);
/**
  * @}
  */ 


/** @defgroup USBD_DESC_Private_Functions
  * @{
  */ 

/**
* @brief  USBD_USR_DeviceDescriptor 
*         return the device descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_DeviceDescriptor( uint8_t speed , uint16_t *length)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
				*length = sizeof(USBD_DeviceDesc_msc);
				return (uint8_t*)USBD_DeviceDesc_msc;
		case use_for_VCP:
				*length = sizeof(USBD_DeviceDesc_vcp);
				return (uint8_t*)USBD_DeviceDesc_vcp;
	}
	/*默认按照VCP来*/
	*length = sizeof(USBD_DeviceDesc_vcp);
	return (uint8_t*)USBD_DeviceDesc_vcp;
}

/**
* @brief  USBD_USR_LangIDStrDescriptor 
*         return the LangID string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_LangIDStrDescriptor( uint8_t speed , uint16_t *length)
{
  *length =  sizeof(USBD_LangIDDesc);  
  return (uint8_t*)USBD_LangIDDesc;
}


/**
* @brief  USBD_USR_ProductStrDescriptor 
*         return the product string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_ProductStrDescriptor( uint8_t speed , uint16_t *length)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
			if(speed == 0)
			{   
				USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_HS_STRING_msc, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_FS_STRING_msc, USBD_StrDesc, length);    
			}
			return USBD_StrDesc;
		case use_for_VCP:
			if(speed == 0)
			{   
				USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_HS_STRING_vcp, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_FS_STRING_vcp, USBD_StrDesc, length);    
			}
			return USBD_StrDesc;
	}
	/*默认按照VCP来*/
	if(speed == 0)
	{   
		USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_HS_STRING_vcp, USBD_StrDesc, length);
	}
	else
	{
		USBD_GetString((uint8_t *)(uint8_t *)USBD_PRODUCT_FS_STRING_vcp, USBD_StrDesc, length);    
	}
	return USBD_StrDesc;
}

/**
* @brief  USBD_USR_ManufacturerStrDescriptor 
*         return the manufacturer string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_ManufacturerStrDescriptor( uint8_t speed , uint16_t *length)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
			USBD_GetString((uint8_t *)(uint8_t *)USBD_MANUFACTURER_STRING_msc, USBD_StrDesc, length);
			return USBD_StrDesc;
		case use_for_VCP:
			USBD_GetString((uint8_t *)(uint8_t *)USBD_MANUFACTURER_STRING_vcp, USBD_StrDesc, length);
			return USBD_StrDesc;
	}
	/*默认按照VCP来*/
	USBD_GetString((uint8_t *)(uint8_t *)USBD_MANUFACTURER_STRING_vcp, USBD_StrDesc, length);
	return USBD_StrDesc;
}

/**
* @brief  USBD_USR_SerialStrDescriptor 
*         return the serial number string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_SerialStrDescriptor( uint8_t speed , uint16_t *length)
{
  *length = USB_SIZ_STRING_SERIAL;
  
  /* Update the serial number string descriptor with the data from the unique ID*/
  Get_SerialNum();
  
  return (uint8_t*)USBD_StringSerial;
}

/**
* @brief  USBD_USR_ConfigStrDescriptor 
*         return the configuration string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_ConfigStrDescriptor( uint8_t speed , uint16_t *length)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
			if(speed  == USB_OTG_SPEED_HIGH)
			{  
				USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_HS_STRING_msc, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_FS_STRING_msc, USBD_StrDesc, length); 
			}
			return USBD_StrDesc;
		case use_for_VCP:
			if(speed  == USB_OTG_SPEED_HIGH)
			{  
				USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_HS_STRING_vcp, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_FS_STRING_vcp, USBD_StrDesc, length); 
			}
			return USBD_StrDesc;

	}
	/*默认按照VCP来*/
	if(speed  == USB_OTG_SPEED_HIGH)
	{  
		USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_HS_STRING_vcp, USBD_StrDesc, length);
	}
	else
	{
		USBD_GetString((uint8_t *)(uint8_t *)USBD_CONFIGURATION_FS_STRING_vcp, USBD_StrDesc, length); 
	}
	return USBD_StrDesc;
}


/**
* @brief  USBD_USR_InterfaceStrDescriptor 
*         return the interface string descriptor
* @param  speed : current device speed
* @param  length : pointer to data length variable
* @retval pointer to descriptor buffer
*/
uint8_t *  USBD_USR_InterfaceStrDescriptor( uint8_t speed , uint16_t *length)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
			if(speed == 0)
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_HS_STRING_msc, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_FS_STRING_msc, USBD_StrDesc, length);
			}
			return USBD_StrDesc; 
		case use_for_VCP:
			if(speed == 0)
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_HS_STRING_vcp, USBD_StrDesc, length);
			}
			else
			{
				USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_FS_STRING_vcp, USBD_StrDesc, length);
			}
			return USBD_StrDesc; 

	}
	/*默认按照VCP来*/
	if(speed == 0)
	{
		USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_HS_STRING_vcp, USBD_StrDesc, length);
	}
	else
	{
		USBD_GetString((uint8_t *)(uint8_t *)USBD_INTERFACE_FS_STRING_vcp, USBD_StrDesc, length);
	}
	return USBD_StrDesc; 
}

/**
  * @brief  Create the serial number string descriptor 
  * @param  None 
  * @retval None
  */
static void Get_SerialNum(void)
{
  uint32_t deviceserial0, deviceserial1, deviceserial2;
  
  deviceserial0 = *(uint32_t*)DEVICE_ID1;
  deviceserial1 = *(uint32_t*)DEVICE_ID2;
  deviceserial2 = *(uint32_t*)DEVICE_ID3;
  
  deviceserial0 += deviceserial2;
  
  if (deviceserial0 != 0)
  {
    IntToUnicode (deviceserial0, &USBD_StringSerial[2] ,8);
    IntToUnicode (deviceserial1, &USBD_StringSerial[18] ,4);
  }
}

/**
  * @brief  Convert Hex 32Bits value into char 
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer 
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

