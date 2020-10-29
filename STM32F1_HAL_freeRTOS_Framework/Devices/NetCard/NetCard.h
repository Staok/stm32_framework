#ifndef _NETCARD_H
#define _NETCARD_H
#include "lwip/sys.h"
#include "lwip/pbuf.h"

#include "sys_config.h"


#define NetCard_ID 0x3971038

//网卡寄存器

//网卡内部PHY寄存器

//网卡工作模式定义
enum NetCard_PHY_mode
{
	NetCard_10MHD 	= 	0, 					//10M半双工
	NetCard_100MHD 	= 	1,					//100M半双工	
	NetCard_10MFD 	= 	4, 					//10M全双工
	NetCard_100MFD 	= 	5,					//100M全双工
	NetCard_AUTO  	= 	8, 					//自动协商
};

//网卡配置结构体，只用于初始化配置
struct NetCard_config
{
	enum NetCard_PHY_mode mode;				//工作模式
	u8  imr_all;							//中断类型 
	u16 queue_packet_len;					//每个数据包大小
	u8  mac_addr[6];						//MAC地址
	u8  multicase_addr[8];					//组播地址
};
extern struct NetCard_config NetCardcfg;	//网卡配置结构体

u8   NetCard_Init(void);
u16  NetCard_ReadReg(u16 reg);
void NetCard_WriteReg(u16 reg,u16 data);
u16  NetCard_PHY_ReadReg(u16 reg);
void NetCard_PHY_WriteReg(u16 reg,u16 data);
u32  NetCard_Get_DeiviceID(void);
u8   NetCard_Get_SpeedAndDuplex(void);	
void NetCard_Set_PHYMode(u8 mode);
void NetCard_Set_MACAddress(u8 *macaddr);
void NetCard_Set_Multicast(u8 *multicastaddr);
void NetCard_Reset(void);


void NetCard_SendPacket(struct pbuf *p);
struct pbuf *NetCard_Receive_Packet(void);
void NetCard_ISRHandler(void);
#endif



