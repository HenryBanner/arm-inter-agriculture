
/*************************************************
*ͷ�ļ�
*************************************************/
#include "IHSys.h"
  
#define GEC6818_MOTOR_FORWARD	_IO('M', 0x01)
#define GEC6818_MOTOR_BACKWARD	_IO('M', 0x02)
#define GEC6818_MOTOR_STOP	_IO('M', 0x03)

int motor()
{
	int fd,i,j,speed;
	fd = open("/dev/stepmotor", O_RDWR); 
	if(fd) 
	{
		for(j=0;j<5;j++)
		{
			for(i=0;i<100;i++)
				ioctl(fd,GEC6818_MOTOR_FORWARD,30);//3
			
			//ioctl(fd,GEC6818_MOTOR_STOP,0);
			printf("fan is working!\n");
			// sleep(3);

			// for(i=0;i<100;i++)
			// 	ioctl(fd,GEC6818_MOTOR_BACKWARD,8);
			
		}
		printf("fan is working!\n");
	}
	close(fd);
	return 0;
}

