/*
 * uart_gps.h
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
#include "uart_gps_nmea.h"

//-----------------------------------------------------------------------------
//	Define vari
//-----------------------------------------------------------------------------
#define GPS_BAUDRATE 57600
#define GPS_BRGVAL ((FCY/GPS_BAUDRATE)/4)-1

#define U2_RX_BUF_LENGHT 100
#define U2_TX_BUF_LENGHT 300

//-----------------------------------------------------------------------------
//	Variabili globali
//-----------------------------------------------------------------------------
char nmea_str_buffer[3][U2_RX_BUF_LENGHT];
unsigned int cuncurrency_gps = 0;

//--------------------------------------------------------------------------------
//  STEP 6:
//  Allocate two buffers for DMA transfers
//--------------------------------------------------------------------------------
char BufferA_U2[U2_RX_BUF_LENGHT] __attribute__((space(dma)));
char BufferB_U2[U2_RX_BUF_LENGHT] __attribute__((space(dma)));
char stringTX_U2[U2_TX_BUF_LENGHT] __attribute__((space(dma)));

//-----------------------------------------------------------------------------
// Configurazione UART GPS
//-----------------------------------------------------------------------------
inline void dev_uart_gps_init()
{
	uart_xbee_print("UART2\tInitializing...\t");

	//--------------------------------------------------------------------------------
	//  STEP 3:
	//  Configura il canale 2 DMA, per la trasmissione da UART
	//--------------------------------------------------------------------------------
	DMA2REQ = 0b0011111;					// Select UART2 Transmitter
	DMA2PAD = (volatile unsigned int) &U2TXREG;

	//--------------------------------------------------------------------------------
	//  STEP 5:
	//  Configure DMA Channel 2 to:
	//  Transfer data from RAM to UART
	//  One-Shot mode
	//  Register Indirect with Post-Increment
	//  Using single buffer
	//  100 transfers per buffer
	//  Transfer words
	//--------------------------------------------------------------------------------
	DMA2CONbits.AMODE = 0b00;
	DMA2CONbits.MODE  = 0b01;
	DMA2CONbits.DIR   = 1;
	DMA2CONbits.SIZE  = 1;
	DMA2CNT = U2_RX_BUF_LENGHT - 1;		// 8 DMA requests

	//--------------------------------------------------------------------------------
	//  STEP 6:
	// Associate one buffer with Channel 2 for one-shot operation
	//--------------------------------------------------------------------------------
	DMA2STA = __builtin_dmaoffset(BufferA_U2);

	//--------------------------------------------------------------------------------
	//  STEP 8:
	//	Abilita gli interrupt DMA
	//--------------------------------------------------------------------------------
	IFS1bits.DMA2IF  = 0;			// Clear DMA Interrupt Flag
	_DMA2IP = 3;
	IEC1bits.DMA2IE  = 1;			// Enable DMA interrupt
/*
	//--------------------------------------------------------------------------------
	//  STEP 3:
	//  Configura il canale 3 DMA, per la ricezione da UART
	//--------------------------------------------------------------------------------
	DMA3REQ = 0b0011110;					// Select UART2 Receiver
	DMA3PAD = (volatile unsigned int) &U2RXREG;

	//--------------------------------------------------------------------------------
	//  STEP 4:
	//  Configure DMA Channel 3 to:
	//  Transfer data from UART to RAM Continuously
	//  Register Indirect with Post-Increment
	//  Using two ping-pong buffers
	//  8 transfers per buffer
	//  Transfer words
	//--------------------------------------------------------------------------------
	DMA3CONbits.AMODE = 0;
	DMA3CONbits.MODE  = 2;
	DMA3CONbits.DIR   = 0;
	DMA3CONbits.SIZE  = 1;
	DMA3CNT = U2_RX_BUF_LENGHT - 1;		// 8 DMA requests

	//--------------------------------------------------------------------------------
	//  STEP 6:
	//  Associate two buffers with Channel 3 for Ping-Pong operation
	//--------------------------------------------------------------------------------
	DMA3STA = __builtin_dmaoffset(BufferA_U2);
	DMA3STB = __builtin_dmaoffset(BufferB_U2);

	//--------------------------------------------------------------------------------
	//  STEP 8:
	//	Enable DMA Interrupts
	//--------------------------------------------------------------------------------
	IFS2bits.DMA3IF  = 0;			// Clear DMA interrupt
	_DMA3IP = 2;
	IEC2bits.DMA3IE  = 1;			// Enable DMA interrupt

	//--------------------------------------------------------------------------------
	//  STEP 9:
	//  Enable DMA Channel 3 to receive UART data
	//--------------------------------------------------------------------------------
	DMA3CONbits.CHEN = 1;					// Enable DMA Channel
*/

	U2MODEbits.STSEL 	= 0;				// 1-stop bit
	U2MODEbits.PDSEL	= 0b00;				// No Parity, 8-data bits
	U2MODEbits.UEN		= 0b00;				// UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLK pins controlled by port latches
	U2MODEbits.IREN		= 0;				// IrDA® encoder and decoder disabled
	U2MODEbits.ABAUD	= 0;				// Auto-Baud Disabled
	U2MODEbits.BRGH		= 1;				// Speed mode, 0 Low, 1 High
	U2MODEbits.LPBACK	= 0;				// LoopBack mode select
	U2BRG				= GPS_BRGVAL;		// BAUD Rate Setting

	U2STAbits.URXISEL	= 0;				// Interrupt after one RX character is received;
	U2STAbits.UTXISEL0	= 0;				// Interrupt after one TX character is transmitted;
	U2STAbits.UTXISEL1	= 0;
//	IEC0bits.U2TXIE		= 0;				// Enable UART Tx interrupt

	U2MODEbits.URXINV 	= 0;				// Inversione livelli logici in ricezione
	U2STAbits.UTXINV 	= 0;				// Inversione livelli logici in trasmissione

	U2MODEbits.UARTEN	= 1;				// Enable UART
	U2STAbits.UTXEN		= 1;				// Enable UART Tx

	_U2RXIF 	= 0;						// Pulisco il flag di interrupt
	_U2RXIP 	= 6;						// Interrupt priority
	_U2RXIE		= 0;						// Abilito gli interrupt in ricezione

//	_U2EIE 		= 1;						// UART2 Error Interrupt

	uart_xbee_print("UART2\tInitializing completed\r\n");
}

//-----------------------------------------------------------------------------
// Interrupt fine trasmissione UART2 con DMA
//-----------------------------------------------------------------------------	
void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
	cuncurrency_gps = 0;
	IFS1bits.DMA2IF = 0;							// Clear the DMA2 Interrupt Flag;
}

/*
//-----------------------------------------------------------------------------
// Interrupt DMA buffer UART2 pieno 
//-----------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
	
}
*/

//-----------------------------------------------------------------------------
// Scrittura su UART2 con DMA
//-----------------------------------------------------------------------------
void uart_gps_print(char *src)
{			
	while(cuncurrency_gps);
	cuncurrency_gps = 1;

	delay_us(300);
	
	int len = strlen(src);
	strcpy(stringTX_U2, src);
	
	DMA2CNT = len-1;
	DMA2STA = __builtin_dmaoffset(stringTX_U2);
	
	DMA2CONbits.CHEN  = 1;			// Re-enable DMA2 Channel
	DMA2REQbits.FORCE = 1;			// Manual mode: Kick-start the first transfer
}	

//-----------------------------------------------------------------------------
// Interrupt ricezione carattere UART2
//-----------------------------------------------------------------------------
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{
	char c;
	static unsigned int current_str = 0;
	static unsigned int str_num = 0;
	static unsigned int chr_num = 0;

	while(U2STAbits.URXDA)			// Ci sono caratteri nel buffer di ricezione?
	{
		c = U2RXREG;

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
						if(c != 'P') current_str = 0;
						break;

					case 3:
						if( c == 'G')
						{
							str_num = 0;		// GGA
							strncpy(nmea_str_buffer[0], "$GPGGA" , 6 );
						}
						if( c == 'R')
						{
							str_num = 1;		// RMC
							strncpy(nmea_str_buffer[1], "$GPRMC" , 6 );
						}
						if( c == 'Z')
						{
							str_num = 2;		// ZDA
							strncpy(nmea_str_buffer[2], "$GPZDA" , 6 );
						}
						break;

					default:
						if( c != '\n' )
						{
							nmea_str_buffer[str_num][chr_num] = c;
						}
						else
						{
							nmea_str_buffer[str_num][chr_num] = c;
							nmea_str_buffer[str_num][chr_num+1] = '\0';

							if(str_num == 2)
							{
								nmea_parser(nmea_str_buffer[0]);
								nmea_parser(nmea_str_buffer[1]);
								nmea_parser(nmea_str_buffer[2]);
							}
						}
						break;
				}
			}
		}
	}
	_U2RXIF = 0;
}

