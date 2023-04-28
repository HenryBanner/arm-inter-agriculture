#include "uart.h"

//烟雾线程
//void *mq01_fun(void *arg)
//{
//	int uart_fd = (int)*(int *)arg;
//	
//	mq_01_send(uart_fd);
//}

//gy39线程
void *gy39_fun(void *arg)
{
	int uart_fd = (int)*(int *)arg;
	
	gy39_send(uart_fd);
}

//串口打开
int uart_open(const char *path)
{
	int fd = open(path, O_RDWR);
	if(fd == -1)
	{
		perror("打开串口失败!");
		return -1;
	}
	
	return fd;
}

//串口初始化
int uart_init(int uart_fd)
{
	struct termios old_cfg,new_cfg;
	
	//1.保存原先串口配置
	int ret = tcgetattr(uart_fd,&old_cfg);
	if(ret == -1)
	{
		perror("保存原先串口配置失败");
		return -1;
	}
	
	//2.激活选项
	cfmakeraw(&new_cfg);
	new_cfg.c_cflag |= CLOCAL | CREAD;
	
	//3.设置波特率,9600
	cfsetispeed(&new_cfg, B9600);
	cfsetospeed(&new_cfg, B9600);
	
	//4.设置字符大小,数据位8位
	new_cfg.c_cflag &= ~CSIZE;	//清空原来的数据位
	new_cfg.c_cflag |= CS8;
	
	//5.设置奇偶校验位,无校验
	new_cfg.c_cflag &= ~PARENB;
	
	//6.停止位,1位
	new_cfg.c_cflag &= ~CSTOPB;
	
	//7.设置最少字符和等待时间
	new_cfg.c_cc[VTIME] = 0;
	new_cfg.c_cc[VMIN] = 1;
	
	//8.清空串口缓冲
	tcflush(uart_fd, TCIFLUSH);
	
	//9.激活配置
	ret = tcsetattr(uart_fd, TCSANOW,&new_cfg);
	if(ret == -1)
	{
		perror("串口激活配置失败");
		return -1;
	}
	return 0;
}

//烟雾传感器发送命令
void mq_01_send(int uart_fd)
{
	//发送的命令
	char cmd_buf[9]={0xff,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
	
	while(1)
	{
		write(uart_fd, cmd_buf, 9);
		sleep(1);
		mq_01_recv(uart_fd);
		sleep(1);
	}
}

//烟雾传感器接收数据
void mq_01_recv(int uart_fd)
{
	char recv_buf[9] = {0};
	int len = 0;
	int ret = 0;
	while(len < 9)
	{
		ret = read(uart_fd, recv_buf+len, 9-len);
		len += ret;
	}
	
	int i;
	printf("===============================\n");
	printf("data：");
	for(i = 0; i < 9; i++)
	{
		printf("%x ", recv_buf[i]);
	}
	printf("\n");
	int mq01_value = (recv_buf[2]<<8)|recv_buf[3];
	printf("yanwu = %d\n", mq01_value);
	printf("===============================\n");
	
	char show_mq01[50] = {0};
	sprintf(show_mq01, "yanwuzhi:%d", mq01_value);
	
	if(udp_flag == 1)
	{
		sendto(socket_fd,show_mq01,strlen(show_mq01),0,(const struct sockaddr *)&recv_addr,sizeof(recv_addr));
		udp_flag = 0;
	}
	
	Clean_Area(50,50,240,40,0xff);
	Display_characterX(50,50,show_mq01,0xffffff,2);
}

//gy39发送命令
void gy39_send(int uart_fd)
{
	//发送的命令
	char cmd_buf[3] = {0xa5,0x83,0x28};
	
	while(1)
	{
		write(uart_fd, cmd_buf, 3);
		sleep(1);
		gy39_recv(uart_fd);
		sleep(1);
	}
}

//gy39接收数据
void gy39_recv(int uart_fd)
{
	char ch;
	int data_len = 0;	//数据长度
	int gy39_i = 0;		//数组下标
	char gy39_buf[16] = {0};	//接收的数组
	enum gy39_flag gy39_flag;	
	gy39_flag = gy39_begin;
	
	while(1)
	{
		read(uart_fd, &ch, 1);
		if(gy39_flag == gy39_begin)
		{
			//接收到第一个5a
			if(ch == 0x5a)
			{
				gy39_buf[gy39_i++] = ch;
				gy39_flag = gy39_5a_1;
			}
		}
		else if(gy39_flag == gy39_5a_1)
		{
			//接收到第二个5a
			if(ch == 0x5a)
			{
				gy39_buf[gy39_i++] = ch;
				gy39_flag = gy39_5a_2;
			}
			else
			{
				gy39_i = 0;
				gy39_flag = gy39_begin;
			}
		}
		else if(gy39_flag == gy39_5a_2)
		{
			//接收到数据类型
			if(ch == 0x15 || ch == 0x45 || ch == 0x55)
			{
				gy39_buf[gy39_i++] = ch;
				gy39_flag = gy39_type;
			}
			else
			{
				gy39_i = 0;
				gy39_flag = gy39_begin;
			}
		}
		else if(gy39_flag == gy39_type)
		{
			//接收到数据长度
			if(ch == 0x04 || ch == 0x0a || ch == 0x01)
			{
				gy39_buf[gy39_i++] = ch;
				gy39_flag = gy39_len;
				data_len = ch;
			}
			else
			{
				gy39_i = 0;
				gy39_flag = gy39_begin;
			}
		}
		else if(gy39_flag == gy39_len)
		{
			//接收数据
			gy39_buf[gy39_i++] = ch;
			data_len--;
			if(data_len == 0)
			{
				//数据信息接收完毕
				gy39_flag = gy39_data;
			}
		}
		else if(gy39_flag == gy39_data)
		{
			//接收到校验和
			gy39_buf[gy39_i++] = ch;
			gy39_flag = gy39_and;
			break;
		}
	}
	
	int i;
	printf("===================================\n");
	printf("data:");
	for(i = 0; i < 16; i++)
	{
		printf("%x ", gy39_buf[i]);
	}
	printf("\n");
	printf("===================================\n");
	
	char show_gy39[50];
	
	if(gy39_buf[2] == 0x15)
	{
		//光照强度
		int Lux = (gy39_buf[4]<<24)|(gy39_buf[5]<<16)|(gy39_buf[6]<<8)|gy39_buf[7];
		/****光强*****/
		bzero(show_gy39,sizeof(show_gy39));
		sprintf(show_gy39, "Lux=%.2flux", Lux/100.0f);
		Clean_Area(50,250,240,40,0xff);
		Display_characterX(50,250,show_gy39,0xffffff,2);
	}
	else if(gy39_buf[2] == 0x45)
	{
		int T = (gy39_buf[4]<<8)|gy39_buf[5];	//温度
		int P = (gy39_buf[6]<<24)|(gy39_buf[7]<<16)|(gy39_buf[8]<<8)|gy39_buf[9];	//气压
		int Hum = (gy39_buf[10]<<8)|gy39_buf[11];	//湿度
		int H = (gy39_buf[12]<<8)|gy39_buf[13];		//海拔
		
		/****温度*****/
		bzero(show_gy39,sizeof(show_gy39));
		sprintf(show_gy39, "T=%.2fC", T/100.0f);
		Clean_Area(50,90,240,40,0xff);
		Display_characterX(50,90,show_gy39,0xffffff,2);
		/****气压*****/
		bzero(show_gy39,sizeof(show_gy39));
		sprintf(show_gy39, "P=%.2fPa", P/100.0f);
		Clean_Area(50,130,240,40,0xff);
		Display_characterX(50,130,show_gy39,0xffffff,2);
		/****湿度*****/
		bzero(show_gy39,sizeof(show_gy39));
		sprintf(show_gy39, "Hum=%.2f%%", Hum/100.0f);
		Clean_Area(50,170,240,40,0xff);
		Display_characterX(50,170,show_gy39,0xffffff,2);
		/****海拔*****/
		bzero(show_gy39,sizeof(show_gy39));
		sprintf(show_gy39, "H=%dm", H);
		Clean_Area(50,210,240,40,0xff);
		Display_characterX(50,210,show_gy39,0xffffff,2);
	}
	
}


