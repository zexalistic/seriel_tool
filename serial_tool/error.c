#include "error.h"

void Error_Handler(char err){
	switch (err){
		case SAME_VERSION:			printf("The version id is same!\n");break;
		case ERASE_FLASH_ERROR: 		printf("Failed to erase flash\n");break;
		case CRC_ERROR:				printf("CRC parity error!\n");break;
		case WRITE_FLASH_ERROR:			printf("Write flash error!\n");break;
		case TRANS_ORDER_ERROR:		printf("Trans order error!\n");break;
		case NO_STD_FIRMWARE:			printf("No STD firmware! Update Failure! Restart MCU...\n");break;
		case WRONG_HEADER:			printf("Wrong header!\n");break;
		case SEND_ERROR:			printf("Send Error!\n");break;
		case BUFFER_OVERFLOW:		printf("Buffer overflows!\n");break;
		default:				printf("err=%x!\n",err);break;
	}

}
