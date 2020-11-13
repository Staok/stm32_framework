#include "system.h"
#include "led.h"
#include "time.h"
#include "malloc.h" 

//#include "spi.h"
#include "sdio_sdcard.h"
#include "SPI_SDcard.h"

#include "can.h"

#include "ff.h"
/*__________________________________________________________________________

	GPIO的AF复用功能完整表在 “stm32f207ie.pdf” 的 59页 开始

	DMAx的数据流和通道分配完整表在 “寄存器手册-cd00225773-stm32f205xx-stm32f207xx-stm32f215xx-and-stm32f217xx-advanced-armbased-32bit-mcus-stmicroelectronics”
				的 180页 开始
	
	
	STM32F207IET6：
		512KB(hd)FLASH，128KB（SRAM1）+112KB+16KB（SRAM2）RAM，176脚LQFP封装，140个GPIO，-40~85℃
   	
	CPU内部 FLASH  			【0x0800 0000 - 0x080F FFFF】,容量512K(0x80000)字节
	CPU内部 SRAM1  			【0x2000 0000 - 0x2000 FFFF】,容量128K(0x20000)字节
	
	时钟分配：
		HSE外接8Mhz晶振，LSE外接32.768Khz晶振
		开启HSE（8M）、LSE（32.768K）和LSI（32K），关闭HSI（16M）
		PLLCLK = SYSCLK = AHBCLK = HCLK 均为120MHz，APB2外设为60MHz，APB1外设为30Mhz，APB2定时器为120MHz，APB1定时器为60Mhz
		
		ETH\RNG\FSMC\USB FS\USB HS\GPIO\DMA 均在120M
		RTC 时钟取自 LSE = 32.768K
		IWDG时钟取自 LSI = 32K
		DCMI 48 Mbyte/s max
		
		APB2外设(60M)：SDIO\USART1\USART6\SPI1\ADC123
		APB2定时器(120M)：TIM1 TIM8 TIM9 TIM10 TIM11
		APB1外设(30M)：USART2\USART3\UART4\UART5\SPI2、I2S2\SPI3、I2S3\I2C123\CAN12\DAC12
		APB1定时器(60M)：TIM2 TIM3 TIM4 TIM5 TIM12 TIM13 TIM14
*/

/*
本程序是STM32F207IE标准库的超级简单的模板工程，东西不多日常测试用，用传统的标准库和中断分组2...

本程序的时钟初始化在 system_stm32f2xx.c文件里，根据宏定义上电运行 SystemInit() 进行配置
*/
int main()
{
	SysTick_Init(120);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
	my_mem_init(SRAMIN);		//初始化内部内存池
	USART1_Init(115200);
	LED_Init();
	
	TIM4_Init(10000-1,6000-1);  //定时1s
	
	
	/*项目外设初始化*/
	FATFS fs[_VOLUMES];		//创建文件系统设备句柄
	FIL ftemp;	  			//文件
	u8 fatbuf[512];			//SD卡数据缓存区
	UINT readCount;
	UINT writeCount;
	
	char strbuf[200];		//字符串缓存
	
	delay_ms(500);
	u8 res;
	res = f_mount(&fs[0],"0:",1); //挂载SDIO驱动的SD卡
	if(res != 0)
	{
		printf("挂载SDIO SD失败\r\n");
		while(1);
	}else					//挂载成功，开始测试
	{
		printf("挂载SDIO SD成功，开始测试\r\n");
		
		f_open(&ftemp,"0:TEXT/demo.txt",FA_READ);
		f_read(&ftemp,fatbuf,512,&readCount);
		snprintf(strbuf,(size_t)readCount,"demo.txt读出内容：\r\n%s\r\n",fatbuf);
		printf("%s",strbuf);
		f_close(&ftemp);
		
		f_open(&ftemp,"0:TEXT/demo01.txt",FA_OPEN_ALWAYS | FA_WRITE);
		f_lseek(&ftemp, f_size(&ftemp));
		f_write(&ftemp,fatbuf,readCount,&writeCount);
		printf("demo01.txt已经写入数据个数 %d\r\n",writeCount);
		f_close(&ftemp);
		
		
		delay_ms(100);
		f_mount(0, "0:", 1);
		printf("测试完毕，卸载SDIO SD\r\n");
	}
	
	res = f_mount(&fs[1],"1:",1); //挂载SPI驱动的SD卡
	if(res != 0)
	{
		printf("挂载SPI SD失败\r\n");
		while(1);
	}else				//挂载成功，开始测试
	{
		printf("挂载SPI SD成功，开始测试\r\n");
		
		f_open(&ftemp,"1:TEXT/demo.txt",FA_READ);
		f_read(&ftemp,fatbuf,512,&readCount);
		snprintf(strbuf,(size_t)readCount,"demo.txt读出内容：\r\n%s\r\n",fatbuf);
		printf("%s",strbuf);
		f_close(&ftemp);
		
		f_open(&ftemp,"1:TEXT/demo02.txt",FA_OPEN_ALWAYS | FA_WRITE);
		f_lseek(&ftemp, f_size(&ftemp));
		f_write(&ftemp,fatbuf,readCount,&writeCount);
		printf("demo02.txt已经写入数据个数 %d\r\n",writeCount);
		f_close(&ftemp);
		
		
		delay_ms(100);
		f_mount(0, "1:", 1);
		printf("测试完毕，卸载SPI SD\r\n");
	}
	
	
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_7tq,CAN_BS1_6tq,6,CAN_Mode_Normal);//500Kbps波特率
		
	printf("Go\r\n");
	for(;;)
	{
		if((USART1_RX_STA & 0x8000) == 1)	//串口接收到数据
		{
			switch(USART1_RX_BUF[0])
			{
				case 1:
					break;
				
				case 2:
					break;
				default:
					break;
			}
		}
		
	}
}


