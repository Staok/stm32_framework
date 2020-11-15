#ifndef __TCP_SERVER_DEMO_H
#define __TCP_SERVER_DEMO_H

#include "lwip_comm.h"
#include "PeriphConfigCore.h"
#include "PeriphConfig.h"

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/memp.h"
#include "lwip/mem.h"

#if LWIP_TCP && (NO_SYS == 1)


#define TCP_SERVER_RX_BUFSIZE	200		//定义tcp server最大接收数据长度
#define TCP_SERVER_TX_BUFSIZE	200		//定义tcp server最大发送数据长度
#define TCP_SERVER_PORT_test	8088	//定义tcp server的端口

//TCP Server接收数据缓冲区
extern char* tcp_server_recvbuf;	
//TCPP Server发送数据缓冲区
extern char* tcp_server_sendbuf;		//需要以 '\0'为结尾！

//TCP Server 测试全局状态标记变量
//bit7:0,没有数据要发送;1,有数据要发送
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有客户端连接上;1,有客户端连接上了.
//bit4~0:保留
extern u8 tcp_server_flag;

//设置发送标志，系统将自动发送 tcp_server_sendbuf 的内容
#define TCP_Server_SET_SEND_Flag	tcp_server_flag|=0x80	

//查询是否有数据被接收到，为真则可以从 tcp_server_recvbuf 取内容
#define TCP_Server_IS_RECV_Flag		tcp_server_flag&0x40
//要保持查询后随手清标志位的好习惯呦~
#define TCP_Server_Clear_RECV_Flag	tcp_server_flag&=(~0x40)

//查询当前连接是否有效，若从机失连后这里系统将其变0，应该一直查询
#define TCP_Server_IS_CONN_Flag		tcp_server_flag&0x20	


extern struct tcp_pcb *TCP_Server_PCB_test;  		//定义一个TCP服务器控制块
extern struct tcp_pcb *TCP_Server_PCBCONN_test;  	//定义一个TCP服务器控制块


//tcp服务器连接状态
enum tcp_server_states
{
	ES_TCPSERVER_NONE = 0,		//没有连接
	ES_TCPSERVER_ACCEPTED,		//有客户端连接上了 
	ES_TCPSERVER_CLOSING,		//即将关闭连接
}; 
//LWIP回调函数使用的结构体
struct tcp_server_struct
{
	u8 state;               //当前连接状
	struct tcp_pcb *pcb;    //指向当前的pcb
	struct pbuf *p;         //指向接收/或传输的pbuf
}; 

//TCP Server 初始化，输入参数：tcp_pcb，选择 remoteip[3] ， 端口号
//初始化例子： tcp_server_init_test(TCP_Server_PCB_test,TCP_Server_PCBCONN_test,TCP_SERVER_PORT_test);
//创建成功后，本地IP即TCP Server的IP，lwip_inf.remoteip 即写入远程连接上的IP
//返回0成功，其他失败
u8 tcp_server_init_test(struct tcp_pcb* tcppcbnew,struct tcp_pcb* tcppcbconn,u16_t tcp_server_port);
//关闭tcp server连接，用户调用
void TCP_Server_connection_close(struct tcp_pcb* tspcb,struct tcp_pcb* tspcbconn);
//err_t tcp_server_accept(void *arg,struct tcp_pcb *newpcb,err_t err);
//err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
//void tcp_server_error(void *arg,err_t err);
//err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
//err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
//void tcp_server_senddata(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
//void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
//void tcp_server_remove_timewait(void);

#endif



#endif 
