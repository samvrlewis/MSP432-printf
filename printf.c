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
#include "driverlib.h"

void sendByte(uint32_t moduleInstance, char c)
{
	MAP_UART_transmitData(moduleInstance, c);
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

void puts(uint32_t moduleInstance, char *s) {
	char c;

	while (c = *s++) {
		sendByte(moduleInstance, c);
	}
}

void putc(uint32_t moduleInstance, unsigned b) {
	sendByte(moduleInstance, b);
}

static void xtoa(uint32_t moduleInstance, unsigned long x, const unsigned long *dp) {
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
			putc(moduleInstance, c);
		} while (!(d & 1));
	} else
		putc(moduleInstance, '0');
}

static void puth(uint32_t moduleInstance, unsigned n) {
	static const char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
			'9', 'A', 'B', 'C', 'D', 'E', 'F' };
	putc(moduleInstance, hex[n & 15]);
}

void printf(uint32_t moduleInstance, char *format, ...)
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
					puts(moduleInstance, va_arg(a, char*));
					break;
				case 'c':// Char
					putc(moduleInstance, va_arg(a, char));
				break;
				case 'i':// 16 bit Integer
				case 'u':// 16 bit Unsigned
					i = va_arg(a, int);
					if(c == 'i' && i < 0) i = -i, putc(moduleInstance, '-');
					xtoa(moduleInstance, (unsigned)i, dv + 5);
				break;
				case 'l':// 32 bit Long
				case 'n':// 32 bit uNsigned loNg
					n = va_arg(a, long);
					if(c == 'l' && n < 0) n = -n, putc(moduleInstance, '-');
					xtoa(moduleInstance, (unsigned long)n, dv);
				break;
				case 'x':// 16 bit heXadecimal
					i = va_arg(a, int);
					puth(moduleInstance, i >> 12);
					puth(moduleInstance, i >> 8);
					puth(moduleInstance, i >> 4);
					puth(moduleInstance, i);
				break;
				case 0: return;
				default: goto bad_fmt;
			}
		} else
			bad_fmt: putc(moduleInstance, c);
	}
	va_end(a);
}
