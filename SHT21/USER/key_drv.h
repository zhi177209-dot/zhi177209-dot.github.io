#ifndef _KEY_DRV_H_
#define _KEY_DRV_H_

#include "fifo.h"
#include "config.h"
sbit INT0_PIN=P3^2;
sbit INT1_PIN=P3^3;

sbit K0_PIN = P5^2;
sbit K1_PIN = P5^3;

extern fifo_t key_fifo;

void single_key(void);
void key_read(void);
#endif