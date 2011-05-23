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
#define EXT_OSC 1					// Set external clock source

#define CK 80000000L				// Define clock frequency
#define FCY CK/2					// Define instruction frequency

#define CIRC_BUF_ACC_LEN	64		// Define circular buffer lenght
#define CIRC_BUF_ACC_DIV	6		// log2(CIRC_BUF_ACC_LEN) for the shift op.

#define LED1 _LATA10				// Define pin A10 -> LED1
#define LED2 _LATA7					// Define pin A7 -> LED2

#define RAD2DEG		180/3,141592	// Constant conversion from Rad to Deg
#define KNOTS2KMH 	1,852			// Constant conversion from knots to km/h

//-----------------------------------------------------------------------------
//	Standard library
//-----------------------------------------------------------------------------
#include "include\p33fj128gp804.h"
#include <stdio.h>
#include <math.h>

//-----------------------------------------------------------------------------
// Typedef
//-----------------------------------------------------------------------------
// Defines the type 8-bit signed integer
typedef int __attribute__((__mode__(QI))) int8;

//-----------------------------------------------------------------------------
//	UAV library
//-----------------------------------------------------------------------------
#include "include\pointer.h"
#include "include\crc.h"
#include "include\delay.h"
#include "include\attitude_data.h"
#include "devices.h"
#include "nav.h"

//-----------------------------------------------------------------------------
//	Program start
//-----------------------------------------------------------------------------
int main()
{
	dev_init();			// Initializes hardware
	nav_init();			// initializes navigation

	// Print boot message
	uart_xbee_print("\n\rINITIALIZATION COMPLETED\r\n");
	
	LED1 = 0;
	LED2 = 0;

	while (1)
	{
		delay_ms(500);
		LED2 ^= 1;
	}
	
	return 0;
}
