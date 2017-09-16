/*
 * syscall.c
 *
 *  Created on: Sep 10, 2017
 *      Author: beattie
 */


#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <errno.h>

#include "serialio.h"

int _write(int file, char *ptr, int len)
{
	int		ret, i;

	if (file == 1) {
		for(i = 0; i < len; i++) {
			if((ret = putserial(file, (unsigned char)ptr[i])) == 0) {
				continue;
			}
			break;
		}

		USART_CR1(USART1) |= USART_CR1_TXEIE;

		return i;
	}
}
