#include "cmd.h"

unsigned char version_id[4] = {0x77, 0x9B, 0xAF, 0xAF};
unsigned short nwrite;
unsigned short crc;
unsigned short wrt_len;
int i;

int wrt_buf_pos = 0;

void Cmd_01(void){
	buffer[0] = 0xAA;	//header,constant
	buffer[1] = 0x01;	//command number
	buffer[2] = 0x00;	//message  length, minimum is 7
	buffer[3] = 0x07;	//message  length is 2 Bytes
	buffer[4] = 0x00;	//datalen, 0 means command frame
	buffer[5] = 0x00;	//crc16
	buffer[6] = 0x00;       //crc16


	nwrite = write(fd, buffer , 7);

	if(nwrite == -1){
		printf("write error!\n");
	}

}

void Cmd_02(void){
	buffer[0] = 0xAA;	//header,constant
	buffer[1] = 0x02;
	buffer[2] = 0x00;
	buffer[3] = 0x0B;
	buffer[4] = 0x00;
	buffer[5] = version_id[0];
	buffer[6] = version_id[1];
	buffer[7] = version_id[2];
	buffer[8] = version_id[3];

	crc = CRC_16C (buffer + 5, 4, 0);
	buffer[9] = crc >> 8;
	buffer[10] = (crc & 0x00ff);
	nwrite = write(fd, buffer , 11);
	if(nwrite == -1){
		printf("write error!\n");
	}

}

void Cmd_03(unsigned short size_write){
	buffer[0] = 0xAA;	//header,constant
	buffer[1] = 0x03;
	buffer[2] = 0x00;
	buffer[3] = 0x09;
	buffer[4] = 0x00;
	
	buffer[5] = (size_write & 0xff00) >> 8;
	buffer[6] = size_write & 0x00ff;
	
	crc = CRC_16C (wrt_buf, size_write, 0);
	printf("crc = %d\n",crc);
	buffer[7] = crc >> 8;
	buffer[8] = (crc & 0x00ff);
	nwrite = write(fd, buffer , 9);
	if(nwrite == -1){
		printf("write error!\n");
	}
	
}

int Cmd_04(unsigned short file_size){
	static unsigned short count = 0;
	static unsigned short data_len = 0;

	count ++;
//	printf("count = %d\n",count);
	data_len=min(BUFFER_SIZE,file_size);
	wrt_len = data_len + 7;
//	printf("wrt_len = %d\n",wrt_len);
	
	buffer[0] = 0xAA;
	buffer[1] = 0x04;
	buffer[2] = wrt_len >> 8;
	buffer[3] = wrt_len & 0x00ff;
	buffer[4] = count;

	memcpy (buffer + 5,wrt_buf + wrt_buf_pos,data_len);
	crc = CRC_16C (buffer + 5, data_len, 0);
	buffer[5 + data_len] = crc >> 8;
	buffer[6 + data_len] = (crc & 0x00ff);
	nwrite = write(fd, buffer, wrt_len);
	if(nwrite == -1){
		printf("write error!\n");
	}
	wrt_buf_pos += data_len;
	file_size -= data_len;
	return file_size;	
}

void Cmd_05(void){
	buffer[0] = 0xAA;
	buffer[1] = 0x05;
	buffer[2] = 0x00;
	buffer[3] = 0x0B;
	buffer[4] = 0x00;
	buffer[5] = version_id[0];
	buffer[6] = version_id[1];
	buffer[7] = version_id[2];
	buffer[8] = version_id[3];

	crc = CRC_16C (buffer + 5, 4, 0);
	buffer[9] = crc >> 8;
	buffer[10] = (crc & 0x00ff);
	nwrite = write(fd, buffer , 11);
	if(nwrite == -1){
		printf("write error!\n");
	}

}
