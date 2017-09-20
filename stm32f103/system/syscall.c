/*
 * syscall.c
 *
 *  Created on: Sep 10, 2017
 *      Author: beattie
 *
 * syscall implementations to support stdio
 */


#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <string.h>
#include <errno.h>

#include "serialio.h"
#include "syscall.h"

#define NSTDIO	4

static int (*driver_read[NSTDIO])(int file, char *ptr, int len);
static int (*driver_write[NSTDIO])(int file, char *ptr, int len);

int stdio_register(int index,
		int (*readp)(int file, char *ptr, int len),
		int (*writep)(int file, char *ptr, int len))
{
	if(index >= NSTDIO) {
		return -1;
	}
	driver_read[index] = readp;
	driver_write[index] = writep;
	return 0;
}

int _open(const char *pathname, int flags)
{
	if(strncmp(pathname, "acm", 3) == 0) {
		return 3;
	}
	return -1;
}

int _write(int file, char *ptr, int len)
{
	if(file < NSTDIO) {
		if(driver_write[file]) {
			return driver_write[file](file, ptr, len);
		}
	}
	return -1;
}

int _read(int file, char *ptr, int len)
{
	if(file < NSTDIO) {
		if(driver_read[file]) {
			return driver_read[file](file, ptr, len);
		}
	}
	return -1;
}
