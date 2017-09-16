/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <stdio.h>
#include <errno.h>

#include "serialio.h"
#include "maplemini.h"

/******************************************************************************
 * The example implementation
 *****************************************************************************/

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
	/* and GPIO ports B and C for LEDS */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
}

static void gpio_setup(void)
{
	gpio_set(BLUE_LED_PORT, BLUE_LED_PIN);
	gpio_set(GREEN_LED_PORT, GREEN_LED_PIN);

	gpio_set_mode(BLUE_LED_PORT, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, BLUE_LED_PIN);
	gpio_set_mode(GREEN_LED_PORT, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GREEN_LED_PIN);
}
static void systick_setup(void)
{
	/* 72MHz / 8 => 9000000 counts per second. */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(8999);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}

void sys_tick_handler(void)
{
	static uint32_t temp32 = 0;

	temp32++;

	/*
	 * We call this handler every 1ms so we are sending hello world
	 * every 10ms / 100Hz.
	 */
	if (temp32 == 100) {
		gpio_toggle(GREEN_LED_PORT, GREEN_LED_PIN);

		temp32 = 0;
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	usart_setup();
	systick_setup();
	printf("Hello World!\r\n");

	while (1)
		__asm__("nop");

	return 0;
}
