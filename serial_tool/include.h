#ifndef INCLUDE_H
#define INCLUDE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define BAUNDRATE B115200
#define UART_DEVICE "/dev/ttyUSB0"
#define TRANS_FILE "test.bin"
#define BUFFER_SIZE 256
#define RESPONSE_SIZE 256
#define FILE_BUF_SIZE 20480		//20k
#define FALSE -1
#define TRUE 0

#define TRANS_SUCCESS 0
#define TRANS_CONTINUE 1

#define min(a,b) (a<b)?a:b

extern	unsigned char buffer[FILE_BUF_SIZE];
extern	int fd,fp;
extern	unsigned char wrt_buf[FILE_BUF_SIZE];

#endif
