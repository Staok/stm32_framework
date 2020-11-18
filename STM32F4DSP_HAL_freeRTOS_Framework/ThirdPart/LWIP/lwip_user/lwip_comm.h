#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "NetCard.h" 
#include "netif/etharp.h"

/*引用应用层的头文件*/
#include "udp.h"
#include "tcp_server.h"
#include "tcp_client.h" 

#include "httpd.h"

//extern ETH_HandleTypeDef ETH_Handler;             //以太网句柄
//extern ETH_DMADescTypeDef *DMARxDscrTab;			//以太网DMA接收描述符数据结构体指针
//extern ETH_DMADescTypeDef *DMATxDscrTab;			//以太网DMA发送描述符数据结构体指针 
//extern uint8_t *Rx_Buff; 							//以太网底层驱动接收buffers指针 
//extern uint8_t *Tx_Buff; 							//以太网底层驱动发送buffers指针
//extern ETH_DMADescTypeDef  *DMATxDescToSet;		//DMA发送描述符追踪指针
//extern ETH_DMADescTypeDef  *DMARxDescToGet; 		//DMA接收描述符追踪指针 

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

extern struct netif lwip_handle;	//定义一个全局的网络接口句柄

/*裸机跑LWIP需要循环执行的两个函数*/
//void lwip_pkt_handle(void);		//在ETH接收中断里，不需要外部调用
void lwip_periodic_handle(void);	//严格10ms周期调用
u8 lwip_comm_init(struct netif *netif);
void lwip_comm_destroy(struct netif *netif);


#endif













