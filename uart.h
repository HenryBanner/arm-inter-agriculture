#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "font.h"
#include <pthread.h>
#include "udp.h"

enum gy39_flag
{
	gy39_begin,		//起始状态
	gy39_5a_1,		//接收到第一个5a
	gy39_5a_2,		//接收到第二个5a
	gy39_type,		//接收到数据类型
	gy39_len,		//接收到数据长度
	gy39_data,		//数据信息接收完毕
	gy39_and,		//接收到校验和
};

int uart_open(const char *path);
int uart_init(int uart_fd);
void mq_01_send(int uart_fd);
void mq_01_recv(int uart_fd);
void gy39_send(int uart_fd);
void gy39_recv(int uart_fd);
void *mq01_fun(void *arg);
void *gy39_fun(void *arg);

#endif /* #ifndef __UART_H__ */
