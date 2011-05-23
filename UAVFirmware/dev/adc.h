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
// Prototype
//-----------------------------------------------------------------------------
inline void adc_gyro_init();
signed int adc_gyro_read(char input);

//-----------------------------------------------------------------------------
// Initialize A/D Converter
//-----------------------------------------------------------------------------
inline void dev_adc_init()
{
	uart_xbee_print("ADC\tInitializing...\t");

	_AD12B	= 0;						// 10 bit mode
	_CHPS	= 0b00;						// Use a single input (CH0)
	_VCFG	= 0b000;					// Set voltage range (AVDD_AVSS)
	_ADCS	= 0;						// Set ADC clock post-scaler
	_FORM0	= 1;						// Set output data-type (signed integer)
	_FORM1	= 0;
	_ASAM	= 0;						// ADC sampling must be controlled setting _SAMP to 1.
	_SSRC	= 0b111;					// The ADC conversion starts automatically after the sampling

	_ADON = 1;							// Enable the ADC

	delay_ms(10);						// Wait for the ADC to stabilize
	
	adc_gyro_init();					// Calibrates the gyro

	uart_xbee_print("ADC\tInitialization completed\r\n");
}

//-----------------------------------------------------------------------------
// Reading of the selected analog input. Must be previously set.
//-----------------------------------------------------------------------------
signed int adc_read()		
{
	int adc_value = 0;
	int8 k = 0;

	// White noise filter, 8 readings with the simple average
	for(k = 0; k < 8 ; k++)
	{
		AD1CON1bits.SAMP = 1; 			// Start Sampling
		delay_us(10); 					// Wait 10us after each reading
		AD1CON1bits.SAMP = 0; 			// Stop sampling and automatically convert
		while (!AD1CON1bits.DONE); 		// Wait for end of conversion
		adc_value += ADC1BUF0; 			// Add the converted value to the average buffer
	}
	
	adc_value = adc_value >> 3;			// Shift of 3, divide by 2^3 = 8

	return (adc_value);
}

//-----------------------------------------------------------------------------
// Gyroscope
//-----------------------------------------------------------------------------
inline void adc_gyro_init()
{
	attitude.gyro_x.offset = -adc_gyro_read('x') -4;
	attitude.gyro_y.offset = -adc_gyro_read('y');
}

signed int adc_gyro_read(char input)
{
	int adc_value = 0;

	switch(input)
	{
      case 'x':									// X-axis of the gyroscope
      	// Initialize MUXA Input Selection
		AD1CHS0bits.CH0SA = GYROX_ADC_AN; 		// Select GYROX_ADC_AN for the positive input of CH0
		AD1CHS0bits.CH0NA = 0; 					// Select VREF- for input negative of CH0
        break;

      case 'y':									// Y-axis of the gyroscope
		// Initialize MUXA Input Selection
		AD1CHS0bits.CH0SA = GYROY_ADC_AN; 		// Select GYROX_ADC_AN for the positive input of CH0
		AD1CHS0bits.CH0NA = 0; 					// Select VREF- for input negative of CH0
        break;

      default:
        break;
    }
    adc_value = adc_read();

    return (adc_value);
}

//-----------------------------------------------------------------------------
// Reading of the gyroscope via timer - 200 Hz
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
// Battery
//-----------------------------------------------------------------------------
float adc_bat_read()
{
	float bat_value = 0;

	// Set the channel of the AD to read the voltage
	AD1CHS0bits.CH0SA = V_ADC_AN;						// Select V_ADC_AN for the positive input of CH0
	AD1CHS0bits.CH0NA = 0; 								// Select VREF- for input negative of CH0
	
	bat_value = (adc_read()+ 512) * (float)K_VSENS;		// Reading the ADC and voltage calculation

	return (bat_value);
}


