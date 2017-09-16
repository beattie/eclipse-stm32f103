# Eclipse project for STM32F103 using libopencm3
This repository contains in it's branches Eclipse projects using libopencm3
targeted for a maple mini clone from BAITE, an STM32F103CB development board.
Some projects contain examples copied from libopencm3-examples.

### usart_irq_printf
This branch demos serial io using USART1 and interrupts using ring buffers.
Is also demos printf with an implementation of _write that copyies it's input
to a the ring buffer configured for output on USART1.

### newlib
https://sourceware.org/newlib/