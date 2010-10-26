/*
 * crc.h
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
//	Define
//-----------------------------------------------------------------------------
#include <string.h>

/*
 * Le stringhe in ingresso alle due funzione devono essere prive dei caratteri
 * terminatori \n\r
 */
 
//-----------------------------------------------------------------------------
//	Calcolo CRC e restituzione di un int
//-----------------------------------------------------------------------------
int8 crc_nmea(char *str)
{
	int i;
	unsigned char XOR;
	unsigned int iLen;
//	static char res[2];
	char buff[strlen(str)];
	strcpy(buff, &str[1]);

	buff[strlen(buff)-3] = '\0';
	
	iLen = strlen(buff);
	
	for (XOR = 0, i = 0; i < iLen; i++)
		XOR ^= (unsigned char)buff[i];

	//sprintf(res, "%2X", XOR);
	return XOR;
}

//-----------------------------------------------------------------------------
//	Calcolo CRC e restituzione di un puntatore ad una stringa
//-----------------------------------------------------------------------------
char* crc_nmea_str(char *str)
{
	int i;
	unsigned char XOR;
	unsigned int iLen;
	static char res[2];
	char buff[strlen(str)];
	
	strcpy(buff, &str[1]);			// Copia senza il $
	buff[strlen(buff)-3] = '\0';	// Cancella i 3 caratteri finali
	
	iLen = strlen(buff);
	
	for (XOR = 0, i = 0; i < iLen; i++)
		XOR ^= (unsigned char)buff[i];

	sprintf(res, "%2X", XOR);
	return res;
}
