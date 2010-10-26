/*
 *  delay.h
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
//	Include delle librerie del dsPIC
//-----------------------------------------------------------------------------
#include "libpic30.h"

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define Kdelay_ms  (FCY/1000)
#define Kdelay_us  (FCY/1000000)

//-----------------------------------------------------------------------------
//	Funzioni
//-----------------------------------------------------------------------------
void delay_ms(int ms)
{
	__delay32(ms * Kdelay_ms);
}

void delay_us(int us)
{
	__delay32(us * Kdelay_us);
}
