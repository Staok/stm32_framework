#include "tcp_server_demo.h" 
#include "SysTick.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "malloc.h"
#include "stdio.h"
#include "string.h"  


//TCP Server接收数据缓冲区
u8 tcp_server_recvbuf[TCP_SERVER_RX_BUFSIZE];	
//TCP服务器发送数据内容
const u8 *tcp_server_sendbuf="PRECHIN-STM32F407 TCP Server send data\r\n";

//TCP Server 测试全局状态标记变量
//bit7:0,没有数据要发送;1,有数据要发送
//bit6:0,没有收到数据;1,收到数据了.
//bit5:0,没有客户端连接上;1,有客户端连接上了.
//bit4~0:保留
u8 tcp_server_flag;	 
 
 
//TCP Server 测试
void tcp_server_test(void)
{
	err_t err;  
	struct tcp_pcb *tcppcbnew;  	//定义一个TCP服务器控制块
	struct tcp_pcb *tcppcbconn;  	//定义一个TCP服务器控制块
	
	u8 *tbuf;
 	u8 key;
	u8 res=0;		
	u8 t=0; 
	u8 connflag=0;		//连接标记
	
	LCD_Clear(WHITE);	//清屏
	FRONT_COLOR=RED; 	//红色字体
	LCD_ShowString(30,30,200,16,16,"PRECHIN STM32F4");
	LCD_ShowString(30,50,200,16,16,"TCP Server Test");
	LCD_ShowString(30,70,200,16,16,"www.prechin.net");  
	LCD_ShowString(30,90,200,16,16,"KEY0:Send data");  
	LCD_ShowString(30,110,200,16,16,"KEY_UP:Quit");  
	tbuf=mymalloc(SRAMIN,200);	//申请内存
	if(tbuf==NULL)return ;		//内存申请失败了,直接退出
	sprintf((char*)tbuf,"Server IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);//服务器IP
	LCD_ShowString(30,130,210,16,16,tbuf);  
	sprintf((char*)tbuf,"Server Port:%d",TCP_SERVER_PORT);//服务器端口号
	LCD_ShowString(30,150,210,16,16,tbuf); 
	tcppcbnew=tcp_new();	//创建一个新的pcb
	if(tcppcbnew)			//创建成功
	{ 
		err=tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_SERVER_PORT);	//将本地IP与指定的端口号绑定在一起,IP_ADDR_ANY为绑定本地所有的IP地址
		if(err==ERR_OK)	//绑定完成
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			//设置tcppcb进入监听状态
			tcp_accept(tcppcbconn,tcp_server_accept); 	//初始化LWIP的tcp_accept的回调函数
		}else res=1;  
	}else res=1;
	FRONT_COLOR=BLUE;//蓝色字体
	while(res==0)
	{
		key=KEY_Scan(0);
		if(key==KEY_UP_PRESS)break;
		if(key==KEY0_PRESS)//KEY0按下了,发送数据
		{
			tcp_server_flag|=1<<7;//标记要发送数据
		}
		if(tcp_server_flag&1<<6)//是否收到数据?
		{
			LCD_Fill(30,210,tftlcd_data.width-1,tftlcd_data.height-1,WHITE);//清上一次数据
			LCD_ShowString(30,210,tftlcd_data.width-30,tftlcd_data.height-210,16,tcp_server_recvbuf);//显示接收到的数据			
			tcp_server_flag&=~(1<<6);//标记数据已经被处理了.
		}
		if(tcp_server_flag&1<<5)//是否连接上?
		{
			if(connflag==0)
			{ 
				sprintf((char*)tbuf,"Client IP:%d.%d.%d.%d",lwipdev.remoteip[0],lwipdev.remoteip[1],lwipdev.remoteip[2],lwipdev.remoteip[3]);//客户端IP
 				LCD_ShowString(30,170,230,16,16,tbuf);
				FRONT_COLOR=RED;
				LCD_ShowString(30,190,tftlcd_data.width-30,tftlcd_data.height-190,16,"Receive Data:");//提示消息		
				FRONT_COLOR=BLUE;//蓝色字体
				connflag=1;//标记连接了
			} 
		}else if(connflag)
		{
			LCD_Fill(30,170,tftlcd_data.width-1,tftlcd_data.height-1,WHITE);//清屏
			connflag=0;	//标记连接断开了
		}
		lwip_periodic_handle();
		delay_ms(2);
		t++;
		if(t==200)
		{
			t=0;
			LED1=!LED1;
		} 
	}   
	tcp_server_connection_close(tcppcbnew,0);//关闭TCP Server连接
	tcp_server_connection_close(tcppcbconn,0);//关闭TCP Server连接 
	tcp_server_remove_timewait(); 
	memset(tcppcbnew,0,sizeof(struct tcp_pcb));
	memset(tcppcbconn,0,sizeof(struct tcp_pcb)); 
	myfree(SRAMIN,tbuf);
} 
//lwIP tcp_accept()的回调函数
err_t tcp_server_accept(void *arg,struct tcp_pcb *newpcb,err_t err)
{
	err_t ret_err;
	struct tcp_server_struct *es; 
 	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);
	tcp_setprio(newpcb,TCP_PRIO_MIN);//设置新创建的pcb优先级
	es=(struct tcp_server_struct*)mem_malloc(sizeof(struct tcp_server_struct)); //分配内存
 	if(es!=NULL) //内存分配成功
	{
		es->state=ES_TCPSERVER_ACCEPTED;  	//接收连接
		es->pcb=newpcb;
		es->p=NULL;
		
		tcp_arg(newpcb,es);
		tcp_recv(newpcb,tcp_server_recv);	//初始化tcp_recv()的回调函数
		tcp_err(newpcb,tcp_server_error); 	//初始化tcp_err()回调函数
		tcp_poll(newpcb,tcp_server_poll,1);	//初始化tcp_poll回调函数
		tcp_sent(newpcb,tcp_server_sent);  	//初始化发送回调函数
		  
		tcp_server_flag|=1<<5;				//标记有客户端连上了
		lwipdev.remoteip[0]=newpcb->remote_ip.addr&0xff; 		//IADDR4
		lwipdev.remoteip[1]=(newpcb->remote_ip.addr>>8)&0xff;  	//IADDR3
		lwipdev.remoteip[2]=(newpcb->remote_ip.addr>>16)&0xff; 	//IADDR2
		lwipdev.remoteip[3]=(newpcb->remote_ip.addr>>24)&0xff; 	//IADDR1 
		ret_err=ERR_OK;
	}else ret_err=ERR_MEM;
	return ret_err;
}
//lwIP tcp_recv()函数的回调函数
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
			tcp_server_flag|=1<<6;	//标记接收到数据了
			lwipdev.remoteip[0]=tpcb->remote_ip.addr&0xff; 		//IADDR4
			lwipdev.remoteip[1]=(tpcb->remote_ip.addr>>8)&0xff; //IADDR3
			lwipdev.remoteip[2]=(tpcb->remote_ip.addr>>16)&0xff;//IADDR2
			lwipdev.remoteip[3]=(tpcb->remote_ip.addr>>24)&0xff;//IADDR1 
 			tcp_recved(tpcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
			pbuf_free(p);  	//释放内存
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
void tcp_server_error(void *arg,err_t err)
{  
	LWIP_UNUSED_ARG(err);  
	printf("tcp error:%x\r\n",(u32)arg);
	if(arg!=NULL)mem_free(arg);//释放内存
} 
//lwIP tcp_poll的回调函数
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
//lwIP tcp_sent的回调函数(当从远端主机接收到ACK信号后发送数据)
err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;
	LWIP_UNUSED_ARG(len); 
	es = (struct tcp_server_struct *) arg;
	if(es->p)tcp_server_senddata(tpcb,es);//发送数据
	return ERR_OK;
} 
//此函数用来发送数据
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
		delay_ms(10);//等待tcp_active_pcbs为空  
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
