/*
 * uart_xbee_term.h
 *
 *  Created on: 07/01/2010
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
// Include
//-----------------------------------------------------------------------------
#include <string.h>

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define XBEE_STRING_LENGHT 	100	// Numero max caratteri stringa autopilota

//-----------------------------------------------------------------------------
//	Struct dati
//-----------------------------------------------------------------------------
/*
 * GSNAV - informazioni sulla posizione del velivolo
 * $GSNAV,123519,309.62,4807.038,N,01131.000,E,24,65,124,50*6A
 */
	struct {
	float time_utc;					// Time (UTC)
	float degrees_true;				// Direzione in gradi reali
	float latitude;					// Latitudine
	char nos_lat;					// N or S (Nord o Sud)
	float longitude;				// Longitudine
	char eow_long;					// E o W (Est o Ovest)
	float speed_overg_kmh;			// Velocità al suolo in Km/h
	float alt_meters;				// Altezza dal suolo in metri
	int  checksum;					// Checksum
} gsnav;

/*
 * GSPLT - Informazioni sullo stato dell'autopilota
 * $GSPLT,123519,0,2400,0,4807.038,N,01131.000,E*6A
 */
	struct {
	float time_utc;					// Time (UTC)
	float alt_meters_or_bool;		// Se 0 ALT è disattivato, sennò sono i metri di ALT
	float speed_kmh;				// Velocità di crociera impostata
	unsigned int wp_follow_bool;	// Autopilota attivo (1) o disattivo (0)
	float wp_lat;					// Latitudine del Way Point
	char wp_nos;					// N or S (Nord o Sud)
	float wp_long;					// Longitudine del Way Point
	char wp_eow;					// E o W (Est o Ovest)
	int  checksum;					// Checksum
} gsplt;

/*
 * ALM - Allarmi 
 * $GSALM,123519,1,0*6A
 */
	struct {
	float time_utc;					// Time (UTC)
	unsigned int wp_arrived_bool;	// L'UAV è arrivato al Way Point
	unsigned int alt_meters_bool;	// Allarme quota
	int  checksum;					// Checksum
} gsalm;

//-----------------------------------------------------------------------------
//	Variabili globali
//-----------------------------------------------------------------------------
char xbee_string[XBEE_STRING_LENGHT];

//-----------------------------------------------------------------------------
//	Interprete informazioni di navigazione
//-----------------------------------------------------------------------------
int xbee_parser(char *source)
{
	char source_buffer[100];
	strcpy(source_buffer, source);

	// Elimina i due caratteri finali
	source[strlen(source_buffer)-2] = '\0'; 		
	
	// Controlla CRC prima del parse per evitare di salvare valori errati
	if(strcmp(crc_nmea_str(source), &source[strlen(source)-2])) return -2;	

	if (!strncmp(source_buffer, "$GSNAV", 6))
	{
		if (sscanf(&source[7], "%f,%f,%f,%c,%f,%c,%f,%f*%2X",
			&gsnav.time_utc,
			&gsnav.degrees_true,
			&gsnav.latitude,
			&gsnav.nos_lat,
			&gsnav.longitude,
			&gsnav.eow_long,
			&gsnav.speed_overg_kmh,
			&gsnav.alt_meters,
			(int*)&gsnav.checksum ) == EOF) return -1;
			
		return 0;	
	}

	else if (!strncmp(source_buffer, "$GSPLT", 6))
	{
		if (sscanf(&source[7], "%f,%f,%f,%u,%f,%c,%f,%c*%2X",
			&gsplt.time_utc,
			&gsplt.alt_meters_or_bool,
			&gsplt.speed_kmh,
			&gsplt.wp_follow_bool,
			&gsplt.wp_lat,
			&gsplt.wp_nos,
			&gsplt.wp_long,
			&gsplt.wp_eow,
			(int*)&gsplt.checksum ) == EOF) return -1;

		return 0;
	}
	
	else if (!strncmp(source_buffer, "$GSALM", 6))
	{
		if( sscanf(&source[7], "%f,%u,%u*%2X", 
			&gsalm.time_utc,
			(unsigned int*)&gsalm.wp_arrived_bool,
			(unsigned int*)&gsalm.alt_meters_bool,
			(int*)&gsalm.checksum ) == EOF) return -1;
		
		return 0;	
	}

	return -3;
}
