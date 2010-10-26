/*
 * pointer.h
 *
 *  Created on: 18/05/2010
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
//	Restituisce il n di celle di memoria fra due puntatori char
//-----------------------------------------------------------------------------
int ptrdiff( char *ptr1, char *ptr2)
{
	if (ptr2 > ptr1)
		return (int)(ptr2-ptr1);
	else
		return (int)(ptr1-ptr2);
}

