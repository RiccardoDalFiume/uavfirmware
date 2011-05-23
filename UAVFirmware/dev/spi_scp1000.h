/*
 * spi_scp1000.h
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
//	Include
//-----------------------------------------------------------------------------
#include "..\include\spi\spi.h"

//-----------------------------------------------------------------------------
//	Configurazione SPI
//-----------------------------------------------------------------------------
inline void dev_spi_init()
{	
	uart_xbee_print("SPI Initializing...");

	//variabili di configurazione dei registri SPI1, altimetro
	unsigned int SPI1CON1value;
	unsigned int SPI1CON2value;
	unsigned int SPI1STATvalue;

	// Configurazione registri SPI1
	SPI1CON1value	=	ENABLE_SCK_PIN &
						ENABLE_SDO_PIN &
						SPI_MODE16_OFF &
						SPI_SMP_OFF &
						SPI_CKE_OFF &
						SLAVE_ENABLE_OFF &
						CLK_POL_ACTIVE_HIGH &
						MASTER_ENABLE_ON &
						SEC_PRESCAL_6_1 &
						PRI_PRESCAL_16_1;

	SPI1CON2value	=	FRAME_ENABLE_OFF;

	SPI1STATvalue	=	SPI_ENABLE &
    					SPI_IDLE_STOP;
    					
	OpenSPI1( SPI1CON1value, SPI1CON2value, SPI1STATvalue );
	
	uart_xbee_print("SPI Initializing completed\r\n");
}

