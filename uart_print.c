/*
 * printf library for the MSP432
 *
 * Largely taken from and inspired from:
 * 	http://www.msp430launchpad.com/2012/06/using-printf.html
 *	http://www.43oh.com/forum/viewtopic.php?f=10&t=1732
 * 
 * See http://www.samlewis.me for an example implementation.
 */

#include "stdarg.h"
#include <stdint.h>
#include <uart_print.h>

static uartSendByteCallback_t uart_putc;
static uint32_t uart_module_instance;

void init_console_print(uint32_t uartInstance, uartSendByteCallback_t uartSendByteCallback) {
    uart_putc = uartSendByteCallback;
    uart_module_instance = uartInstance;
}

void sendByte(char c) {
    uart_putc(uart_module_instance, c);
}

static const unsigned long dv[] = {
		1000000000,// +0
		100000000, // +1
		10000000, // +2
		1000000, // +3
		100000, // +4
		10000, // +5
		1000, // +6
		100, // +7
		10, // +8
		1, // +9
		};

void puts_P(char *s) {
	char c;

	while (c = *s++) {
		sendByte(c);
	}
}

void putc_P(unsigned b) {
	sendByte(b);
}

static void xtoa(unsigned long x, const unsigned long *dp) {
	char c;
	unsigned long d;
	if (x) {
		while (x < *dp)
			++dp;
		do {
			d = *dp++;
			c = '0';
			while (x >= d)
				++c, x -= d;
			putc_P(c);
		} while (!(d & 1));
	} else
		putc_P('0');
}

static void puth(unsigned n) {
	static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'A', 'B', 'C', 'D', 'E', 'F' };
	putc_P(hex[n & 15]);
}

void printf_P(char *format, ...)
{
	char c;
	int i;
	long n;

	va_list a;
	va_start(a, format);
	while(c = *format++) {
		if(c == '%') {
			switch(c = *format++) {
				case 's': // String
					puts_P(va_arg(a, char*));
					break;
				case 'c':// Char
					putc_P(va_arg(a, char));
				break;
				case 'd':// 16 bit Integer
				case 'i':// 16 bit Integer
				case 'u':// 16 bit Unsigned
					i = va_arg(a, int);
					if((c == 'i' || c == 'd') && (i < 0)) i = -i, putc_P('-');
					xtoa((unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					n = va_arg(a, long);
					if(c == 'l' && n < 0) n = -n, putc_P('-');
					xtoa((unsigned long)n, dv);
				break;
				case 'x':// 16 bit heXadecimal
					i = va_arg(a, int);
					puth(i >> 12);
					puth(i >> 8);
					puth(i >> 4);
					puth(i);
				break;
				case 0: return;
				default: goto bad_fmt;
			}
		} else
			bad_fmt: putc_P(c);
	}
	va_end(a);
}
