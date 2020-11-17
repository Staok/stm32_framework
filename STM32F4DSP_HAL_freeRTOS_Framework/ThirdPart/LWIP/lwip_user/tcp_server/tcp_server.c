#include "tcp_server.h"  

/*历史遗留问题，字符串操作都是用string.c标准库的API*/


/*使用介绍：

创建连接：
	//TCP Server 初始化，输入参数：tcp_pcb，选择 remoteip[3] ， 端口号
	//初始化例子： tcp_server_init_test(TCP_Server_PCB_test,TCP_Server_PCBCONN_test,TCP_SERVER_PORT_test);
	//创建成功后，本地IP即TCP Server的IP，lwip_inf.remoteip 即写入远程连接上的IP
	//返回0成功，其他失败
	u8 tcp_server_init_test(struct tcp_pcb* tcppcbnew,struct tcp_pcb* tcppcbconn,u16_t tcp_server_port)
查询是否有客户端连接上：
	一直循环检查 TCP_Server_IS_CONN_Flag 宏是否为真，若从机失连后这里系统将其变0
注意：
	不可连续一次以上调用初始化API，在调用 关闭连接API 之前只能调用一次！
	初始化并且连接上后，周期检查 TCP_Server_IS_CONN_Flag 宏，如果失连，
	先调用	tcp_server_close(TCP_Server_PCB_test,TCP_Server_PCBCONN_test);
	再调用	tcp_server_init_test(TCP_Server_PCB_test,TCP_Server_PCBCONN_test,TCP_SERVER_PORT_test);

关闭连接：
	//关闭tcp server连接
	//例子：tcp_server_close(TCP_Server_PCB_test,TCP_Server_PCBCONN_test);
	void tcp_server_close(struct tcp_pcb* tspcb,struct tcp_pcb* tspcbconn)
接收数据：
	检查 TCP_Server_IS_RECV_Flag 宏是否为真，为真则表示收到数据，
	从接收缓冲区 tcp_server_recvbuf 取出数据，
	然后调用 TCP_Server_Clear_RECV_Flag; 宏 清零接收标志位
发送数据：
	先给 tcp_server_sendbuf 填想要发的数据，需要以 '\0'为结尾！
		例如：sprintf_(tcp_server_sendbuf,"stm32_framework - STM32F407 TCP Server demo send data\r\n");
	然后调用 TCP_Server_SET_SEND_Flag; 宏 设置发送标志位，系统会自动发送 tcp_server_sendbuf 的数据

接收缓冲区 tcp_server_recvbuf 和 发送缓冲区 tcp_server_sendbuf 的空间各为 TCP_SERVER_RX_BUFSIZE 和 TCP_SERVER_TX_BUFSIZE 

*/

#if LWIP_TCP && (NO_SYS == 1)

//TCP Server接收数据缓冲区
char* tcp_server_recvbuf;	
//TCPP Server发送数据缓冲区
char* tcp_server_sendbuf;		//需要以 '\0'为结尾！

//TCP Server 测试全局状态标记变量
//bit7:0,没有数据要发送;1,有数据要发送
//bit6:0,没有收到数据;1,收到数据了
//bit5:0,没有客户端连接上;1,有客户端连接上了
//bit4:0,没有初始化或者经过了反初始化;1,初始化了
//bit3~0:保留
u8 tcp_server_flag;


//tcp_server的初始化标志位，用于防止连续一次以上调用初始化函数，系统调用，用户勿用
#define TCP_Server_SET_INITED_Flag		(tcp_server_flag|=0x10)
#define TCP_Server_Clear_INITED_Flag	(tcp_server_flag&=(~0x10))
#define TCP_Server_IS_INITED_Flag		(tcp_server_flag&0x10)

struct tcp_pcb *TCP_Server_PCB_test;  		//定义一个TCP服务器控制块
struct tcp_pcb *TCP_Server_PCBCONN_test;  	//定义一个TCP服务器控制块


//lwIP tcp_recv()函数的回调函数，接收到数据的回调函数 Called when data has been received.系统级的不用用户调用
err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	err_t ret_err;
	u32 data_len = 0;
	struct pbuf *q;
  	struct tcp_server_struct *es;
	LWIP_ASSERT("arg != NULL",arg != NULL);
	es=(struct tcp_server_struct *)arg;
	if(p==NULL) //从客户端接收到空数据
	{
		es->state=ES_TCPSERVER_CLOSING;//需要关闭TCP 连接了
		es->p=p; 
		ret_err=ERR_OK;
	}else if(err!=ERR_OK)	//从客户端接收到一个非空数据,但是由于某种原因err!=ERR_OK
	{
		if(p)pbuf_free(p);	//释放接收pbuf
		ret_err=err;
	}else if(es->state==ES_TCPSERVER_ACCEPTED) 	//处于连接状态
	{
		if(p!=NULL)  //当处于连接状态并且接收到的数据不为空时将其打印出来
		{
			memset(tcp_server_recvbuf,0,TCP_SERVER_RX_BUFSIZE);  //数据接收缓冲区清零
			for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
			{
				//判断要拷贝到TCP_SERVER_RX_BUFSIZE中的数据是否大于TCP_SERVER_RX_BUFSIZE的剩余空间，如果大于
				//的话就只拷贝TCP_SERVER_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
				if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//拷贝数据
				else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
				data_len += q->len;  	
				if(data_len > TCP_SERVER_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
			}
			
			tcp_server_flag|=1<<6;			//标记接收到数据了！
			
			lwip_inf.remoteip[0]=tpcb->remote_ip.addr&0xff; 		//IADDR4
			lwip_inf.remoteip[1]=(tpcb->remote_ip.addr>>8)&0xff; 	//IADDR3
			lwip_inf.remoteip[2]=(tpcb->remote_ip.addr>>16)&0xff;	//IADDR2
			lwip_inf.remoteip[3]=(tpcb->remote_ip.addr>>24)&0xff;	//IADDR1
 			tcp_recved(tpcb,p->tot_len);	//用于获取接收数据,通知LWIP可以获取更多数据
			pbuf_free(p);  					//释放内存
			ret_err=ERR_OK;
		}
	}else//服务器关闭了
	{
		tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
		es->p=NULL;
		pbuf_free(p); //释放内存
		ret_err=ERR_OK;
	}
	return ret_err;
}
//lwIP tcp_err函数的回调函数
//Called when the pcb receives a RST or is unexpectedly closed for any other reason.
void tcp_server_error(void *arg,err_t err)
{  
	char* buf = mymalloc(InrRAM,50);
	LWIP_UNUSED_ARG(err);
	
	sprintf_(buf,"tcp error:%x\r\n",(u32)arg);
	FaultASSERT(buf,7,flag_Warning);
	myfree(InrRAM,buf);
	
	if(arg!=NULL)mem_free(arg);//释放内存
}

//关闭tcp连接
void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	tcp_close(tpcb);
	tcp_arg(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_recv(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);
	if(es)mem_free(es); 
	tcp_server_flag&=~(1<<5);//标记连接断开了
}

//此函数用来发送数据，系统级自动的，被 tcp_server_poll 函数调用，用户不用调用
void tcp_server_senddata(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	struct pbuf *ptr;
	u16 plen;
	err_t wr_err=ERR_OK;
	 while((wr_err==ERR_OK)&&es->p&&(es->p->len<=tcp_sndbuf(tpcb)))
	 {
		ptr=es->p;
		wr_err=tcp_write(tpcb,ptr->payload,ptr->len,1); //将要发送的数据加入发送缓冲队列中
		if(wr_err==ERR_OK)
		{ 
			plen=ptr->len;
			es->p=ptr->next;			//指向下一个pbuf
			if(es->p)pbuf_ref(es->p);	//pbuf的ref加一
			pbuf_free(ptr);
			tcp_recved(tpcb,plen); 		//更新tcp窗口大小
		}else if(wr_err==ERR_MEM)es->p=ptr;
		tcp_output(tpcb);   //将发送缓冲队列中的数据发送出去
	 }
}

//lwIP tcp_poll的回调函数，Called periodically as specified by，系统级的不用用户调用
//当给 tcp_server_flag 标志位加上发送标志后，会在此函数调用发送函数发送 tcp_server_sendbuf 的数据
//还判断当前连接是否关闭，如果关闭则调用 tcp_server_connection_close 清理变量并标记失连标志位
err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server_struct *es; 
	es=(struct tcp_server_struct *)arg; 
	if(es!=NULL)
	{
		if(tcp_server_flag&(1<<7))	//判断是否有数据要发送
		{
			es->p=pbuf_alloc(PBUF_TRANSPORT,strlen((char*)tcp_server_sendbuf),PBUF_POOL);//申请内存
			pbuf_take(es->p,(char*)tcp_server_sendbuf,strlen((char*)tcp_server_sendbuf));
			tcp_server_senddata(tpcb,es); 		//轮询的时候发送要发送的数据
			tcp_server_flag&=~(1<<7);  			//清除数据发送标志位
			if(es->p!=NULL)pbuf_free(es->p); 	//释放内存	
		}else if(es->state==ES_TCPSERVER_CLOSING)//需要关闭连接?执行关闭操作
		{
			tcp_server_connection_close(tpcb,es);//关闭连接
		}
		ret_err=ERR_OK;
	}else
	{
		tcp_abort(tpcb);//终止连接,删除pcb控制块
		ret_err=ERR_ABRT; 
	}
	return ret_err;
} 
//lwIP tcp_sent的回调函数(当从远端主机接收到ACK信号后发送数据)，系统级的不用用户调用
err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;
	LWIP_UNUSED_ARG(len); 
	es = (struct tcp_server_struct *) arg;
	if(es->p)tcp_server_senddata(tpcb,es);//发送数据
	return ERR_OK;
}

//lwIP tcp_accept()的回调函数，注册各种回调函数，
//客户端连接上本地Server IP时的回调函数，系统级的不用用户调用
//Called when a new connection can be accepted on a listening pcb.
err_t tcp_server_accept(void *arg,struct tcp_pcb *newpcb,err_t err)
{
	err_t ret_err;
	struct tcp_server_struct *es; 
 	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);
	tcp_setprio(newpcb,TCP_PRIO_NORMAL);//设置新创建的pcb优先级
	es=(struct tcp_server_struct*)mem_malloc(sizeof(struct tcp_server_struct)); //分配内存
 	if(es != NULL) //内存分配成功
	{
		es->state=ES_TCPSERVER_ACCEPTED;  	//接收连接
		es->pcb=newpcb;
		es->p=NULL;
		
		tcp_arg(newpcb,es);
		tcp_recv(newpcb,tcp_server_recv);	//初始化tcp_recv()的回调函数
		tcp_err(newpcb,tcp_server_error); 	//初始化tcp_err()回调函数
		tcp_poll(newpcb,tcp_server_poll,1);	//初始化tcp_poll回调函数
		tcp_sent(newpcb,tcp_server_sent);  	//初始化发送回调函数
		  
		tcp_server_flag|=1<<5;				//标记有客户端连上了！
		
		//标记远程连接上的IP
		lwip_inf.remoteip[0] = newpcb->remote_ip.addr&0xff; 		//IADDR4
		lwip_inf.remoteip[1] = (newpcb->remote_ip.addr>>8)&0xff;  	//IADDR3
		lwip_inf.remoteip[2] = (newpcb->remote_ip.addr>>16)&0xff; 	//IADDR2
		lwip_inf.remoteip[3] = (newpcb->remote_ip.addr>>24)&0xff; 	//IADDR1 
		ret_err=ERR_OK;
	}else ret_err=ERR_MEM;
	return ret_err;
}


//TCP Server 初始化，输入参数：tcp_pcb，选择 remoteip[3] ， 端口号
//初始化例子： tcp_server_init_test(TCP_Server_PCB_test,TCP_Server_PCBCONN_test,TCP_SERVER_PORT_test);
//创建成功后，本地IP即TCP Server的IP，lwip_inf.remoteip 即写入远程连接上的IP
/*不可连续一次以上调用，在调用 关闭连接 之前只能调用一次
初始化并且连接上后，周期检查 TCP_Server_IS_CONN_Flag 宏，如果失连，
先调用	tcp_server_connection_close(TCP_Server_PCB_test,TCP_Server_PCBCONN_test);
再调用	tcp_server_init_test(TCP_Server_PCB_test,TCP_Server_PCBCONN_test,TCP_SERVER_PORT_test);*/
//返回0成功，其他失败
u8 tcp_server_init_test(struct tcp_pcb* tcppcbnew,struct tcp_pcb* tcppcbconn,u16_t tcp_server_port)
{
	err_t err;
	
	if(TCP_Server_IS_INITED_Flag == 0)	//查询是否初始化过，用于防止连续一次以上调用初始化函数
	{
			tcp_server_recvbuf = (char*)mymalloc(InrRAM,TCP_SERVER_RX_BUFSIZE);
		tcp_server_sendbuf = (char*)mymalloc(InrRAM,TCP_SERVER_TX_BUFSIZE);
		if((tcp_server_recvbuf == NULL)||(tcp_server_sendbuf == NULL))return 4;
		
		sprintf_(tcp_server_sendbuf,"stm32_framework - STM32F407 TCP Server demo send data\r\n");
		
		tcppcbnew = tcp_new();		//创建一个新的pcb
		if(tcppcbnew)				//创建成功
		{ 
			err = tcp_bind(tcppcbnew,IP_ADDR_ANY,tcp_server_port);	//将本地IP与指定的端口号绑定在一起,IP_ADDR_ANY为绑定本地所有的IP地址
			if(err == ERR_OK)		//绑定完成
			{
				tcppcbconn = tcp_listen(tcppcbnew); 			//设置tcppcb进入监听状态
				tcp_accept(tcppcbconn,tcp_server_accept); 		//初始化LWIP的tcp_accept的回调函数
				TCP_Server_SET_INITED_Flag;	//标记初始化了
				return 0;
			}else return 2;
		}else return 1;
	}return 3;
}
 
 
extern void tcp_pcb_purge(struct tcp_pcb *pcb);	//在 tcp.c里面 
extern struct tcp_pcb *tcp_active_pcbs;			//在 tcp.c里面 
extern struct tcp_pcb *tcp_tw_pcbs;				//在 tcp.c里面  
//强制删除TCP Server主动断开时的time wait
void tcp_server_remove_timewait(void)
{
	struct tcp_pcb *pcb,*pcb2; 
	while(tcp_active_pcbs!=NULL)
	{
		lwip_periodic_handle();//继续轮询
		HAL_Delay(10);//等待tcp_active_pcbs为空
	}
	pcb=tcp_tw_pcbs;
	while(pcb!=NULL)//如果有等待状态的pcbs
	{
		tcp_pcb_purge(pcb); 
		tcp_tw_pcbs=pcb->next;
		pcb2=pcb;
		pcb=pcb->next;
		memp_free(MEMP_TCP_PCB,pcb2);	
	}
}


//关闭tcp server连接，用户调用
//例子：tcp_server_close(TCP_Server_PCB_test,TCP_Server_PCBCONN_test);
void tcp_server_close(struct tcp_pcb* tspcb,struct tcp_pcb* tspcbconn)
{
	tcp_server_connection_close(tspcb,0);
	tcp_server_connection_close(tspcbconn,0);
	tcp_server_remove_timewait(); 
	memset(tspcb,0,sizeof(struct tcp_pcb));
	memset(tspcbconn,0,sizeof(struct tcp_pcb)); 
	
	myfree(InrRAM,tcp_server_recvbuf);
	myfree(InrRAM,tcp_server_sendbuf);
	
	TCP_Server_Clear_INITED_Flag;	//标记反初始化了
}

#endif

