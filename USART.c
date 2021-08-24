/*
 * USART.c
 *USART driver but not support interrupt send and recieve only by polling
 *  Created on: Jan 22, 2021
 *      Author: seif adel
 */
#include "USART.h"

static volatile void (*u_callBackPtr)(void) = NULL_PTR;
ISR(USART_RXC_vect)
{
	//recieved_data = UDR;
}

ISR(USART_TXC_vect)
{

}

void UART_init(const uart_config_type * config_ptr)
{
	if(config_ptr ->mode == SYNCH)
	{
		#define BAUD_PRESCALE (((F_CPU / ((config_ptr->USART_BAUD) * 2UL))) - 1)
		/************************** UCSRC Description **************************/

		/* by default 1 stop bit and no parity check */
		UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1)| (1<<UMSEL);
		if(config_ptr ->edge_transmit == FALLING)
		{
			SET_BIT(UCSRC,UCPOL);
		}

		/************************** UCSRB Description **************************/

		if(config_ptr ->WayOfWork == INTERRUPT)
		{
			UCSRB = (1<<RXCIE) | (1<<TXCIE) | (1<<UDRIE);
		}
		else if(config_ptr ->WayOfWork == POLLING)
		{
			UCSRB = (1<<RXEN) | (1<<TXEN);
		}
		if(config_ptr ->NumberOfData == bit_9)
		{
			SET_BIT(UCSRB,UCSZ2);
		}

		/************************** UCSRA Description **************************/
		/*by default frame error and parity error and data overrun error and MPCM is disable */
		/* no configuration UCSRA in case of sync. */
		/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
			UBRRH = BAUD_PRESCALE>>8;
			UBRRL = (uint8)(BAUD_PRESCALE);

	}

	else if(config_ptr ->mode == ASYNCH)
	{
		#define BAUD_PRESCALE (((F_CPU / ((config_ptr ->USART_BAUD) * 16UL))) - 1)
		/************************** UCSRC Description **************************/

			/* by default 1 stop bit and no parity check */
			UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

			/************************** UCSRB Description **************************/
			if(config_ptr ->WayOfWork == INTERRUPT)
			{
				UCSRB = (1<<RXCIE) | (1<<TXCIE) | (1<<UDRIE);
			}
			else if(config_ptr ->WayOfWork == POLLING)
			{
				UCSRB = (1<<RXEN) | (1<<TXEN);
			}
			if(config_ptr ->NumberOfData == bit_9)
			{
				SET_BIT(UCSRB,UCSZ2);
			}
			/************************** UCSRA Description **************************/
			/*by default frame error and parity error and data overrun error and MPCM is disable */
			/* no configuration UCSRA in case of async. */
			/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
				UBRRH = BAUD_PRESCALE>>8;
				UBRRL = (uint8)BAUD_PRESCALE;
	}
	else if(config_ptr ->mode == ASYNCH_DOUBLE)
	{
		#define BAUD_PRESCALE (((F_CPU / ((config_ptr -> USART_BAUD) * 8UL))) - 1)
		/************************** UCSRC Description **************************/
				/* by default 1 stop bit and no parity check */
				UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);

				/************************** UCSRB Description **************************/
				if(config_ptr ->WayOfWork == INTERRUPT)
				{
					UCSRB = (1<<RXCIE) | (1<<TXCIE) | (1<<UDRIE) |(1<<TXEN) |(1<<RXEN);
				}
				else if(config_ptr ->WayOfWork == POLLING)
				{
					UCSRB = (1<<RXEN) | (1<<TXEN);
				}
				if(config_ptr ->NumberOfData == bit_9)
				{
					SET_BIT(UCSRB,UCSZ2);
				}
				/************************** UCSRA Description **************************/
				/*by default frame error and parity error and data overrun error and MPCM is disable */
				UCSRA = (1<<U2X);
				/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
				UBRRH = BAUD_PRESCALE>>8;
				UBRRL = (uint8)BAUD_PRESCALE;
	}
}

void UART_sendByte(const uint8 data ,uart_way_of_work x)
{
	if(x==POLLING)
	{
	// the logic is to send and recieve the byte by polling so we will not use interrupt
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	}
	else if(x==INTERRUPT)
	{
		while(BIT_IS_CLEAR(UCSRA,UDRE)){}
		UDR = data;
	}
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}

void UART_sendString(const uint8 *Str ,uart_way_of_work x)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i] ,x);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}

void UART_setCallBack(void(*app_ptr)(void)) // call this fn->put address of app function to be executed when interrupt occur in u_callbackptr then call this fn.in ISR
{
	 //Save the address of the Call back function in a global variable
	u_callBackPtr = app_ptr;
}
