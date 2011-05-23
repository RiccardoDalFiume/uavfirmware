/*
 * nav.h
 *
 *  Created on: 26/ott/2009
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
#include <math.h>
#include "nav/pid.h"
//#include "nav/kalman.h"

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define R 6371					// raggio della terra in km

//-----------------------------------------------------------------------------
//	Global variables
//-----------------------------------------------------------------------------
struct point
{
	float lat;
	float lon;
};

struct point my_pos, dest_pos;

//-----------------------------------------------------------------------------
// Prototype
//-----------------------------------------------------------------------------
float nav_distxy(struct point *p1, struct point *p2);

//-----------------------------------------------------------------------------
// Navigation and route
//-----------------------------------------------------------------------------
float nav_distxy(struct point *p1, struct point *p2)
{
	float dist = 0;
	dist = acosf(sinf((*p1).lat)*sinf((*p2).lat)+cosf((*p1).lat)*cosf((*p2).lat)*cosf((*p2).lon-(*p1).lon))*R;
	return dist;
}

//-----------------------------------------------------------------------------
// Kalman and PID
//-----------------------------------------------------------------------------
void nav_init()
{
	nav_pid_init();
//	nav_kalman_init();
}	

//-----------------------------------------------------------------------------
// Calculation navigation data via timer - 100 Hz
//-----------------------------------------------------------------------------
void tmr_nav()
{
	static int f_div = 0;
	if (f_div < 4)
	{
		f_div++;
	}
	else 
	{
		f_div = 0;
		pid_calc();		// Calcola il PID
		f_div ++;
	}
} 

