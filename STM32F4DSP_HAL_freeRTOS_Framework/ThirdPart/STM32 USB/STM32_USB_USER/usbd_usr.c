#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_dcd_int.h"
#include "PeriphConfigCore.h"

#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  


USB_OTG_CORE_HANDLE  USB_OTG_dev;

/*用户使用，USB Dvice MSC模式 初始化*/
void sys_USBD_User_Init(void)
{
	MSC_BOT_Data = mymalloc(InrRAM,MSC_MEDIA_PACKET);	//申请内存，MSC_MEDIA_PACKET 默认占 InrRAM 32KB
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	HAL_Delay(100);
}

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBD_OTG_ISR_Handler(&USB_OTG_dev);
}
//指向DEVICE_PROP结构体
//USB Device 用户回调函数. 
USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,    
};
//USB Device 用户自定义初始化函数
void USBD_USR_Init(void)
{
	//printf("USBD_USR_Init\r\n");
} 
//USB Device 复位
//speed:USB速度,0,高速;1,全速;其他,错误.
void USBD_USR_DeviceReset (uint8_t speed)
{
	switch (speed)
	{
		case USB_OTG_SPEED_HIGH:
//			printf("USB Device Library v1.1.0  [HS]\r\n");
			FaultASSERT("USB Device Library v1.1.0  [HS]\r\n",0,flag_Warning);
			break; 
		case USB_OTG_SPEED_FULL: 
//			printf("USB Device Library v1.1.0  [FS]\r\n");
			FaultASSERT("USB Device Library v1.1.0  [FS]\r\n",0,flag_Warning);
			break;
		default:
//			printf("USB Device Library v1.1.0  [??]\r\n"); 
			FaultASSERT("USB Device Library v1.1.0  [??]\r\n",0,flag_Warning);
			break;
	}
}
//USB Device 配置成功
void USBD_USR_DeviceConfigured (void)
{
    bDeviceState=1;
//	printf("MSC Interface started.\r\n"); 
	FaultASSERT("MSC Interface started.\r\n",0,flag_Warning);
} 
//USB Device挂起
void USBD_USR_DeviceSuspended(void)
{
    bDeviceState=0;
//	printf("Device In suspend mode.\r\n");
	FaultASSERT("Device In suspend mode.\r\n",0,flag_Warning);
} 
//USB Device恢复
void USBD_USR_DeviceResumed(void)
{ 
//	printf("Device Resumed\r\n");
	FaultASSERT("Device Resumed\r\n",0,flag_Warning);
}
//USB Device连接成功
void USBD_USR_DeviceConnected (void)
{
	bDeviceState=1;
//	printf("USB Device Connected.\r\n");
	FaultASSERT("USB Device Connected.\r\n",0,flag_Warning);
}
//USB Device未连接
void USBD_USR_DeviceDisconnected (void)
{
	bDeviceState=0;
//	printf("USB Device Disconnected.\r\n");
	FaultASSERT("USB Device Disconnected.\r\n",0,flag_Warning);
} 








