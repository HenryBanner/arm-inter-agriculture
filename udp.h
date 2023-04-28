#ifndef __UDP_H__
#define __UDP_H__

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
int send_L, send_T, send_P, send_M, send_H; //光照、温度、压强、湿度、海拔
extern struct sockaddr_in recv_addr;	//ipv4结构体，接收
extern int udp_flag;				//接收标志位，0:未接收,1:已接收到
extern int socket_fd;				//套接字

int udp_init(void);
void *udp_recv(void *arg);

#endif /* __UDP_H__ */
