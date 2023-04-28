#include "udp.h"
#include "IHSys.h"
struct sockaddr_in recv_addr;	//ipv4结构体，接收
int udp_flag = 0;				//接收标志位，0:未接收,1:已接收到
int socket_fd = 0;				//套接字

//UDP初始化
int udp_init(void)
{
	//1.创建套接字
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(socket_fd == -1)
	{
		perror("socket error!");
		return -1;
	}
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(2234);	//端口号
	//address.sin_addr.s_addr = inet_addr("192.168.0.88");	//IP地址
	address.sin_addr.s_addr = INADDR_ANY;	//绑定所有IP地址,相当于inet_addr("0.0.0.0")
	
	//2.绑定
	int ret = bind(socket_fd, (const struct sockaddr *)&address,sizeof(address));
	if(ret == -1)
	{
		perror("bind error!");
		return -1;
	}
	
	return socket_fd;
}

//UDP接收，线程
void *udp_recv(void *arg)
{
	int socket_fd = (int)*(int *)arg;
	
	char recv_buf[1024];
	int send_buf[5];
	send_buf[0] = send_L;
	send_buf[1] = send_T;
	send_buf[2] = send_P;
	send_buf[3] = send_M;
	send_buf[4] = send_H;
	 
	socklen_t address_len;
	
	while(1)
	{
		
		bzero(recv_buf, sizeof(recv_buf));
		recvfrom(socket_fd,recv_buf,1024,0,(struct sockaddr *)&recv_addr,&address_len);
		printf("=========================\n");
		printf("IP地址:%s\n", inet_ntoa(recv_addr.sin_addr));
		printf("端口号:%d\n", ntohs(recv_addr.sin_port));
		printf("消息:%s\n", recv_buf);
		printf("=========================\n");
		if(strstr(recv_buf,"MUSIC_PLAY"))
		{
			motor();
		}
		if(strstr(recv_buf, "GET_VIDEO"))
		{
			sendto(socket_fd,send_buf,1024,0,(struct sockaddr *)&recv_addr,address_len);
			//sendto(socket_fd,send_L,1024,0,(struct sockaddr *)&recv_addr,address_len);
			//sendto(socket_fd,send_T,1024,0,(struct sockaddr *)&recv_addr,address_len);
			//sendto(socket_fd,send_P,1024,0,(struct sockaddr *)&recv_addr,address_len);
			//sendto(socket_fd,send_M,1024,0,(struct sockaddr *)&recv_addr,address_len);
			//sendto(socket_fd,send_H,1024,0,(struct sockaddr *)&recv_addr,address_len);
		}
		if(strstr(recv_buf,"MUSIC_STOP_CONT"))
		{
			printf("开始!\n");
		}
		//此处可以实现APP控制程序
	}
}
