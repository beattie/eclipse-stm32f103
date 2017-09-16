/*
 * serialio.h
 *
 *  Created on: Sep 10, 2017
 *      Author: beattie
 */

#ifndef _SERIALIO_H_
#define _SERIALIO_H_

#define SERIALSPEED	115200

int	putserial(int file, unsigned char c);
int getserial(int file);
void usart_setup(void);

#endif /* _SERIALIO_H_ */
