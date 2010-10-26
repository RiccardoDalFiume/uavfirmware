/*
 * uart_gps_nmea.h
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
#include <string.h>

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define NMEA_STRING_LENGHT 	100	// Numero max caratteri stringa NMEA

//-----------------------------------------------------------------------------
//	Struct dati
//-----------------------------------------------------------------------------
/*
 * GGA - Global Positioning System Fix Data.
 * $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
 */
	struct {
	float time_utc;			// Time (UTC)
	float latitude;			// Latitude
	char  nos_lat;			// N or S (North or South)
	float longitude;		// Longitude
	char  eow_long;			// E or W (East or West)
	int  gps_quality;		// GPS Quality Indicator, 0-FIX not available, 1-GPS FIX, 2-Differential GPS fix
	int  n_satellites;		// Number of satellites in view, 00 - 12
	float hdop;				// Horizontal Dilution of precision
	float altitude_msl;		// Antenna Altitude above/below mean-sea-level (geoid)
	char  meters1;			// Units of antenna altitude, meters
	float altitude_wgs84;	// Geoidal separation, the difference between the WGS-84 earth
							// ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
	char  meters2;			// Units of geoidal separation, meters
	// Empty				// Age of differential GPS data, time in seconds since last SC104
							// type 1 or 9 update, null field when DGPS is not used
	// Empty				// Differential reference station ID, 0000-1023
	int  checksum;			// Checksum
} gpgga;

/*
 * GLL - Geographic Position - Latitude/Longitude
 * $GPGLL,4916.45,N,12311.12,W,225444,A,*1D
 *
	struct {
	float latitude;			// Latitude
	char  nos_lat;			// N or S (North or South)
	float longitude;		// Longitude
	char  eow_long;			// E or W (East or West)
	float time_utc;			// Time (UTC)
	char  status;			// Status A - Data Valid, V - Data Invalid
	int  checksum;			// Checksum
} gpgll;
*/

/*
 * RMC - Recommended Minimum Navigation Information
 * $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
 */
	struct {
	float time_utc;			// Time (UTC)
	char  status;			// Status, V = Navigation receiver warning
	float latitude;			// Latitude
	char  nos_lat;			// N or S (North or South)
	float longitude;		// Longitude
	char  eow_long;			// E or W (East or West)
	float speed_overg_knots;// Speed over ground, knots
	float degrees_true;		// Track made good, degrees true
	float date;				// Date, ddmmyy
	float degrees_magnvar;	// Magnetic Variation, degrees
	char  eow_deg;			// E or W (East or West)
	int  checksum;			// Checksum
} gprmc;

/*
 * VTG - Track Made Good and Ground Speed
 * $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48
 *
	struct {
	float degrees_track;	// True track made good (degrees)
	char  true;				// T = True
	float magnetic_track;	// Magnetic track made good
	char  magnetic;			// M = Magnetic
	float speed_overg_knots;// Speed over ground, knots
	char  knots;			// N = Knots
	float speed_overg_kmh;	// Speed over ground, km/h
	char  kmh;				// K = Kilometers Per Hour
	int  checksum;			// Checksum
} gpvtg;
*/

/*
 * ZDA - Time & Date - UTC, Day, Month, Year and Local Time Zone
 * $GPZDA,201530.00,04,07,2002,00,00*60
 */
	struct {
	float time_utc;			// Time (UTC)
	int8  day;				// Day, 01 to 31
	int8  month;			// Month, 01 to 12
	int8  year;				// Year
	int8  hour_local;		// Local zone hour
	int8  min_local;		// Local zone minutes
	int  checksum;			// Checksum
} gpzda;

//-----------------------------------------------------------------------------
//	Variabili globali
//-----------------------------------------------------------------------------
char nmea_string[NMEA_STRING_LENGHT];

//-----------------------------------------------------------------------------
//	Interprete NMEA 0813
//-----------------------------------------------------------------------------
int nmea_parser(char *source)
{
	char source_buffer[100];
	strcpy(source_buffer, source);

	// Elimina i due caratteri finali
	source[strlen(source_buffer)-2] = '\0'; 		
	
	// Controlla CRC prima del parse per evitare di salvare valori errati
	if(strcmp(crc_nmea_str(source), &source[strlen(source)-2])) return -2;	

	if (!strncmp(source_buffer, "$GPGGA", 6))
	{
		if (sscanf(&source[7], "%f,%f,%c,%f,%c,%i,%i,%f,%f,%c,%f,%c,,*%2X",
			&gpgga.time_utc,
			&gpgga.latitude,
			&gpgga.nos_lat,
			&gpgga.longitude,
			&gpgga.eow_long,
			(int*)&gpgga.gps_quality,
			(int*)&gpgga.n_satellites,
			&gpgga.hdop,
			&gpgga.altitude_msl,
			&gpgga.meters1,
			&gpgga.altitude_wgs84,
			&gpgga.meters2,
			(int*)&gpgga.checksum ) == EOF) return -1;
			
		return 0;	
	}

	else if (!strncmp(source_buffer, "$GPRMC", 6))
	{
		if (sscanf(&source[7], "%f,%c,%f,%c,%f,%c,%f,%f,%f,%f,%c*%2X",
			&gprmc.time_utc,
			&gprmc.status,
			&gprmc.latitude,
			&gprmc.nos_lat,
			&gprmc.longitude,
			&gprmc.eow_long,
			&gprmc.speed_overg_knots,
			&gprmc.degrees_true,
			&gprmc.date,
			&gprmc.degrees_magnvar,
			&gprmc.eow_deg,
			(int*)&gprmc.checksum ) == EOF) return -1;

		return 0;
	}
			
	else if (!strncmp(source_buffer, "$GPZDA", 6))
	{
		if( sscanf(&source[7], "%f,%i,%i,%i,%i,%i*%2X", 
			&gpzda.time_utc,
			(int*)&gpzda.day,
			(int*)&gpzda.month,
			(int*)&gpzda.year,
			(int*)&gpzda.hour_local,
			(int*)&gpzda.min_local,
			(int*)&gpzda.checksum ) == EOF) return -1;
		
		return 0;	
	}
	
	return -3;
}

