#include "udp.h"


//UDP接收数据缓冲区
u8 udp_demo_recvbuf[UDP_RX_BUFSIZE];	//UDP接收数据缓冲区
//UDP发送数据缓冲区
const u8 tcp_demo_sendbuf[] = "stm32_framework - STM32F407 UDP demo send data\r\n";

struct udp_pcb *UDP_PCB_test;  	//定义一个UDP服务器控制块


#if (LWIP_UDP == 1)			//如果使用UDP时

u8 udp_demo_flag = 0;		//成功接收到一次数据的标志位，用于判断


//UDP初始化，输入参数：udp_pcb，选择 remoteip[3] ， 端口号
u8 UDP_init_test(struct udp_pcb *pcb, u8 remoteip_3, u16 udp_port)
{
 	err_t err;
	struct ip4_addr rmtipaddr;  	//远端ip地址
 	
	//设置远端IP地址
	//前三个IP保持和DHCP得到的IP一致
	lwip_inf.remoteip[0]=lwip_inf.ip[0];
	lwip_inf.remoteip[1]=lwip_inf.ip[1];
	lwip_inf.remoteip[2]=lwip_inf.ip[2];
	lwip_inf.remoteip[3] = remoteip_3;
	
	pcb=udp_new();
	if(pcb)//创建成功
	{ 
		IP4_ADDR(&rmtipaddr,lwip_inf.remoteip[0],lwip_inf.remoteip[1],lwip_inf.remoteip[2],lwip_inf.remoteip[3]);
		err=udp_connect(pcb,&rmtipaddr,udp_port);	//UDP客户端连接到指定IP地址和端口号的服务器
		if(err==ERR_OK)
		{
			err=udp_bind(pcb,IP_ADDR_ANY,udp_port);	//绑定本地IP地址与端口号
			if(err==ERR_OK)	//绑定完成
			{
				udp_recv(pcb,UDP_recv_test,NULL);	//注册接收回调函数
				return 0;
			}else return 3;
		}else return 2;		
	}else return 1;
}

#endif

#if ((NO_SYS == 1) && (LWIP_UDP == 1))		//如果不使用操作系统并且使用UDP时
//UDP服务器回调函数，自动把接收的字符串写到 tcp_demo_sendbuf 字符串数组里
void UDP_recv_test(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	u32 data_len = 0;
	struct pbuf *q;
	if(p!=NULL)	//接收到不为空的数据时
	{
		memset(udp_demo_recvbuf,0,UDP_RX_BUFSIZE);  //数据接收缓冲区清零
		for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			//判断要拷贝到UDP_DEMO_RX_BUFSIZE中的数据是否大于UDP_DEMO_RX_BUFSIZE的剩余空间，如果大于
			//的话就只拷贝UDP_DEMO_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			if(q->len > (UDP_RX_BUFSIZE-data_len)) memcpy(udp_demo_recvbuf+data_len,q->payload,(UDP_RX_BUFSIZE-data_len));//拷贝数据
			else memcpy(udp_demo_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > UDP_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
		}
		pcb->remote_ip=*addr; 				//记录远程主机的IP地址
		pcb->remote_port=port;  			//记录远程主机的端口号
		lwip_inf.remoteip[0]= pcb->remote_ip.addr&0xff; 	//IADDR4
		lwip_inf.remoteip[1]=(pcb->remote_ip.addr>>8)&0xff; //IADDR3
		lwip_inf.remoteip[2]=(pcb->remote_ip.addr>>16)&0xff;//IADDR2
		lwip_inf.remoteip[3]=(pcb->remote_ip.addr>>24)&0xff;//IADDR1 
		
		udp_demo_flag = 1;	//标记接收到数据了
		
		pbuf_free(p);//释放内存
	}else
	{
		udp_disconnect(pcb); 
	} 
} 
//UDP服务器发送数据，发送 tcp_demo_sendbuf 字符数组所存的数据
void UDP_senddata_test(struct udp_pcb *upcb)
{
	struct pbuf *ptr;
	ptr=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_demo_sendbuf),PBUF_POOL); //申请内存
	if(ptr)
	{
		ptr->payload=(void*)tcp_demo_sendbuf; 
		udp_send(upcb,ptr);	//udp发送数据 
		pbuf_free(ptr);//释放内存
	} 
} 

#endif


//关闭tcp连接
void UDP_connection_close(struct udp_pcb *upcb)
{
	udp_disconnect(upcb); 
	udp_remove(upcb);		//断开UDP连接 
}
