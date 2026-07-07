#include "key_task.h"
#include "uart.h"
#include "key_drv.h"

void key_handle(void){
	uint8_t key;
	static uint16_t time=0;
	if(++time>=10){
		if(!fifo_is_empty(&key_fifo)){
			fifo_dequeue(&key_fifo,&key);
			printf("Key %u Press\r\n",(uint16_t)key);
			switch(key){
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
					break;
				case 10:break;
				case 11:break;
				case 12:break;
				case 13:break;
				case 14:break;
				case 15:break;
				case 16:break;
				case 17:break;
				default:break;
			}
		}
	}
}

uint8_t code correct_pwd[4] = {1,3,5,7};
uint8_t user_input[4] = {0};
uint8_t input_len = 0;

void pwd_lock_task(void){
	uint8_t key_val;
	if(!fifo_is_empty(&key_fifo)){
		fifo_dequeue(&key_fifo,&key_val);
		if((key_val >=0 && key_val<=9)&& input_len<4){
			user_input[input_len++]=key_val;
		}else if(key_val == 17){
			uint8_t i,flag=1;
			for(i=0;i<4;i++){
				if(user_input[i] !=correct_pwd[i]){
					flag=0;
					break;
				}
			}
			if(flag) printf("right!\r\n");
			else printf("Error\r\n");
			
			input_len = 0;
			for(i=0;i<4;i++) user_input[i]=0;
		}
	}
}