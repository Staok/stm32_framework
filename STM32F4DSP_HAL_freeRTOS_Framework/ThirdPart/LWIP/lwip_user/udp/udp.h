#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H

#include "lwip_comm.h"
#include "PeriphConfigCore.h"
#include "PeriphConfig.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#if ((NO_SYS == 1) && (LWIP_UDP == 1))

#define UDP_RX_BUFSIZE			200		//定义udp最大接收数据长度，使用malloc为缓冲区申请空间，用户可修改 
#define UDP_TX_BUFSIZE			200		//定义udp最大发送数据长度，使用malloc为缓冲区申请空间，用户可修改 
#define UDP_PORT_test			8089	//定义udp连接的端口，用户用

//UDP接收数据缓冲区
extern char* udp_demo_recvbuf;			//用户用
//UDP发送数据缓冲区
extern char* tcp_demo_sendbuf;			//需要以 '\0'为结尾！，用户用

	extern struct udp_pcb *UDP_PCB_test;
	extern u8 udp_demo_recv_flag;	//成功接收到一次数据的标志位，用于判断，若为1则可从 udp_demo_recvbuf 取数据，用户用
	u8 UDP_init_test(struct udp_pcb *pcb, u8 remoteip_3, u16 udp_port);	//UDP初始化，输入参数：udp_pcb，选择 remoteip[3] ， 端口号，用户用

//	void UDP_recv_test(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
	void UDP_senddata_test(struct udp_pcb *upcb); 		//UDP服务器发送数据，发送 tcp_demo_sendbuf 字符数组所存的数据，用户用
	void UDP_connection_close(struct udp_pcb *upcb); 	//关闭tcp连接，用户用


#endif


#endif

