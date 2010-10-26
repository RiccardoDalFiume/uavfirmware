/*
 * main.c
 *
 *  Created on: 26/10/2009
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
//	Define
//-----------------------------------------------------------------------------
#define EXT_OSC 1					// Imposta il clock come esterno

#define CK 80000000L				// Definisce la frequenza di clock
#define FCY CK/2					// Definisce la frequenza di istruzione

#define CIRC_BUF_ACC_LEN	64		// Lunghezza del buffer circolare
#define CIRC_BUF_ACC_DIV	6		// log2(CIRC_BUF_ACC_LEN) per lo shift

#define LED1 _LATA10				// Definisce il pin A10 come LED1
#define LED2 _LATA7					// Definisce il pin A7 come LED2

#define RAD2DEG		180/3,141592	// K di conversione da radianti a gradi
#define KNOTS2KMH 	1,852			// K di conversione da nodi a km/h

//-----------------------------------------------------------------------------
//	Include delle librerie standard
//-----------------------------------------------------------------------------
#include "include\p33fj128gp804.h"
#include <stdio.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Typerdef
//-----------------------------------------------------------------------------
// Definisce il tipo intero con segno a 8 bit
typedef int __attribute__((__mode__(QI))) int8;

//-----------------------------------------------------------------------------
//	Include delle librerie UAV
//-----------------------------------------------------------------------------
#include "include\pointer.h"
#include "include\crc.h"
#include "include\delay.h"
#include "include\attitude_data.h"
#include "devices.h"
#include "nav.h"

//-----------------------------------------------------------------------------
//	Inizio programma
//-----------------------------------------------------------------------------
int main()
{
	dev_init();			// INIZIALIZZA HARDWARE
	nav_init();			// INIZIALIZZA NAVIGAZIONE

	// Stampa messaggio di avvio
	uart_xbee_print("\n\rINIZIALIZZAZIONE COMPLETATA\r\n");
	
	LED1 = 0;
	LED2 = 0;

	while (1)
	{
		delay_ms(500);
		LED2 ^= 1;
	}
	
	return 0;
}
