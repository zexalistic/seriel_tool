#include "include.h"
#include "error.h"
#include "cmd.h"
#include "crc.h"
#include "serial.h"

/**************************************global*variables************************************/
	int fd, fp;
	unsigned char buffer[FILE_BUF_SIZE] = { 0xAA, 0x01, 0x00, 0x07, 0x00, 0x00, 0x00};
	unsigned char wrt_buf[FILE_BUF_SIZE] = {'\0'};
/**************************************main*function************************************/
int main(int argc, char *argv[]){
	char *interface =  NULL, *filename = NULL;
	int i,t = 0;	
	unsigned char rd_buf[BUFFER_SIZE] = {0,0,0,0,0,0,0,0};
	int  size_read = 0 ;	
	unsigned short ack = RESET;	//acknowledge
	unsigned short size_write, file_size;
	//unsigned short length,tick_count = 0;	//debug information

	if (argc < 2){
		interface = UART_DEVICE;
		filename = TRANS_FILE;
	}
	else{
		interface = argv[1];
		filename = argv[2];
	}

	//Open USB
	fd = open(interface, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1){
		printf("%s Open Error!\n", interface);
		return FALSE;
	}
	printf("Open %s successfully\n", interface);


	set_speed(fd,115200);
	if (set_Parity(fd, 8, 1, 'N') == FALSE){
		printf("Set Parity Error\n");
		exit(1);
	}
	tcflush(fd, TCIOFLUSH);
	Cmd_01();
//while(1){	//main loop
//Handshake first, check the version id
//wait for response
	while(1){
		//read
		size_read = read(fd, rd_buf, RESPONSE_SIZE);
		if(size_read > 0){
			if(rd_buf[0] == OK_01){
				ack = OK;
			}
			else 
				Error_Handler(rd_buf[0]);
			if (ack == OK){
				ack = RESET;
				if(size_read == 5){
					printf("Current version is %u.%u.%u.%u\n",rd_buf[1],rd_buf[2],rd_buf[3],rd_buf[4]);
					break;
				}	
				else 
					printf("Error in handshake!\n");
			}
	}
	}

	Cmd_02();
//After handshake, command 1, check the version id and judge if it's going to update
//wait for response
	while(1){
		//read
		size_read = read(fd, rd_buf, RESPONSE_SIZE);
		if(size_read > 0){
			if(rd_buf[0] == OK_02){
				ack = OK;
			}
			else if (rd_buf[0] == OK_01)
				continue;
			else 
				Error_Handler(rd_buf[0]);
			if (ack == OK){
				ack = RESET;
				printf("Start to update!\n");
				break;	
			}
		}
	}	

//open file
	fp = open(filename, O_RDWR);
	if (fp == -1){
		printf("open %s failure\n", filename);
		return FALSE;
	}	
        size_write = read(fp, wrt_buf, FILE_BUF_SIZE);

	Cmd_03(size_write);
	while(1){
		//read
		size_read = read(fd, rd_buf, RESPONSE_SIZE);
		if(size_read > 0){
			if(rd_buf[0] == OK_03){
				ack = OK;
			}
			else 
				Error_Handler(rd_buf[0]);
			if (ack == OK){
				printf("Finish transmitting the message of whole file...\n");
				ack = RESET;
				break;	
			}
	}
	}
//Command 3
        file_size = size_write;
	printf("file size = %d\n",file_size);
	file_size = Cmd_04(file_size);
	while(1){
		//read
		size_read = read(fd, rd_buf,RESPONSE_SIZE);
		if(size_read > 0){
		
			if(rd_buf[0] == OK_04){
				ack = OK;
			}
			else
				Error_Handler(rd_buf[0]);
			if (ack == OK){
				ack = RESET;
				printf("Updating...\n");
				printf("Current number is %d\n",rd_buf[1]);
				//length = ((unsigned short)rd_buf[2] << 8) + rd_buf[3];
				//printf("Reclength = %d\n",length);
				if (file_size == TRANS_SUCCESS)
					break;
				else
					file_size = Cmd_04(file_size);
			}

		}		
	}
	close (fp);
//Command 4

	Cmd_05();
	while(1){
		//read
		size_read = read(fd, rd_buf,  BUFFER_SIZE);
		if(size_read > 0){
			if(rd_buf[0] == OK_05){
				ack = OK;
			}
			else
				for (i = 0;i< size_read; i++)
					printf("%d = %d\n",i,rd_buf[i]); 
//				Error_Handler(rd_buf[0]);
			if (ack == OK){
				ack = RESET;
				printf("Current version is %u.%u.%u.%u\n",rd_buf[1],rd_buf[2],rd_buf[3],rd_buf[4]);
				printf("Update finished!\n");
				break;	
			}
		}
	}
//read result
	while(1){
		//read
		size_read = read(fd, rd_buf,  BUFFER_SIZE);
		if(size_read > 0){
			if(rd_buf[0] == OK){
				ack = OK;
			}
			else 
				Error_Handler(rd_buf[0]);
			if (ack == OK){
				ack = RESET;
				printf("WINNER WINNER, CHICKEN DINNER!\n");
				break;	
			}
		}
	}

//}  //end of main loop

	close (fd);
	return 0;

}



