#ifndef __UDP_DEMO_H
#define __UDP_DEMO_H

#include "lwip_comm.h"
#include "PeriphConfigCore.h"
#include "PeriphConfig.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"



#define UDP_RX_BUFSIZE			200		//定义udp最大接收数据长度 
#define UDP_TX_BUFSIZE			200		//定义udp最大发送数据长度 
#define UDP_PORT_test			8089	//定义udp连接的端口 



#if (LWIP_UDP == 1)
	extern struct udp_pcb *UDP_PCB_test;
	extern u8 udp_demo_flag;
	u8 UDP_init_test(struct udp_pcb *pcb, u8 remoteip_3, u16 udp_port);
#endif
 
#if ((NO_SYS == 1) && (LWIP_UDP == 1))
	void UDP_recv_test(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
	void UDP_senddata_test(struct udp_pcb *upcb);
#endif


void UDP_connection_close(struct udp_pcb *upcb);

#endif

