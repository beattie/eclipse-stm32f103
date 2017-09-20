/*
 * serialio.c
 *
 *  Created on: Sep 10, 2017
 *      Author: beattie
 */

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include <stdio.h>
#include <errno.h>

#include "serialio.h"
#include "maplemini.h"

#define SERIALECHO	1 << 0

#define SERIALSZ	1024

struct serialq {
	unsigned	in;
	unsigned	out;
	unsigned char	b[SERIALSZ];
};

struct serial {
	unsigned	flags;
	void		*interface;
	struct serialq	in;
	struct serialq	out;
} serial[] = {
		{ .flags = SERIALECHO, .interface = USART1, },
};

static int putq(struct serialq *q, unsigned char c)
{
	unsigned	p = (q->in + 1) % SERIALSZ;

	if(p == q->out) {
		return -1;
	}
	q->b[q->in] = c;
	q->in = p;
	return 0;
}

static int getq(struct serialq *q)
{
	unsigned char	c = q->b[q->out];

	if(q->in == q->out) {
		return -1;
	}
	q->out = (q->out + 1) % SERIALSZ;
	return c;
}

int	writeserial(int file, char *ptr, int len)
{

	int		ret, i;

	if (file == 1) {
		for(i = 0; i < len; i++) {
			if(ptr[i] == '\n') {
				putq(&serial[0].out, '\r');
			}
			if((ret = putq(&serial[0].out, (unsigned char)ptr[i])) == 0) {
				continue;
			}
			break;
		}

		USART_CR1(USART1) |= USART_CR1_TXEIE;

		return i;
	}
	return -1;
}

int readserial(int file, char *ptr, int len)
{
	return -1;
}

void usart_setup(void)
{
	/* Enable the USART1 interrupt. */
	nvic_enable_irq(NVIC_USART1_IRQ);

	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port B for receive. */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		      GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, SERIALSPEED);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

	/* Enable USART1 Receive interrupt. */
	USART_CR1(USART1) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(USART1);
	stdio_register(0, readserial, writeserial);
	stdio_register(1, readserial, writeserial);
}

void usart1_isr(void)
{
	struct serial	*intf = &serial[0];
	int				c;

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

		/* Indicate that we got data. */
		gpio_toggle(BLUE_LED_PORT, BLUE_LED_PIN);

		/* Retrieve the data from the peripheral. */
		putq(&intf->in, c = usart_recv(USART1));
		if(intf->flags &SERIALECHO) {
			putq(&intf->out, c);	/* put in output queue to echo */
		}

		/* Enable transmit interrupt so it sends back the data. */
		USART_CR1(USART1) |= USART_CR1_TXEIE;
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(USART1) & USART_SR_TXE) != 0)) {

		c = getq(&intf->out);

		if (c == -1) {
			/* Disable the TXE interrupt, it's no longer needed. */
			USART_CR1(USART1) &= ~USART_CR1_TXEIE;
		} else {
			/* Put data into the transmit register. */
			usart_send(USART1, c);
		}
	}
}
