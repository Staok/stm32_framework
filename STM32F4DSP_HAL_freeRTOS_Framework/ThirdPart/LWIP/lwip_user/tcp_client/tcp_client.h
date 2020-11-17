#ifndef __TCP_CLIENT_DEMO_H
#define __TCP_CLIENT_DEMO_H
#include "lwip_comm.h"
#include "PeriphConfigCore.h"
#include "PeriphConfig.h"


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "lwip/mem.h"

#if LWIP_TCP && (NO_SYS == 1)

#define TCP_CLIENT_RX_BUFSIZE	200		//定义tcp client最大接收数据长度
#define TCP_CLIENT_TX_BUFSIZE	200		//定义tcp client最大接收数据长度
#define	TCP_CLIENT_PORT			8087	//定义tcp client要连接的远端端口

//TCP Client接收数据缓冲区
extern char* tcp_client_recvbuf;	
//TCP Client发送数据缓冲区
extern char* tcp_client_sendbuf;			//需要以 '\0'为结尾！

//TCP Client 测试全局状态标记变量
//bit7:0,没有数据要发送;1,有数据要发送
//bit6:0,没有收到数据;1,收到数据了
//bit5:0,没有连接上服务器;1,连接上服务器了
//bit4:0,没有初始化或者经过了反初始化;1,初始化了
//bit3~0:保留
extern u8 tcp_client_flag;


//设置发送标志，系统将自动发送 tcp_client_sendbuf 的内容
#define TCP_Client_SET_SEND_Flag	(tcp_client_flag|=0x80)	

//查询是否有数据被接收到，为真则可以从 tcp_client_recvbuf 取内容
#define TCP_Client_IS_RECV_Flag		(tcp_client_flag&0x40)
//要保持查询后随手清标志位的好习惯呦~
#define TCP_Client_Clear_RECV_Flag	(tcp_client_flag&=(~0x40))

//查询当前连接是否有效，若连接断开(比如主机下线)后这里系统将其变0，应该一直查询
#define TCP_Client_IS_CONN_Flag		(tcp_client_flag&0x20)	



extern struct tcp_pcb* TCP_Client_PCB_test;

//tcp服务器连接状态
enum tcp_client_states
{
	ES_TCPCLIENT_NONE = 0,		//没有连接
	ES_TCPCLIENT_CONNECTED,		//连接到服务器了 
	ES_TCPCLIENT_CLOSING,		//关闭连接
}; 
//LWIP回调函数使用的结构体
struct tcp_client_struct
{
	u8 state;               //当前连接状
	struct tcp_pcb *pcb;    //指向当前的pcb
	struct pbuf *p;         //指向接收/或传输的pbuf
};  

u8 tcp_client_init_test(struct tcp_pcb *tcppcb, u8 remoteip_3, u16 tcp_client_port);
//err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
//err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err);
//void tcp_client_error(void *arg,err_t err);
//err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
//err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
//void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es);
void tcp_client_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es );


#endif


#endif


