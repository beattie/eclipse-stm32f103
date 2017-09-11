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
	int ret;

	if (file == 1) {
		ret = ring_write(&output_ring, (uint8_t *)ptr, len);

		if (ret < 0)
			ret = -ret;

		USART_CR1(USART1) |= USART_CR1_TXEIE;

		return ret;
	}

	errno = EIO;
	return -1;
}
