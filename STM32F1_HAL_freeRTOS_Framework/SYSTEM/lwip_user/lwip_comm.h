#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "NetCard.h" 

#define LWIP_MAX_DHCP_TRIES		4   //DHCP服务器最大重试次数

//lwip信息结构体
typedef struct  
{
	u8 mac[6];      //MAC地址
	u8 remoteip[4];	//远端主机IP地址 
	u8 ip[4];       //本机IP地址
	u8 netmask[4]; 	//子网掩码
	u8 gateway[4]; 	//默认网关的IP地址
	
	vu8 dhcpstatus;	//dhcp状态 
					//0,未获取DHCP地址;
					//1,进入DHCP获取状态
					//2,成功获取DHCP地址
					//0XFF,获取失败.
}__lwip_dev;
extern __lwip_dev lwip_inf;	//lwip信息结构体

/*裸机跑LWIP需要循环执行的两个函数*/
void lwip_pkt_handle(void);
void lwip_periodic_handle(void);

u8 lwip_comm_init(void);

#endif













