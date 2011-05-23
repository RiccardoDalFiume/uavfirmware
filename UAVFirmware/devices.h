/*
 * devices.h
 *
 *  Created on: 31/03/2010
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
//	Include
//-----------------------------------------------------------------------------
#include "dev/microcontroller.h"
#include "dev/uart_xbee.h"
#include "dev/uart_gps.h"
#include "dev/spi_scp1000.h"
#include "dev/i2c_acc.h"
#include "dev/adc.h"
#include "dev/timers.h"

//-----------------------------------------------------------------------------
//	Initialization functions
//-----------------------------------------------------------------------------
void dev_init()
{
	dev_osc();				// Configure the oscillator
	
	dev_register_init();	// Configure the device registers
	dev_settris_init();		// Configure pic Input/Output
	dev_pinset_init();		// Configure dsPIC remappable Input/Output

	dev_uart_xbee_init();	// Configure UART for XBEE
	dev_adc_init();			// Configure ADC
//	dev_spi_init();			// Configure SPI
	dev_i2c_init();			// Configure I2C
	dev_timer_init();		// Configure and enable the outputs of the servo
	dev_uart_gps_init();	// Configure UART for GPS
}

