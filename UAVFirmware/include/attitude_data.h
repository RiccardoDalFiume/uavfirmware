/*
 *  attitude_data.h
 *
 *  Created on: 24/05/2010
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
// Dichiarazione strutture dati globali attitude
//-----------------------------------------------------------------------------
struct acc
{
	int8	read;
	int8	Cbuff[CIRC_BUF_ACC_LEN];
	int8	count;
};

struct gyro
{
	int		adc;
	int8	offset;
};

struct axis
{
	float	acc_angle;
	float	angle;
};

struct attitude
{
	struct acc acc_x;
	struct acc acc_y;
	struct acc acc_z;

	struct gyro gyro_x;
	struct gyro gyro_y;

	struct axis roll;
	struct axis pitch;
	struct axis yaw;
}attitude;
