#include "usb_bsp.h"

/*
	硬件级相关配置都在这里
	下面的API会被USB Core调用
	
	基本API需要定义：
	void USB_OTG_BSP_Init (USB_OTG_CORE_HANDLE *pdev);						//USB硬件初始化
	void USB_OTG_BSP_uDelay (const uint32_t usec);
	void USB_OTG_BSP_mDelay (const uint32_t msec);
	void USB_OTG_BSP_EnableInterrupt (USB_OTG_CORE_HANDLE *pdev);

USB HOST额外需要定义的API：
	void USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev);
	void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev,uint8_t state);	//控制USB口供电
	void USB_OTG_BSP_Resume(USB_OTG_CORE_HANDLE *pdev) ;            		//USB口供电控制IO的初始化
	void USB_OTG_BSP_Suspend(USB_OTG_CORE_HANDLE *pdev);
*/


/*
以下对于 USB FS 都是固定的，不用动
	除非要增加USB HS 的支持
*/

//USB主机电源控制口，用于控制HOST USB口的5V启停，电路上应用高端P MOS加一个N MOS的结构控制
//此接口可以任意选择并修改 USB_OTG_BSP_ConfigVBUS()
#define USB_HOST_PWRCTRL 	PAout(15)

//USB OTG 底层IO初始化
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
     GPIO_InitTypeDef  GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                   //使能GPIOA时钟
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();              //使能OTG FS时钟
	
	
	//配置PA11 12
    GPIO_InitStruct.Pin=GPIO_PIN_11|GPIO_PIN_12;    //PA11 12
    GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;           //复用
    GPIO_InitStruct.Pull=GPIO_NOPULL;               //无上下拉
    GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;          //高速
    GPIO_InitStruct.Alternate=GPIO_AF10_OTG_FS;     //复用为OTG FS
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);         //初始化
   
}

//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{  	
    HAL_NVIC_SetPriority(OTG_FS_IRQn,1,0);          //抢占优先级1，子优先级0
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);                //使能OTG USB FS中断 
}

//关闭 OTG_FS_IRQn 中断
void USB_OTG_BSP_DisableInterrupt(void)
{
	HAL_NVIC_DisableIRQ(OTG_FS_IRQn);                //失能OTG USB FS中断 
}
//USB HOST OTG 端口供电设置
//pdev:USB OTG内核结构体指针
//state:0,断电;1,上电
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
  if (0 == state)
  {
    /* DISABLE is needed on output of the Power Switch */
    USB_HOST_PWRCTRL = 0;								//关闭USB HOST电源供电
  }
  else
  {
    /* ENABLE the Power Switch by driving the Enable LOW */
    USB_HOST_PWRCTRL = 1;								//开启USB HOST电源供电
  }
}
//USBHOST OTG 端口供电IO配置
//pdev:USB OTG内核结构体指针
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{
     GPIO_InitTypeDef  GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                   //使能GPIOA时钟
	
	//PA15
    GPIO_InitStruct.Pin=GPIO_PIN_15; 				//PA15
    GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_InitStruct.Pull=GPIO_NOPULL;          		//无上下拉
    GPIO_InitStruct.Speed=GPIO_SPEED_HIGH;     		//高速
    HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USB_HOST_PWRCTRL = 0;							//关闭USB HOST电源供电
} 
//USB_OTG us级延时函数
//usec:要延时的us数
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
	delay_us(usec);
}
//USB_OTG ms级延时函数
//官方例程采用的是定时器2来实现的.
//msec:要延时的ms数
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	HAL_Delay(msec);
}


