/*
 * syscall.h
 *
 *  Created on: Sep 17, 2017
 *      Author: beattie
 */

#ifndef _SYSCALL_H_
#define _SYSCALL_H_

int stdio_register(int index,
		int (*readp)(int, char *, int),
		int (*writep)(int, char *, int));

#endif /* _SYSCALL_H_ */
