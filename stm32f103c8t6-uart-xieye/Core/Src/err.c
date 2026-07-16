#include "err.h"

void Send_Error(uint8_t err){
	uint8_t frame[4];
	
	frame[0]=0xEE;
	frame[1]=0x04;
	frame[2]=err;
	frame[3]=frame[0]+frame[1]+frame[2];
	
	HAL_UART_Transmit(&huart1,frame,4,1000);
}


void Log_Print(char *msg){
	uint16_t len = strlen(msg);
	HAL_UART_Transmit(&huart1,(uint8_t *)msg,len,1000);
}