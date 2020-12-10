/**
  ******************************************************************************
  * @file    usbh_usr.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file is the header file for usb usr file
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USH_USR_H__
#define __USH_USR_H__


/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h" 
#include "usbh_hid_core.h"
#include "usbh_msc_core.h"
//#include <stdio.h>

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_HID_DEMO_USER_CALLBACKS
* @{
*/
  
/** @defgroup USBH_USR 
  * @brief This file is the header file for user action
  * @{
  */ 


/** @defgroup USBH_CORE_Exported_Variables
  * @{
  */ 
  
  
enum sys_USBH_use_for_enum
{
	use_for_MSC = 1,
	use_for_VCP,
	use_for_HID,
	
	//...
};

extern const enum sys_USBH_use_for_enum sys_USBH_use_for_;

u8 sys_USBH_User_Init(void);

extern  USBH_Usr_cb_TypeDef USR_Callbacks;
//表示USB连接状态
//0,没有连接;
//1,已经连接;
extern vu8 bDeviceState;		//默认没有连接

extern USBH_HOST  USB_Host;
extern USB_OTG_CORE_HANDLE	USB_OTG_Core;
//extern HID_Machine_TypeDef	HID_Machine;	//放到了 usbh_hid_core.h里面

//自行添加的函数
u8 USBH_Check_EnumeDead(USBH_HOST *phost);
//u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm);	//放到了 usbh_hid_core.h里面
void USBH_HID_Reconnect(void);


/* State Machine for the USBH_USR_ApplicationState */
#define USH_USR_FS_INIT       	0
#define USH_USR_FS_TEST   		1  

int USBH_USR_MSC_Application(void);
u8 USBH_UDISK_Status(void);
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt);
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt);

/**
  * @}
  */ 


/** @defgroup USBH_CORE_Exported_FunctionsPrototype
  * @{
  */ 

void USBH_USR_ApplicationSelected(void);
void USBH_USR_Init(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected (void);
void USBH_USR_OverCurrentDetected (void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed); 
void USBH_USR_Device_DescAvailable(void *);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc);
void USBH_USR_Manufacturer_String(void *);
void USBH_USR_Product_String(void *);
void USBH_USR_SerialNum_String(void *);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);



/**
  * @}
  */ 

#endif /* __USBH_USR_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
