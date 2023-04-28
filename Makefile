all:
	arm-linux-gcc pro.c IHSys.c  udp.c stepmotor_test.c -o main -I./lib -L./lib -lfont -lpthread
