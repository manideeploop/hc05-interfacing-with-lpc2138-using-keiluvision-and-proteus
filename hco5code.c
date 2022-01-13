#include <lpc214x.h>
#include <stdint.h>

void delay_ms(uint16_t j)
{
    uint16_t x,i;
	for(i=0;i<j;i++)
	{
    for(x=0; x<6000; x++);    /* loop to generate 1 milisecond delay with Cclk = 60MHz */
	}
}

void UART0_init(void)
{
	PINSEL0 = PINSEL0 | 0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;	/* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0x00;	/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 0x61;	/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; /* DLAB = 0 */
}

void UART0_TxChar(char ch) /* A function to send a byte on UART0 */
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	/* Wait till THRE bit becomes 1 which tells that transmission is completed */
}

void UART0_SendString(char* str) /* A function to send string on UART0 */
{
	uint8_t i = 0;
	while( str[i] != '\0' )
	{
		UART0_TxChar(str[i]);
		i++;
	}
}

unsigned char UART0_RxChar(void) /* A function to receive a byte on UART0 */
{
	while( (U0LSR & 0x01) == 0);	/* Wait till RDR bit becomes 1 which tells that receiver contains valid data */
		return U0RBR;
}

int main (void)
{
	char data_in;
	UART0_init();
	IO0DIR = IO0DIR | 0x00000010;	/* Configure P0.4 as output */
	while(1)
	{
		data_in = UART0_RxChar();	/* Receive data from bluetooth */
		if ( data_in == '1' )	/* If data received is 1 */
		{
			IO0PIN = ( IO0PIN | 0x00000010 );
			UART0_SendString("FAN ON\n");
		}
		else if ( data_in == '0' )	/* If data received is 0 */
		{
			IO0PIN = ( IO0PIN & 0xFFFFFFEF );
			UART0_SendString("FAN OFF\n");
		}
		else
			UART0_SendString("Select Proper Option\n");
	}
}
