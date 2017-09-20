# Eclipse project for STM32F103 using libopencm3
This repository contains in it's branches Eclipse projects using libopencm3
targeted for a maple mini clone from BAITE, an STM32F103CB development board.
Some projects contain examples copied from libopencm3-examples.

### cdcacm
This branch builds on usart_irq_printf. It demos the USB cdc_acm(USB serial
adapter). It connects the cdc_acm to tty1 to make a serial adapter.

It also demos implementations of \_read and \_write and _open.

This branch provides a basic demo of cdcacm and syscall IO.

### newlib
https://sourceware.org/newlib/