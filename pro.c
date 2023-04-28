#include "IHSys.h"
#include "uart.h"
#include "udp.h"
//获取坐标
void* get_user_touch()
{
	while (1)
	{
		get_xy();
		//printf("x = %d y = %d\n", ret_x, ret_y);
	}
}

//测量
/*void* get_gy39()
{
	while (1)
	{
		gy39();
	}
}
*/
//音乐
void* get_music()
{
	while (1)
	{
		music_play();
	}
}

//判断触摸点
void* get_bmp_switch()
{
	while (1)
	{
		bmp_switch();
	}
}

int main()
{

	//初始化屏幕和映射
	init_lcd();
	int uart1_fd = uart_open("/dev/ttySAC3");
	// int uart2_fd = uart_open("/dev/ttySAC2");
	int socket_fd;
	socket_fd = udp_init(); // UDP初始化
	uart_init(uart1_fd);
	// uart_init(uart2_fd);
	Init_Font(); // 字库初始化
	//读取图片数据
	read_data(1);

	display_bmp(0, 0);

	pthread_t id[5];
	pthread_create(&id[0], NULL, get_user_touch, (void*)NULL);
	pthread_create(&id[1], NULL, udp_recv, &socket_fd);
	pthread_create(&id[2], NULL, gy39_fun, &uart1_fd);
	pthread_create(&id[3], NULL, get_music, (void*)NULL);
	pthread_create(&id[4], NULL, get_bmp_switch, (void*)NULL);

	while (1)
	{
		;
	}

	//关闭屏幕和解映射
	uninit_lcd();
	UnInit_Font(); // 字库释放资源
	close(uart1_fd);
	return 0;
}
