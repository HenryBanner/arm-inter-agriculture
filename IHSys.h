#ifndef _LCD_H_
#define _LCD_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>
#include <termios.h> 
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>



//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/mman.h>
//#include <linux/input.h>
//struct 
//{
//	int lcd_fd;
//	int* plcd;
//}LCD;

int motor();
// ͼƬ��ʾ
int display_bmp(int x0, int y0);

//��ʼ����Ļ��ӳ��
int init_lcd();

//�ر���Ļ�ͽ�ӳ��
int uninit_lcd();

//ӳ��
void lcd_draw_point(int i, int j, int color);

//������
int get_xy();

//��ȡͼƬ����
int read_data();

//��ģ
void draw_word(int x0, int y0, int w, int h, int color, char s1[]);

//ͼƬת��
void bmp_switch();

//led���ƺ���
void led_ctrl(char* led_id, int on_or_off);

//����
int music_play();

//������ʾ
void lcd_number(int x0, int y0, double lf);

////��ʼ��gy39����
void init_tty(int fd);

//����
void gy39();
#endif
