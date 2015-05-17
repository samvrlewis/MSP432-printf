/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "printf.h"

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 9600 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 *http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
const eUSCI_UART_Config uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};

void sendString(char* string)
{
	char toSend = *string;

	while (toSend != 0x00)
	{
		MAP_UART_transmitData(EUSCI_A0_MODULE, toSend);

		*string++;

		toSend = *string;
	}
}


int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    /* Selecting P1.2 and P1.3 in UART mode */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting DCO to 12MHz */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_MODULE, &uartConfig);

    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_MODULE);

    MAP_Interrupt_enableMaster();   

    /* Initialize values to display */
    char *s = "printf test";
	char c = '!';
	int i = -12345;
	unsigned u = 4321;
	long int l = -123456780;
	long unsigned n = 1098765432;
	unsigned x = 0xABCD;

    while(1)
    {
		printf(EUSCI_A0_MODULE, "String         %s\r\n", s);
		printf(EUSCI_A0_MODULE, "Char           %c\r\n", c);
		printf(EUSCI_A0_MODULE, "Integer        %i\r\n", i);
		printf(EUSCI_A0_MODULE, "Unsigned       %u\r\n", u);
		printf(EUSCI_A0_MODULE, "Long           %l\r\n", l);
		printf(EUSCI_A0_MODULE, "uNsigned loNg  %n\r\n", n);
		printf(EUSCI_A0_MODULE, "heX            %x\r\n", x);
    }
}
