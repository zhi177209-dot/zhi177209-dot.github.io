#include "key_drv.h"
#include "fifo.h"
void matrix_key(void);
code uint8_t key_code[]={1,2,3,11,4,5,6,12,7,8,9,13,10,0,15,14};

fifo_t key_fifo={0};

void single_key(void){
	static bit pre_state0=1,cur_state0=1;
	static bit pre_state1=1,cur_state1=1;
	
	pre_state0=cur_state0;
	cur_state0=INT0_PIN;
	
	if(!cur_state0 && pre_state0){fifo_enqueue(&key_fifo,17);}
	
	pre_state1=cur_state1;
	cur_state1=INT1_PIN;
	
	if(!cur_state1 && pre_state1){fifo_enqueue(&key_fifo,16);}
}

void key_read(void){
	static uint8_t time = 0;
	if(++time>=10){
		time = 0;
		single_key();
	}
	matrix_key();
	
}

void matrix_key(void){
	static uint16_t pre_state=0xffff,cur_state=0xffff;
	uint8_t i;
	pre_state=cur_state;
	
	K0_PIN=0;K1_PIN=1;
	cur_state=P7;
	cur_state<<=8;
	K0_PIN=1;K1_PIN=0;
	cur_state+=P7;
	if(pre_state>cur_state){
		for(i=0;i<16;i++){
			if((cur_state & (0x8000>>i))==0)
				break;
		}
		
		fifo_enqueue(&key_fifo,key_code[i]);
	}
}