/*
 * adc.h
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
//	Define
//-----------------------------------------------------------------------------
#define R1 302					// Valori in Kohm
#define R2 100					// Valori in Kohm
#define K_VSENS ((3.3 )/(1024)) * ((R1+R2)/R2) * (1.0292)

#define I_ADC_AN 1
#define V_ADC_AN 2
#define T_ADC_AN 3
#define GYROX_ADC_AN 9
#define GYROY_ADC_AN 10

//-----------------------------------------------------------------------------
// Prototipi
//-----------------------------------------------------------------------------
void adc_gyro_init();
signed int adc_gyro_read(char input);

//-----------------------------------------------------------------------------
// Inizializzazione A/D Converter
//-----------------------------------------------------------------------------
void dev_adc_init()
{
	uart_xbee_print("ADC\tInitializing...\t");

	_AD12B	= 0;						// Modalità 10 bit
	_CHPS	= 0b00;						// Utilizza un solo input (CH0)
	_VCFG	= 0b000;					// Setta i limiti di tensione (AVDD_AVSS)
	_ADCS	= 0;						// Post scaler del clock dell'ADC
	_FORM0	= 1;						// Seleziona il tipo di dato in uscita(signed integer)
	_FORM1	= 0;
	_ASAM	= 0;						// Il sampling dell'ADC deve essere comandato impostando a 1 _SAMP
	_SSRC	= 0b111;					// La conversione dell'ADC parte in automatico dopo il sampling

	_ADON = 1;							// Abilita l'ADC

	delay_ms(10);						// Aspetta che l'elettronica si stabilizzi
	
	adc_gyro_init();					// Calibra il giroscopio

	uart_xbee_print("ADC\tInitializing completed\r\n");
}

//-----------------------------------------------------------------------------
// Lettura dell'ingresso analogico impostato, è necessario impostarlo a priori
//-----------------------------------------------------------------------------
signed int adc_read()		
{
	int adc_value = 0;
	int8 k = 0;

	// Filtro rumore bianco, 8 letture con media semplice
	for(k = 0; k < 8 ; k++)
	{
		AD1CON1bits.SAMP = 1; 			// Avvia il campionamento
		delay_us(10); 					// Aspetta 10us dopo ogni lettura
		AD1CON1bits.SAMP = 0; 			// Ferma il campionamento e converti automaticamente
		while (!AD1CON1bits.DONE); 		// Aspetta la fine della conversione
		adc_value += ADC1BUF0; 			// Aggiungi il valore convertito al buffer della media
	}
	
	adc_value = adc_value >> 3;			// Shift binario di 3, divide per 2^3 = 8

	return (adc_value);
}

//-----------------------------------------------------------------------------
// Giroscopio
//-----------------------------------------------------------------------------
void adc_gyro_init()
{
	attitude.gyro_x.offset = -adc_gyro_read('x') -4;
	attitude.gyro_y.offset = -adc_gyro_read('y');
}

signed int adc_gyro_read(char input)
{
	int adc_value = 0;

	switch(input)
	{
      case 'x':									// Asse x del giroscopio
      	// Initialize MUXA Input Selection
		AD1CHS0bits.CH0SA = GYROX_ADC_AN; 		// Seleziona GYROX_ADC_AN per l'input positivo di CH0
		AD1CHS0bits.CH0NA = 0; 					// Seleziona VREF- per l'input negativo di CH0
        break;

      case 'y':									// Asse y del giroscopio
		// Initialize MUXA Input Selection
		AD1CHS0bits.CH0SA = GYROY_ADC_AN; 		// Seleziona GYROY_ADC_AN per l'input positivo di CH0
		AD1CHS0bits.CH0NA = 0; 					// Seleziona VREF- per l'input negativo di CH0
        break;

      default:
        break;
    }
    adc_value = adc_read();

    return (adc_value);
}

//-----------------------------------------------------------------------------
// Lettura tramite timer del giroscopio - 200 Hz
//-----------------------------------------------------------------------------
void tmr_gyro_read()
{
	static int f_div = 0;

	if (f_div < 2)
	{
		f_div++;
	}
	else
	{
		f_div = 0;
		attitude.gyro_x.adc = adc_gyro_read('x') + attitude.gyro_x.offset;
		attitude.gyro_y.adc = adc_gyro_read('y') + attitude.gyro_y.offset;
		f_div ++;
	}
}

//-----------------------------------------------------------------------------
// Batteria
//-----------------------------------------------------------------------------
float adc_bat_read()
{
	float bat_value = 0;

	// Imposto il canale dell'ad per leggere la tensione
	AD1CHS0bits.CH0SA = V_ADC_AN;						// Seleziona V_ADC_AN per l'input positivo di CH0
	AD1CHS0bits.CH0NA = 0; 								// Seleziona VREF- per l'input negativo di CH0
	
	bat_value = (adc_read()+ 512) * (float)K_VSENS;		// Lettura dell'adc e calcolo della tensione

	return (bat_value);
}


