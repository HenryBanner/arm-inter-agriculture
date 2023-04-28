//#include "uart.h"
#include "udp.h"
//#include "IHSys.h"
int main(void)
{
	//int uart1_fd = uart_open("/dev/ttySAC1");
	// int uart2_fd = uart_open("/dev/ttySAC2");

	socket_fd = udp_init(); // UDP初始化
	//uart_init(uart1_fd);
	// uart_init(uart2_fd);
	//Init_Font(); // 字库初始化

	/***烟雾线程***/
	// pthread_t thread_mq01;
	// pthread_create(&thread_mq01, NULL, mq01_fun, &uart1_fd);

	/***GY39线程***/
	//pthread_t thread_gy39;
	//pthread_create(&thread_gy39, NULL, gy39_fun, &uart1_fd);

	/***UDP接收线程***/
	pthread_t udp_thread;
	pthread_create(&udp_thread, NULL, udp_recv, &socket_fd);

	/***主要功能***/

	// 等待线程退出,阻塞等待
	// pthread_join(thread_mq01, NULL);
	//pthread_join(thread_gy39, NULL);
	pthread_join(udp_thread, NULL);

	//UnInit_Font(); // 字库释放资源
	//close(uart1_fd);
	// close(uart2_fd);

	return 0;
}
