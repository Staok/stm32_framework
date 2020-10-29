#include "NetCard.h"
#include "lwip_comm.h"

struct NetCard_config NetCardcfg;				//网卡配置结构体

/*
这里对网卡初始化
	（初始化IO，设置MAC地址和组播地址，设置寄存器，设置工作模式，读出ID）

再根据 struct pbuf 打包的数据 完成数据收发函数
*/

u8 NetCard_Init(void)
{
	u32 temp;
	
	/*_________________________________IO口相关外设初始化_____________________________________*/

	
	/*_________________________________参数配置_____________________________________*/
	
	temp=*(vu32*)(0x1FFFF7E8);				//获取STM32的唯一ID的前24位作为MAC地址后三字节
	NetCardcfg.mode=NetCard_AUTO;	
 	NetCardcfg.queue_packet_len=0;
	
	//初始化MAC地址
	NetCardcfg.mac_addr[0]=2;
	NetCardcfg.mac_addr[1]=0;
	NetCardcfg.mac_addr[2]=0;
	NetCardcfg.mac_addr[3]=(temp>>16)&0XFF;	//低三字节用STM32的唯一ID
	NetCardcfg.mac_addr[4]=(temp>>8)&0XFFF;
	NetCardcfg.mac_addr[5]=temp&0XFF;
	
	//初始化组播地址
	NetCardcfg.multicase_addr[0]=0Xff;
	NetCardcfg.multicase_addr[1]=0Xff;
	NetCardcfg.multicase_addr[2]=0Xff;
	NetCardcfg.multicase_addr[3]=0Xff;
	NetCardcfg.multicase_addr[4]=0Xff;
	NetCardcfg.multicase_addr[5]=0Xff;
	NetCardcfg.multicase_addr[6]=0Xff;
	NetCardcfg.multicase_addr[7]=0Xff; 
	
	NetCard_Reset();							//复位NetCard
	delay_ms(100);
	
	
	/*_________________________________网卡寄存器初始化_____________________________________*/
	
	temp=NetCard_Get_DeiviceID();			//获取NetCardID
	if(temp!=NetCard_ID) return 1; 			//读取ID错误
	
	NetCard_Set_PHYMode(NetCardcfg.mode);		//设置PHY工作模式
	
	//一堆...
	//NetCard_WriteReg(,); 	
	
	
	//设置MAC地址和组播地址
	NetCard_Set_MACAddress(NetCardcfg.mac_addr);		//设置MAC地址
	NetCard_Set_Multicast(NetCardcfg.multicase_addr);	//设置组播地址
	//temp=NetCard_Get_SpeedAndDuplex();		//获取NetCard的连接速度和双工状态
	//NetCard_WriteReg(NetCard_IMR,NetCardcfg.imr_all);	//设置中断
	return 0;		
}

//读取NetCard指定寄存器的值
//reg:寄存器地址
//返回值：NetCard指定寄存器的值
u16 NetCard_ReadReg(u16 reg)
{
	u16 value;
	
	return value;
}

//向NetCard指定寄存器中写入指定值
//reg:要写入的寄存器
//data:要写入的值
void NetCard_WriteReg(u16 reg,u16 data)
{
	
}

//读取NetCard的PHY的指定寄存器
//reg:要读的PHY寄存器
//返回值:读取到的PHY寄存器值
u16 NetCard_PHY_ReadReg(u16 reg)
{
	u16 value;
	
	return value;
}

//向NetCard的PHY寄存器写入指定值
//reg:PHY寄存器
//data:要写入的值
void NetCard_PHY_WriteReg(u16 reg,u16 data)
{
	
}

//获取NetCard的芯片ID
//返回值：NetCard的芯片ID值
u32 NetCard_Get_DeiviceID(void)
{
	u32 value;
	
	return value;
}

//获取NetCard的连接速度和双工模式
//返回值：	0,100M半双工
//			1,100M全双工
//			2,10M半双工
//			3,10M全双工
//			0XFF,连接失败！
u8 NetCard_Get_SpeedAndDuplex(void)
{
	u8 temp;
	
	return temp;
}

//设置DM900的PHY工作模式
//mode:PHY模式
void NetCard_Set_PHYMode(u8 mode)
{
//	u16 BMCR_Value,ANAR_Value;	
//	switch(mode)
//	{
//		case NetCard_10MHD:		//10M半双工
//			BMCR_Value=0X0000;
//			ANAR_Value=0X21;
//			break;
//		case NetCard_10MFD:		//10M全双工
//			BMCR_Value=0X0100;
//			ANAR_Value=0X41;
//			break;
//		case NetCard_100MHD:		//100M半双工
//			BMCR_Value=0X2000;
//			ANAR_Value=0X81;
//			break;
//		case NetCard_100MFD:		//100M全双工
//			BMCR_Value=0X2100;
//			ANAR_Value=0X101;
//			break;
//		case NetCard_AUTO:		//自动协商模式
//			BMCR_Value=0X1000;
//			ANAR_Value=0X01E1;
//			break;		
//	}
//	NetCard_PHY_WriteReg(NetCard_PHY_BMCR,BMCR_Value);
//	NetCard_PHY_WriteReg(NetCard_PHY_ANAR,ANAR_Value);
// 	NetCard_WriteReg(NetCard_GPR,0X00);	//使能PHY
}

//设置NetCard的MAC地址
//macaddr:指向MAC地址
void NetCard_Set_MACAddress(u8 *macaddr)
{
	u8 i;
	for(i=0;i<6;i++)
	{
		//NetCard_WriteReg(NetCard_PAR+i,macaddr[i]);
	}
}
//设置NetCard的组播地址
//multicastaddr:指向多播地址
void NetCard_Set_Multicast(u8 *multicastaddr)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		//NetCard_WriteReg(NetCard_MAR+i,multicastaddr[i]);
	}
}
//复位NetCard
void NetCard_Reset(void)
{
	//复位NetCard,复位步骤参考 NetCard Application 手册
	
} 

//通过NetCard发送数据包
//p:pbuf结构体指针
void NetCard_SendPacket(struct pbuf *p)
{
//	struct pbuf *q;
	
//	q->next;
//	q->len;
//	q->payload;					
}

//NetCard接收数据包
//接收到的数据包存放在NetCard的RX FIFO中，地址为0X0C00~0X3FFF
//接收到的数据包的前四个字节并不是真实的数据，而是有特定含义的
//byte1:表明是否接收到数据，为0x00或者0X01，如果两个都不是的话一定要软件复位NetCard
//		0x01，接收到数据
//		0x00，未接收到数据
//byte2:第二个字节表示一些状态信息，和NetCard的RSR(0X06)寄存器一致的
//byte3:本帧数据长度的低字节
//byte4:本帧数据长度的高字节
//返回值：pbuf格式的接收到的数据包
struct pbuf *NetCard_Receive_Packet(void)
{
	struct pbuf* p;
	//p=pbuf_alloc(PBUF_RAW,rx_length,PBUF_POOL);	//pbufs内存池分配pbuf

	return (struct pbuf*)p; 
}


//中断处理函数
//在外部中断回调函数中调用此函数
void NetCard_ISRHandler(void)
{
//	u16 int_status;
//	u16 last_io; 
//	last_io = NetCard->REG;
//	int_status=NetCard_ReadReg(NetCard_ISR); 
//	NetCard_WriteReg(NetCard_ISR,int_status);				//清除中断标志位，NetCard的ISR寄存器的bit0~bit5写1清零
//	if(int_status & ISR_ROS)printf("overflow \r\n");
//    if(int_status & ISR_ROOS)printf("overflow counter overflow \r\n");	
//	if(int_status & ISR_PRS)		//接收中断
//	{  
// 		//接收完成中断，用户自行添加所需代码
//	} 
//	if(int_status&ISR_PTS)			//发送中断
//	{ 
//		//发送完成中断，用户自行添加所需代码
//	}
//	NetCard->REG=last_io;	
}

