/*
 * uart_xbee.h
 *
 *  Created on: 10/02/2010
 *      Author: Berti Giulio, Dal Fiume Riccardo
 */

//
//	 This file is part of UAVFirmware.
//
//    UAVFirmware is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 2 of the License, or
//    (at your option) any later version.
//
//    UAVFirmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with UAVFirmware.  If not, see <http://www.gnu.org/licenses/>.
//


//-----------------------------------------------------------------------------
//	Include header
//-----------------------------------------------------------------------------
#include "uart_xbee_term.h"

//-----------------------------------------------------------------------------
//	Prototipi
//-----------------------------------------------------------------------------
void uart_xbee_print(char *src);

//-----------------------------------------------------------------------------
//	Define vari
//-----------------------------------------------------------------------------
#define XB_BAUDRATE 57600
#define XB_BRGVAL ((FCY/XB_BAUDRATE)/4)-1

#define U1_RX_BUF_LENGHT 100
#define U1_TX_BUF_LENGHT 200

//-----------------------------------------------------------------------------
//	Variabili globali
//-----------------------------------------------------------------------------
char xbee_str_buffer[3][U1_RX_BUF_LENGHT];
unsigned int cuncurrency_xbee = 0;

//-----------------------------------------------------------------------------
//  STEP 6:
//  Allocate two buffers for DMA transfers
//-----------------------------------------------------------------------------
char BufferA_U1[U1_RX_BUF_LENGHT] __attribute__((space(dma)));
char BufferB_U1[U1_RX_BUF_LENGHT] __attribute__((space(dma)));
char stringTX_U1[U1_TX_BUF_LENGHT] __attribute__((space(dma)));

//-----------------------------------------------------------------------------
// Configurazione UART Xbee
//-----------------------------------------------------------------------------
inline void dev_uart_xbee_init()
{
	//--------------------------------------------------------------------------------
	//  STEP 3:
	//  Configura il canale 0 DMA, per la trasmissione da UART
	//--------------------------------------------------------------------------------
	DMA0REQ = 0b0001100;					// Select UART1 Transmitter
	DMA0PAD = (volatile unsigned int) &U1TXREG;

	//--------------------------------------------------------------------------------
	//  STEP 5:
	//  Configure DMA Channel 0 to:
	//  Transfer data from RAM to UART
	//  One-Shot mode
	//  Register Indirect with Post-Increment
	//  Using single buffer
	//  100 transfers per buffer
	//  Transfer words
	//--------------------------------------------------------------------------------
	DMA0CONbits.AMODE = 0b00;
	DMA0CONbits.MODE  = 0b01;
	DMA0CONbits.DIR   = 1;
	DMA0CONbits.SIZE  = 1;
	DMA0CNT = U1_TX_BUF_LENGHT - 1;			// DMA requests

	//--------------------------------------------------------------------------------
	// STEP 6:
	// Associate one buffer with Channel 0 for one-shot operation
	//--------------------------------------------------------------------------------
	DMA0STA = __builtin_dmaoffset(stringTX_U1);

	//--------------------------------------------------------------------------------
	//  STEP 8:
	//	Enable DMA Interrupts
	//--------------------------------------------------------------------------------
	IFS0bits.DMA0IF  = 0;					// Clear DMA Interrupt Flag
	_DMA0IP = 3;
	IEC0bits.DMA0IE  = 1;					// Enable DMA interrupt

	U1MODEbits.STSEL 	= 0;				// 1-stop bit
	U1MODEbits.PDSEL	= 0b00;				// No Parity, 8-data bits
	U1MODEbits.UEN		= 0b00;				// UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by port latches
	U1MODEbits.IREN		= 0;				// IrDA® encoder and decoder disabled
	U1MODEbits.ABAUD	= 0;				// Auto-Baud Disabled
	U1MODEbits.BRGH		= 1;				// Speed mode, 0 Low, 1 High
	U1MODEbits.LPBACK	= 0;				// LoopBack mode select
	U1BRG				= XB_BRGVAL;		// BAUD Rate Setting

	U1STAbits.URXISEL	= 0;				// Interrupt after one RX character is received;
	U1STAbits.UTXISEL0	= 0;				// Interrupt after one TX character is transmitted;
	U1STAbits.UTXISEL1	= 0;
//	IEC0bits.U1TXIE		= 0;				// Enable UART Tx interrupt

	U1MODEbits.URXINV 	= 0;				// Inversione livelli logici in ricezione
	U1STAbits.UTXINV 	= 0;				// Inversione livelli logici in trasmissione

	U1MODEbits.UARTEN	= 1;				// Enable UART
	U1STAbits.UTXEN		= 1;				// Enable UART Tx

	_U1RXIF 	= 0;						// Pulisco il flag di interrupt
	_U1RXIP 	= 6;						// Interrupt priority
	_U1RXIE		= 1;

//	_U1EIE = 0;								// UART1 Error Interrupt

	// Scrittura inizializzazione
	uart_xbee_print("\n\n\rdsPIC BOOTSTRAP STARTED\r\n");
	uart_xbee_print("UART1\tInitializing completed\n\r");
}

//-----------------------------------------------------------------------------
// Interrupt fine trasmissione UART1 con DMA
//-----------------------------------------------------------------------------	
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
	cuncurrency_xbee = 0;
	IFS0bits.DMA0IF = 0;							// Clear the DMA0 Interrupt Flag;
}

/*
//-----------------------------------------------------------------------------
// Interrupt DMA buffer UART1 pieno 
//-----------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
	static unsigned int BufferCount = 0;			// Keep record of which buffer contains Rx Data
	BufferCount ^= 1;								// Change current buffer
	IFS1bits.DMA1IF = 0;							// Clear the DMA1 Interrupt Flag
}
*/

//-----------------------------------------------------------------------------
// Scrittura su UART1 con DMA
//-----------------------------------------------------------------------------
void uart_xbee_print(char *src)
{			
	while(cuncurrency_xbee);
	cuncurrency_xbee = 1;

	delay_us(300);
	
	int len = strlen(src);
	strcpy(stringTX_U1, src);
	
	DMA0CNT = len-1;
	DMA0STA = __builtin_dmaoffset(stringTX_U1);
	
	DMA0CONbits.CHEN  = 1;							// Re-enable DMA0 Channel
	DMA0REQbits.FORCE = 1;							// Manual mode: Kick-start the first transfer
}

//-----------------------------------------------------------------------------
// Interrupt ricezione carattere UART1
//-----------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
	char c;
	static unsigned int current_str = 0;
	static unsigned int str_num = 0;
	static unsigned int chr_num = 0;

	while(U1STAbits.URXDA)			// Ci sono caratteri nel buffer di ricezione?
	{
		c = U1RXREG;

		if(c == '$')
		{
			current_str = 1;
			chr_num = 0;
		}
		else						// se non è un dollaro
		{
			if(current_str)
			{
				chr_num++;
				switch(chr_num)
				{
					case 1:
						if(c !=	'G') current_str = 0;
						break;

					case 2:
						if(c != 'S') current_str = 0;
						break;

					case 3:
						if( c == 'N')
						{
							str_num = 0;		// NAV
							strncpy(xbee_str_buffer[0], "$GSNAV" , 6 );
						}
						if( c == 'P')
						{
							str_num = 1;		// PLT
							strncpy(xbee_str_buffer[1], "$GSPLT" , 6 );
						}
						if( c == 'A')
						{
							str_num = 2;		// ALM
							strncpy(xbee_str_buffer[2], "$GSALM" , 6 );
						}
						break;

					default:
						if( c != '\n' )
						{
							xbee_str_buffer[str_num][chr_num] = c;
						}
						else
						{
							xbee_str_buffer[str_num][chr_num] = c;
							xbee_str_buffer[str_num][chr_num+1] = '\0';

							if(str_num == 0)
								xbee_parser(xbee_str_buffer[0]);
							if(str_num == 1)
								xbee_parser(xbee_str_buffer[1]);
							if(str_num == 2)
								xbee_parser(xbee_str_buffer[2]);
						}
						break;
				}
			}
		}
	}
	_U1RXIF = 0;
}
