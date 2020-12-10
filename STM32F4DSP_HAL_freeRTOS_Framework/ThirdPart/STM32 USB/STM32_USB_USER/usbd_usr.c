#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_dcd_int.h"
#include "PeriphConfigCore.h"

#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"

#include "usbd_cdc_core.h"
/*源文件修改记录：在 usbd_cdc_core 里面 把 USBD_DeviceDesc 换成了 USBD_DeviceDesc_vcp*/

#include "usbd_hid_core.h"



/*
选择 USB Device 用于哪种 class
	选项				class
	use_for_MSC			MSC（用于把本机模拟为一个U盘，把本地外挂的SPI FLASH或者SD卡模拟成U盘供电脑端操作）
	use_for_VCP			VCP（用于把本机模拟为一个USB虚拟串口，上位机需要先安装STM32 VCP驱动）
	use_for_mouse		HID（用于把本机模拟为一个鼠标）
	use_for_keyboard	HID（用于把本机模拟为一个键盘）（有待移植的键盘例程：https://blog.csdn.net/xuquanlin_1680/article/details/80502183）
	...
	
	注意只能在开机的时候选择一次，运行时改动无效
	加入新的class的时候，先在 usbd_usr.h 里面加入枚举，再主要对比着修改 usbd_desc.c 和 usbd_conf.h
*/
const enum sys_USBD_use_for_enum sys_USBD_use_for_ = use_for_mouse;

/*USB Device使用说明
连接状况标志位使用例子：
	u8 Divece_STA;
	if(Divece_STA != bDeviceState)
	{
		if(bDeviceState == 1)
			LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"USB Connected    ");//提示USB连接已经建立
		else 
			LCD_ShowString(10,140,tftlcd_data.width,tftlcd_data.height,16,"USB DisConnected ");//提示USB被拔出了
		Divece_STA = bDeviceState;
	}
VCP模式（USB模拟虚拟串口）：
	在 usbd_cdc_vcp.c 文件里面：
		接收：
			从USB口接收数据后会自动调用 uint16_t VCP_DataRx(uint8_t * Buf, uint32_t Len) 函数
			只需构造一个数据存在里面就行，然后标个标志位
			
		发送：
			目前暂用一个一个字节的发送。
			先在 APP_Rx_Buffer[APP_Rx_ptr_in] 里面存一个字节
			然后调用 uint16_t VCP_DataTx(void) 函数一次
			例如：
				APP_Rx_Buffer[APP_Rx_ptr_in] = aRxBuffer[0];
				VCP_DataTx();
		设定要模拟的串口的模式：
			uint16_t VCP_Ctrl(uint32_t Cmd, uint8_t * Buf, uint32_t Len)
			
			
MSC模式（大容量存储）：
	就像FATFS的diskio.c定义一样，在 usbd_storage_msd.c 文件里面：
		定义有几个设备： #define STORAGE_LUN_NBR 	2
		填写 标准查询数据(每个lun占36字节)： STORAGE_Inquirydata[]
		完成这几个函数：
			int8_t STORAGE_Init (uint8_t lun);
			int8_t STORAGE_GetCapacity (uint8_t lun,uint32_t *block_num,uint32_t *block_size);
			int8_t  STORAGE_IsReady (uint8_t lun);
			int8_t  STORAGE_IsWriteProtected (uint8_t lun);
			int8_t STORAGE_Read (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len);
			int8_t STORAGE_Write (uint8_t lun,uint8_t *buf,uint32_t blk_addr,uint16_t blk_len);
			int8_t STORAGE_GetMaxLun (void);
			
			STORAGE_GetMaxLun()要注意，里面是先检测SD卡在不在线，如果在则返回 (STORAGE_LUN_NBR - 1)，表示有两个设备 0和1
				如果SD卡不在，则返回 (STORAGE_LUN_NBR - 2)，表示只有一个设备（SPI FLASH），标号0
		查询标志位使用举例：
			if(USB_STATUS_REG)	//状态改变了，状态会在 usbd_storage_msd.c 文件里面被回调函数自动改变
			{
				USB_STATUS_REG = 0;
				LCD_Fill(10,160,240,160+16,WHITE);//清除显示			  	   
				if(USB_STATUS_REG&0x01)//正在写		  
				{
					LCD_ShowString(10,160,tftlcd_data.width,tftlcd_data.height,16,"USB Writing...");//提示USB正在写入数据	 
				}
				
				if(USB_STATUS_REG&0x02)//正在读
				{
					LCD_ShowString(10,160,tftlcd_data.width,tftlcd_data.height,16,"USB Reading...");//提示USB正在读出数据  		 
				}	 										  
				
				if(USB_STATUS_REG&0x04)
					LCD_ShowString(10,180,tftlcd_data.width,tftlcd_data.height,16,"USB Write Err ");//提示写入错误
				else 
					LCD_Fill(10,180,240,180+16,WHITE);//清除显示	  
				
				if(USB_STATUS_REG&0x08)
					LCD_ShowString(10,210,tftlcd_data.width,tftlcd_data.height,16,"USB Read  Err ");//提示读出错误
				else 
					LCD_Fill(10,210,240,210+16,WHITE);//清除显示    
				
			}
			
			
HID（人机接口）：可以模拟鼠标（点击，xy移动和滚轮）或者键盘
		
		有待移植的键盘例程：https://blog.csdn.net/xuquanlin_1680/article/details/80502183
		鼠标控制举例：
			uint32_t counter = 0;
			char buf[4] = {0};		//不能带有unsigned标识符！
			//这段默认放在1ms循环执行里，USBD_HID_GetPollingInterval(&USB_OTG_dev)为10，即10ms发送一次数据
			if (counter++ >= USBD_HID_GetPollingInterval(&USB_OTG_dev))
			{
				//buf 的四个字节（0 1 2 3）分别是 鼠标单击，x，y，滚轮
				//	说明：	x\y正值左上，负值右下
				buf[0] = 0;buf[3] = 0;
				buf[1] = 1;buf[2] = -1;
				USBD_HID_SendReport(&USB_OTG_dev, (uint8_t*)buf, 4);
				counter = 0;
			}
*/


/*以下都是固定的，不用动*/

//表示USB连接状态
//这个只是用来标记的，用户可以查询，没其他作用
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  


USB_OTG_CORE_HANDLE  USB_OTG_dev;

/*
用户使用，USB Dvice初始化
返回0成功，返回其他失败
*/
u8 sys_USBD_User_Init(void)
{
	switch(sys_USBD_use_for_)
	{
		case use_for_MSC:
			MSC_BOT_Data = mymalloc(InrRAM,MSC_MEDIA_PACKET);	//申请内存，MSC_MEDIA_PACKET 默认占 InrRAM 12KB
			if(MSC_BOT_Data == NULL) return 1;
						/*USB句柄	      识别HS还是FS	描述符(在usbd_desc.c里面)   MSC(在MSC class里面)	 用户回调函数定义*/
			USBD_Init(&USB_OTG_dev,		USB_OTG_FS_CORE_ID,		&USR_desc,				&USBD_MSC_cb,			&USR_cb);
			break;
		
		case use_for_VCP:
			USBD_Init(&USB_OTG_dev,		USB_OTG_FS_CORE_ID,		&USR_desc,				&USBD_CDC_cb, 			&USR_cb);
			break;
		case use_for_keyboard:
		case use_for_mouse:
			USBD_Init(&USB_OTG_dev,		USB_OTG_FS_CORE_ID,		&USR_desc,				&USBD_HID_cb, 			&USR_cb);
			break;
			
		
		default:
			return 0xfe;
	}
	HAL_Delay(100);
	
	return 0;
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
//USB Device 用户自定义初始化函数，没有用到
void USBD_USR_Init(void)
{
	//FaultASSERT("USBD_USR_Init\r\n",0,flag_Warning);
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








