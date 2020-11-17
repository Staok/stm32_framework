#include "tcp_client.h" 



/*使用介绍：

创建连接：
	//TCP Client 初始化
	//lwip_inf.remoteip 为远程主机Server的IP，lwip_inf.ip为本地客户端的IP
	//初始化例子 ： tcp_client_test(TCP_Client_PCB_test,100,TCP_CLIENT_PORT);
	//返回0成功，其他失败
	u8 tcp_client_test(struct tcp_pcb *tcppcb, u8 remoteip_3, u16 tcp_client_port)
查询是否有客户端连接上：
	一直循环检查 TCP_Client_IS_CONN_Flag 宏是否为真，若主机失连后这里系统将其变0
注意：
	不可连续一次以上调用初始化API，在调用 关闭连接API 之前只能调用一次！
	初始化并且连接上后，周期检查 TCP_Client_IS_CONN_Flag 宏，如果失连，
	先调用	tcp_client_close(TCP_Client_PCB_test,0);
	再调用	tcp_client_init_test(TCP_Client_PCB_test,100,TCP_CLIENT_PORT);
关闭连接：
	//关闭tcp server连接
	//例子：tcp_client_close(TCP_Client_PCB_test,0);
	void tcp_client_close(struct tcp_pcb* tspcb,struct tcp_pcb* tspcbconn);
接收数据：
	检查 TCP_Client_IS_RECV_Flag 宏是否为真，为真则表示收到数据，
	从接收缓冲区 tcp_client_recvbuf 取出数据，
	然后调用 TCP_Client_Clear_RECV_Flag; 宏 清零接收标志位
发送数据：
	先给 tcp_client_sendbuf 填想要发的数据，需要以 '\0'为结尾！
		例如：sprintf_(tcp_client_sendbuf,"stm32_framework - STM32F407 TCP Client demo send data\r\n");
	然后调用 TCP_Client_SET_SEND_Flag; 宏 设置发送标志位，系统会自动发送 tcp_client_sendbuf 的数据

接收缓冲区 tcp_client_recvbuf 和 发送缓冲区 tcp_client_sendbuf 的空间各为 TCP_CLIENT_RX_BUFSIZE 和 TCP_CLIENT_TX_BUFSIZE 

*/


#if LWIP_TCP && (NO_SYS == 1)

//TCP Client接收数据缓冲区
char* tcp_client_recvbuf;	
//TCP Client发送数据缓冲区
char* tcp_client_sendbuf;			//需要以 '\0'为结尾！

//TCP Client 测试全局状态标记变量
//bit7:0,没有数据要发送;1,有数据要发送
//bit6:0,没有收到数据;1,收到数据了
//bit5:0,没有连接上服务器;1,连接上服务器了
//bit4:0,没有初始化或者经过了反初始化;1,初始化了
//bit3~0:保留
u8 tcp_client_flag;


//tcp_client的初始化标志位，用于防止连续一次以上调用初始化函数，系统调用，用户勿用
#define TCP_Client_SET_INITED_Flag		(tcp_client_flag|=0x10)
#define TCP_Client_Clear_INITED_Flag	(tcp_client_flag&=(~0x10))
#define TCP_Client_IS_INITED_Flag		(tcp_client_flag&0x10)	


struct tcp_pcb* TCP_Client_PCB_test;


//lwIP tcp_recv()函数的回调函数，接收到数据的回调函数 Called when data has been received.系统级的不用用户调用
err_t tcp_client_recv(void *arg,struct tcp_pcb *tpcb,struct pbuf *p,err_t err)
{ 
	u32 data_len=0;
	struct pbuf *q;
	struct tcp_client_struct *es;
	err_t ret_err; 
	LWIP_ASSERT("arg != NULL",arg != NULL);
	es=(struct tcp_client_struct *)arg; 
	if(p==NULL)//如果从服务器接收到空的数据帧就关闭连接
	{
		es->state=ES_TCPCLIENT_CLOSING;//需要关闭TCP 连接了 
 		es->p=p; 
		ret_err=ERR_OK;
	}else if(err!= ERR_OK)//当接收到一个非空的数据帧,但是err!=ERR_OK
	{ 
		if(p)pbuf_free(p);//释放接收pbuf
		ret_err=err;
	}else if(es->state==ES_TCPCLIENT_CONNECTED)	//当处于连接状态时
	{
		if(p!=NULL)//当处于连接状态并且接收到的数据不为空时
		{
			memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);  //数据接收缓冲区清零
			for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
			{
				//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
				//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
				if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//拷贝数据
				else memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
				data_len += q->len;  	
				if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
			}
			
			tcp_client_flag|=1<<6;		//标记接收到数据了！
 			
			tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
			pbuf_free(p);  	//释放内存
			ret_err=ERR_OK;
		}
	}else  //接收到数据但是连接已经关闭,
	{ 
		tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
		es->p=NULL;
		pbuf_free(p); //释放内存
		ret_err=ERR_OK;
	}
	return ret_err;
} 
//lwIP tcp_err函数的回调函数，系统级的不用用户调用
void tcp_client_error(void *arg,err_t err)
{  
	//这里我们不做任何处理
}

//此函数用来发送数据，系统级自动的，被 tcp_server_poll 函数调用，用户不用调用
void tcp_client_senddata(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	struct pbuf *ptr;
 	err_t wr_err=ERR_OK;
	while((wr_err==ERR_OK)&&es->p&&(es->p->len<=tcp_sndbuf(tpcb))) //将要发送的数据加入到发送缓冲队列中
	{
		ptr=es->p;
		wr_err=tcp_write(tpcb,ptr->payload,ptr->len,1);
		if(wr_err==ERR_OK)
		{  
			es->p=ptr->next;			//指向下一个pbuf
			if(es->p)pbuf_ref(es->p);	//pbuf的ref加一
			pbuf_free(ptr);				//释放ptr 
		}else if(wr_err==ERR_MEM)es->p=ptr;
		tcp_output(tpcb);		//将发送缓冲队列中的数据立即发送出去
	} 
}


//lwIP tcp_poll的回调函数，系统级的不用用户调用
//当给 tcp_client_flag 标志位加上发送标志后，会在此函数调用发送函数发送 tcp_client_sendbuf 的数据
//还判断当前连接是否关闭，如果关闭则调用 tcp_client_connection_close 清理变量并标记失连标志位
err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_client_struct *es; 
	es=(struct tcp_client_struct*)arg;
	if(es!=NULL)  //连接处于空闲可以发送数据
	{
		if(tcp_client_flag&(1<<7))	//判断是否有数据要发送 
		{
			es->p=pbuf_alloc(PBUF_TRANSPORT, strlen((char*)tcp_client_sendbuf),PBUF_POOL);	//申请内存 
			pbuf_take(es->p,(char*)tcp_client_sendbuf,strlen((char*)tcp_client_sendbuf));	//将tcp_client_sentbuf[]中的数据拷贝到es->p_tx中
			tcp_client_senddata(tpcb,es);//将tcp_client_sentbuf[]里面复制给pbuf的数据发送出去
			tcp_client_flag&=~(1<<7);	//清除数据发送标志！
			if(es->p)pbuf_free(es->p);	//释放内存
		}else if(es->state==ES_TCPCLIENT_CLOSING)
		{ 
 			tcp_client_close(tpcb,es);//关闭TCP连接
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
err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_client_struct *es;
	LWIP_UNUSED_ARG(len);
	es=(struct tcp_client_struct*)arg;
	if(es->p)tcp_client_senddata(tpcb,es);//发送数据
	return ERR_OK;
}


//lwIP TCP连接建立后调用回调函数，注册各种回调函数，系统级的不用用户调用
err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	struct tcp_client_struct *es=NULL;  
	if(err==ERR_OK)   
	{
		es=(struct tcp_client_struct*)mem_malloc(sizeof(struct tcp_client_struct));  //申请内存
		if(es) //内存申请成功
		{
 			es->state=ES_TCPCLIENT_CONNECTED;//状态为连接成功
			es->pcb=tpcb;  
			es->p=NULL; 
			tcp_arg(tpcb,es);        			//使用es更新tpcb的callback_arg
			tcp_recv(tpcb,tcp_client_recv);  	//初始化LwIP的tcp_recv回调功能
			tcp_err(tpcb,tcp_client_error); 	//初始化tcp_err()回调函数
			tcp_sent(tpcb,tcp_client_sent);		//初始化LwIP的tcp_sent回调功能
			tcp_poll(tpcb,tcp_client_poll,1); 	//初始化LwIP的tcp_poll回调功能
 			
			tcp_client_flag|=1<<5; 				//标记连接到服务器了！
			
			err=ERR_OK;
		}else
		{ 
			tcp_client_close(tpcb,es);//关闭连接
			err=ERR_MEM;	//返回内存分配错误
		}
	}else
	{
		tcp_client_close(tpcb,0);//关闭连接
	}
	return err;
}

//TCP Client 初始化
//lwip_inf.remoteip 为远程主机Server的IP，lwip_inf.ip为本地客户端的IP
//初始化例子 ： tcp_client_test(TCP_Client_PCB_test,100,TCP_CLIENT_PORT);
/*不可连续一次以上调用，在调用 关闭连接 之前只能调用一次
初始化并且连接上后，周期检查 TCP_Client_IS_CONN_Flag 宏，如果失连，
先调用	tcp_client_close(TCP_Client_PCB_test,0);
再调用	tcp_client_init_test(TCP_Client_PCB_test,100,TCP_CLIENT_PORT);*/
//返回0成功，其他失败
u8 tcp_client_init_test(struct tcp_pcb *tcppcb, u8 remoteip_3, u16 tcp_client_port)
{
	struct ip4_addr rmtipaddr;  	//远端ip地址
	
	if(TCP_Client_IS_INITED_Flag == 0)	//查询是否初始化过，用于防止连续一次以上调用初始化函数
	{
		tcp_client_recvbuf = (char*)mymalloc(InrRAM,TCP_CLIENT_RX_BUFSIZE);
		tcp_client_sendbuf = (char*)mymalloc(InrRAM,TCP_CLIENT_TX_BUFSIZE);
		if((tcp_client_recvbuf == NULL)||(tcp_client_sendbuf == NULL))return 4;
		
		sprintf_(tcp_client_sendbuf,"stm32_framework - STM32F407 TCP Client demo send data\r\n");
		
		//前三个IP保持和DHCP得到的IP一致
		lwip_inf.remoteip[0] = lwip_inf.ip[0];
		lwip_inf.remoteip[1] = lwip_inf.ip[1];
		lwip_inf.remoteip[2] = lwip_inf.ip[2];
		lwip_inf.remoteip[3] = remoteip_3;
		
		tcppcb = tcp_new();	//创建一个新的pcb
		if(tcppcb)			//创建成功
		{
			IP4_ADDR(&rmtipaddr,lwip_inf.remoteip[0],lwip_inf.remoteip[1],lwip_inf.remoteip[2],lwip_inf.remoteip[3]); 
			tcp_connect(tcppcb,&rmtipaddr,tcp_client_port,tcp_client_connected);  //连接到目的地址的指定端口上,当连接成功后回调tcp_client_connected()函数
			TCP_Client_SET_INITED_Flag;	//标记初始化了
			return 0;
		}else return 2;
	
	}else return 1;
}

//关闭与服务器的连接
//例子：tcp_client_close(TCP_Client_PCB_test,0);
void tcp_client_close(struct tcp_pcb *tpcb, struct tcp_client_struct * es)
{
	//移除回调
	tcp_abort(tpcb);//终止连接,删除pcb控制块
	tcp_arg(tpcb,NULL);  
	tcp_recv(tpcb,NULL);
	tcp_sent(tpcb,NULL);
	tcp_err(tpcb,NULL);
	tcp_poll(tpcb,NULL,0);  
	if(es)mem_free(es); 
	
	myfree(InrRAM,tcp_client_recvbuf);	
	myfree(InrRAM,tcp_client_sendbuf);
	
	tcp_client_flag&=~(1<<5);		//标记连接断开了
	TCP_Client_Clear_INITED_Flag;	//标记反初始化了
}

#endif


