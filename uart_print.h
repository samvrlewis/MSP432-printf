#ifndef UART_PRINT_H_
#define UART_PRINT_H_

typedef void (*uartSendByteCallback_t)(uint32_t, uint_fast8_t);

void init_console_print(uint32_t, uartSendByteCallback_t);

void printf_P(char *, ...);

#endif /* UART_PRINTF_H_ */
