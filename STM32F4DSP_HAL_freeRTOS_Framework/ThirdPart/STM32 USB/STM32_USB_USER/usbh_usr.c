#include "usbh_usr.h"
#include "usb_hcd_int.h"
#include "PeriphConfigCore.h"

#include "ff.h"	//用于MSC
#include "fatfs_app.h"

#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"  
/*
源文件修改记录：
	usbh_hid_core.c 里面 587行：phost->Control.setup.b.wLength.w = 100;		//这里由0修改为 100 以后,识别率明显提高
						 395行：classReqStatus = USBH_Set_Idle (pdev, pphost, 100, 0);	//由0修改为100,提高兼容性
					在其.h文件里面，131行，加入两句有关 HID_Machine_TypeDef 的声明，因为放在本文件的.h里面总莫名报错
	usbh_hid_keybd.h 里面，要修改键盘类型的定义，选择：#define QWERTY_KEYBOARD		//通用键盘
	usbh_hid_mouse.c 里面，123行的 MOUSE_Decode() 函数修改
*/


/*
选择 USB HSOT 用于哪种 class
	选项				class
	use_for_MSC			MSC（用于外部USB挂载FATFS文件系统）
	use_for_VCP			VCP（USB HOST不支持 VCP，HOST这么腻害，就用于VCP太浪费了）
	use_for_HID			HID（目前仅支持鼠标和键盘）
	...
	
	注意只能在开机的时候选择一次，运行时改动无效
	加入新的class的时候，先在 usbh_usr.h 里面加入枚举，再主要对比着修改 usbh_usr.c、usbh_usr.h 和 usbh_conf.h
*/
const enum sys_USBH_use_for_enum sys_USBH_use_for_ = use_for_HID;

/*
USB HOST使用说明
必须在一个大循环里添加以下程序：（所在循环里不能有延时）（默认已经在BareConfig.c的循环里添加）
		#if ((SYSTEM_USB_ENABLE) && (USE_HOST_MODE))
			//Host Task handler
			//USBH_Process 函数仅在 U 盘识别阶段，需要频繁反复调用，但是当 U 盘被识别后，剩下的操作（U 盘读写），都可以由 USB 中断处理。
			USBH_Process(&USB_OTG_Core, &USB_Host);
			
			if(sys_USBH_use_for_ == use_for_HID)	//只有在使用HOST HID的时候进行死机检测并处理
			{
				if(bDeviceState==1)//连接建立了
				{
					if(USBH_Check_HIDCommDead(&USB_OTG_Core,&HID_Machine))//检测USB HID通信,是否还正常? 
					{ 	    
						USBH_HID_Reconnect();//重连
					}					
				}
				else	//连接未建立的时候,检测
				{
					if(USBH_Check_EnumeDead(&USB_Host))	//检测USB HOST 枚举是否死机了?死机了,则重新初始化 
					{ 	    
						USBH_HID_Reconnect();//重连
					}		
				}
			}
			
			//检查设备是否连接
			if(HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				
			}
		#endif

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
	USB HOST不支持 VCP，HOST这么腻害，就用于VCP太浪费了


MSC模式（大容量存储）：
	必须打开FATFS启用的宏 SYSTEM_FATFS_ENABLE
	当U盘插入USB HOST接口后，会执行下面的用户程序一次，在此程序中默认会用FATS挂载 DEV_USB设备，即挂载USB UOST MSC文件设备（U盘）
	u8 USH_MSC_User_App(void)
	
	成功后就可以用FATFS的API操作通过USB MSC挂载的外部U盘了
	
	
HID（人机接口）：可以模拟鼠标（点击，xy移动和滚轮）或者键盘
	在此文件的最下面，需要完成四个用户API程序：
		若在USB HOST接口插入的是 鼠标HID设备，则在以下函数内填入此设备插入后执行一次的用户程序（刷新相应的界面，标志位等等）
		void USR_MOUSE_Init	(void)

		若在USB HOST接口插入的是 键盘HID设备，则在以下函数内填入此设备插入后执行一次的用户程序（刷新相应的界面，标志位等等）
		void  USR_KEYBRD_Init(void)
		
		鼠标和键盘的用户应用程序，每当设备输入一次，则会被回调一次
		void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
		void  USR_KEYBRD_ProcessData (uint8_t data)
	
	以上四个API的原型在 usbh_hid_keybd.c 和 usbh_hid_mouse.c 里面
*/


//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

static u8 AppState = USH_USR_FS_INIT;

/*
用户使用，USB HOST 初始化
返回0成功，返回其他失败
*/
u8 sys_USBH_User_Init(void)
{
	switch(sys_USBH_use_for_)
	{
		case use_for_MSC:
						/*USB句柄	      识别HS还是FS		 USB_Host信息结构体     MSC(在MSC class里面)	 用户回调函数定义*/
			USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID,	&USB_Host,				&USBH_MSC_cb,			&USR_Callbacks);
			break;
		case use_for_VCP:
			FaultASSERT("USB HOST不支持 VCP，HOST这么腻害，就用于VCP太浪费了\r\n\r\n",NULL,flag_Warning);
			break;
		case use_for_HID:
			USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID,	&USB_Host,				&HID_cb,				&USR_Callbacks);
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
	USBH_OTG_ISR_Handler(&USB_OTG_Core);
}  

//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};
 
//USB HOST 初始化 
void USBH_USR_Init(void)
{
//	printf("USB OTG FS MSC Host\r\n");
//	printf("> USB Host library started.\r\n");
//	printf("  USB Host Library v2.1.0\r\n\r\n");
	FaultASSERT("USB OTG FS MSC Host\r\n",NULL,flag_Warning);
	FaultASSERT("> USB Host library started.\r\n",NULL,flag_Warning);
	FaultASSERT("  USB Host Library v2.1.0\r\n\r\n",NULL,flag_Warning);
	
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{ 
//	printf("检测到USB设备插入!\r\n");
	FaultASSERT("检测到USB设备插入!\r\n",NULL,flag_Warning);
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{ 
//	printf("USB设备拔出!\r\n");
	FaultASSERT("USB设备拔出!\r\n",NULL,flag_Warning);
	bDeviceState=0;	//USB设备拔出了
	
	if(sys_USBH_use_for_ == use_for_MSC)
	{
		f_mount(0,"DEV_USB:",1); 	//卸载U盘
	}
}
//复位从机
void USBH_USR_ResetDevice(void)
{
//	printf("复位设备...\r\n");
	FaultASSERT("复位设备...\r\n",NULL,flag_Warning);
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
//		printf("高速(HS)USB设备!\r\n");
		FaultASSERT("高速(HS)USB设备!\r\n",NULL,flag_Warning);
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
//		printf("全速(FS)USB设备!\r\n"); 
		FaultASSERT("全速(FS)USB设备!\r\n",NULL,flag_Warning);
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
//		printf("低速(LS)USB设备!\r\n");  
		FaultASSERT("低速(LS)USB设备!\r\n",NULL,flag_Warning);
	}
	else
	{
//		printf("设备错误!\r\n");  
		FaultASSERT("设备错误!\r\n",NULL,flag_Warning);
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
	USBH_DevDesc_TypeDef *hs;
	hs=DeviceDesc;   
//	printf_("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
//	printf_("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
	printf_uart(UART1,"VID: %04Xh\r\n", (uint32_t)(*hs).idVendor);
	printf_uart(UART1,"PID: %04Xh\r\n", (uint32_t)(*hs).idProduct);
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
//	printf("从机地址分配成功!\r\n");  
	FaultASSERT("从机地址分配成功!\r\n",NULL,flag_Warning);	
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
//		printf("可移动存储器设备!\r\n"); 
		FaultASSERT("可移动存储器设备!\r\n",NULL,flag_Warning);
	}else if((*id).bInterfaceClass==0x03)
	{
//		printf("HID 设备!\r\n"); 
		FaultASSERT("HID 设备!\r\n",NULL,flag_Warning);
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
	printf_uart(UART1,"Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
//	printf("Product: %s\r\n",(char *)ProductString);  
	printf_uart(UART1,"Product: %s\r\n", (char *)ProductString);
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
	printf_uart(UART1,"Serial Number: %s\r\n", (char *)SerialNumString);
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{ 
//	printf("设备枚举完成!\r\n\r\n");   
	FaultASSERT("设备枚举完成!\r\n\r\n",NULL,flag_Warning);
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{ 
//	USBH_Msg_Show(3);//无法识别的USB设备
//	printf("无法识别的USB设备!\r\n\r\n"); 
	FaultASSERT("无法识别的USB设备!\r\n\r\n",NULL,flag_Warning);
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
//	printf("跳过用户确认步骤!\r\n");
	FaultASSERT("跳过用户确认步骤!\r\n",NULL,flag_Warning);
	bDeviceState=1;//USB设备已经连接成功
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
//	printf("端口电流过大!!!\r\n");
	FaultASSERT("端口电流过大!!!\r\n",NULL,flag_Warning);
}  
//重新初始化
void USBH_USR_DeInit(void)
{
	switch(sys_USBH_use_for_)
	{
		case use_for_HID:
			FaultASSERT("重新初始化!!!\r\n",NULL,flag_Warning);
			break;
		case use_for_MSC:
			AppState=USH_USR_FS_INIT;
			break;
		case use_for_VCP:
			break;
		default:break;
	}

}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
//	printf("无法恢复的错误!!!\r\n\r\n");
	FaultASSERT("无法恢复的错误!!!\r\n\r\n",NULL,flag_Warning);	
}
//////////////////////////////////////////////////////////////////////////////////////////
//下面两个函数,以防止USB死机

//USB枚举状态死机检测,防止USB枚举失败导致的死机
//phost:USB_HOST结构体指针
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt=0;
	//这个状态,如果持续存在,则说明USB死机了.
	if(phost->gState==HOST_CTRL_XFER&&(phost->EnumState==ENUM_IDLE||phost->EnumState==ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if(errcnt>2000)//死机了
		{ 
			errcnt=0;
			RCC->AHB2RSTR|=1<<7;	//USB OTG FS 复位
			HAL_Delay(5);
			RCC->AHB2RSTR&=~(1<<7);	//复位结束  
			return 1;
		} 
	}else errcnt=0;
	return 0;
} 
//USB HID通信死机检测,防止USB通信死机(暂时仅针对:DTERR,即Data toggle error)
//pcore:USB_OTG_Core_dev_HANDLE结构体指针
//phidm:HID_Machine_TypeDef结构体指针 
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm)
{
 	if(pcore->host.HC_Status[phidm->hc_num_in]==HC_DATATGLERR)//检测到DTERR错误,直接重启USB.
	{  
		return 1;
	}
	return 0;
}

//HID重新连接
void USBH_HID_Reconnect(void)
{
	//关闭之前的连接
	USBH_DeInit(&USB_OTG_Core,&USB_Host);	//复位USB HOST
	USB_OTG_StopHost(&USB_OTG_Core);		//停止USBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//存在,才禁止
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//关闭USB连接
		USBH_DeInit(&USB_OTG_Core, &USB_Host);
		USB_Host.usr_cb->DeInit();
		USB_Host.class_cb->DeInit(&USB_OTG_Core,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core);//关闭所有中断
	//重新复位USB
    __HAL_RCC_USB_OTG_FS_FORCE_RESET();//USB OTG FS 复位
	HAL_Delay(5);
    __HAL_RCC_USB_OTG_FS_RELEASE_RESET();//复位结束
	
	memset(&USB_OTG_Core,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	//重新连接USB HID设备
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
}

//////////////////////////////////////////////////////////////////////////////////////////
//USB键盘鼠标数据处理

u8 USB_FIRST_PLUGIN_FLAG = 0;	//USB第一次插入标志,如果为1,说明是第一次插入，不用动

//鼠标初始化
void USR_MOUSE_Init	(void)
{
	//USB 鼠标，做鼠标HID设备的标记，执行鼠标处理用户程序
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}
//键盘初始化
void  USR_KEYBRD_Init(void)
{ 
	//USB 键盘，做键盘HID设备的标记，执行键盘处理用户程序
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}

//用于存放键盘输入内容
const u16 maxinputchar = 100;
__align(4) u8 Keybd_buf[maxinputchar]; 

//USB鼠标数据处理
//data:USB鼠标数据结构体指针
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{  
	static signed short x,y,z; 
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
		x=y=z=0;
	}
	x += (signed char)data->x;			//HID_MOUSE_Data_TypeDef *data为增量式
		if(x>9999)x=9999;
		if(x<-9999)x=-9999;
	y += (signed char)data->y;
		if(y>9999)y=9999;
		if(y<-9999)y=-9999;
	z += (signed char)data->z;			//z为滚轮
		if(z>9999)z=9999;
		if(z<-9999)z=-9999;
	
	//0000 0001————LEFT
	if(data->button&0X01)
	{
		
	}
	
	//0000 0010————RIGHT
	if((data->button&0X03)==0X02)
	{
		
	}
	//0000 0011————+RIGHT
	else if((data->button&0X03)==0X03)
	{
		
	}
	
	//0000 0100————MID
	if((data->button&0X07)==0X04)
	{
		
	}
	//0000 01xx————+MID（xx > 0）
	else if((data->button&0X07)>0X04)
	{
		
	}
	
//	printf_uart(UART1,"btn,X,Y,Z:0x%x,%d,%d,%d\r\n",data->button,(signed char)data->x,(signed char)data->y,(signed char)data->z); 
} 


//USB键盘数据处理
//data:USB鼠标数据结构体指针
void  USR_KEYBRD_ProcessData (uint8_t data)
{ 
	static u16 pos;
	
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
 		pos=0; 
	}
	
	if(data>=' '&&data<='~')
	{
		Keybd_buf[pos++]=data;
		Keybd_buf[pos]=0;		//添加结束符. 
		if(pos>maxinputchar)pos=maxinputchar;//最大输入这么多
	}else if(data==0X0D)	//退格键
	{
		if(pos)pos--;
		Keybd_buf[pos]=0;		//添加结束符.  
	}
	
	
	if(pos<=maxinputchar)	//没有超过显示区
	{
		printf_uart(UART1,"%s\r\n",Keybd_buf);
	}
	
//	printf_uart(UART1,"KEY Board Value:%02X\r\n",data);
//	printf_uart(UART1,"KEY Board Char:%c\r\n",data); 
}


//////////////////////////////////////////////////////////////////////////////////////////
//USB HOST MSC处理函数，实现fatfs diskio的接口函数

//定义U盘插上后，执行一次的用户程序
u8 USBH_MSC_User_App(void)
{
	#if SYSTEM_FATFS_ENABLE
	
		u8 init_return = 1;
		u32 total,free;	//获取U盘容量和剩余容量，字节为单位
		
		init_return = f_mount(fs[2],"DEV_USB:",1);		//挂载USB UOST MSC文件设备（U盘）
			if(init_return != 0){ FaultASSERT("f_mount fs[2]",init_return,flag_Warning);return init_return;}
		init_return = fatfs_getfree((u8*)"DEV_USB:",&total,&free);
		if(init_return == 0)
		{
			printf_uart(UART1,"U Disk Total Size: %d MB\r\n",total >> 10);	//获取U盘容量和剩余容量 并显示出
			printf_uart(UART1,"U Disk  Free Size: %d MB\r\n",free >> 10);
		}else{return init_return;}
	
	#endif
	
	return 0;
} 

//USB HOST MSC类用户应用程序
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://初始化文件系统 
			FaultASSERT("开始执行用户程序!!!\r\n",NULL,flag_Warning);
			AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//执行USB OTG 测试主程序
			res=USBH_MSC_User_App(); //用户主程序
     		res=0;
			if(res)AppState=USH_USR_FS_INIT;
      		break;
    	default:break;
  	} 
	return res;
}

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//返回U盘状态
}

//读U盘
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

//写U盘
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}



