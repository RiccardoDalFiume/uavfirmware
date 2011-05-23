/*
 * microcontroller.h
 *
 *  Created on: 22/04/2010
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
//	Settaggio registri all'accensione
//-----------------------------------------------------------------------------
#if EXT_OSC == 0							// Internal Clock
_FOSCSEL(FNOSC_FRCPLL );							// FRC Oscillator con PLL attivo
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF  & POSCMD_NONE);	// Clock Switching e Fail Safe Clock Monitor disabilitati
													// Funzione del pin OSC2: OSC2 è usato dal quarzo
													// Primary Oscillator Mode: Disabilitato

#elif EXT_OSC == 1							// External Clock
_FOSCSEL(FNOSC_PRIPLL & IESO_OFF);					// Primary oscillator (XT, HS, EC) con PLL
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF  & POSCMD_HS);	// Clock Switching e Fail Safe Clock Monitor disabilitati
													// Funzione del pin OSC2: OSC2 è usato dal quarzo
													// Primary Oscillator Mode: Oscillatore alta velocità HS
#endif

_FWDT(FWDTEN_OFF);									// Watchdog Timer disabled
_FPOR(ALTI2C_OFF);									// Usa il BUS I2C sui pin SDA1/SCL1
_FICD( JTAGEN_OFF & ICS_PGD3 );						// JTAG OFF, usa PGD3 in debug

//-----------------------------------------------------------------------------
//	Configura il PLL del dsPIC
//-----------------------------------------------------------------------------
void dev_osc()
{
	#if EXT_OSC == 0					// Internal Clock
		OSCTUNbits.TUN = 4;				// Regola la precisione della frequenza interna
		_PLLPRE = 3 - 2;				// Prescaler del PLL, 7.5/3 = 2.5Mhz
		_PLLDIV = 64-2;					// Divisore del PLL, 2,5 * 64 = 160Mhz  (il -2 serve per correggere il valore di molt. es 0=x2 1=x3 62=x64)
		_PLLPOST = 0;					// Postscaler del PLL, 160 / 2 = 80Mhz
		while(!OSCCONbits.LOCK);		// Attendi che il PLL sia agganciato

	#elif EXT_OSC == 1					// External Clock
		_PLLPRE = 2 -2;					// Prescaler del PLL, (il valore da settare al registro è inferire di 2 rispetto al valore di divisione)
		_PLLDIV = 32 -2;				// Divisore del PLL, (il valore da settare al registro è inferire di 2 rispetto al valore di divisione)
		_PLLPOST = 0;					// Postscaler del PLL, 0 = /2
		while(!OSCCONbits.LOCK);		// Attendi che il PLL sia agganciato

	#endif
}

//-----------------------------------------------------------------------------
//	Configura gli input/output rimappabili del dsPIC
//-----------------------------------------------------------------------------
void dev_settris_init()
{
//	TRISA
	_TRISA0 	= 0;			// Inutilizzato
	_TRISA1 	= 1;			// AN1 - I sense
	_TRISA2 	= 1;			// OSC 1
	_TRISA3 	= 1;			// OSC 2
	_TRISA4 	= 0;			// Servo 3
	_TRISA7		= 0;			// LED2
	_TRISA8		= 0;			// Servo 1
	_TRISA9		= 0;			// Servo 4
	_TRISA10 	= 0;			// LED1

//	TRISB
	_TRISB0		= 1;			// AN2 - V sense
	_TRISB1 	= 1;			// AN3 - T sense
	_TRISB2 	= 1;			// dsPic RX GPS TX
	_TRISB3 	= 0;			// dsPic TX GPS RX
	_TRISB4 	= 0;			// Servo 2
	_TRISB5 	= 1;			// PGED3
	_TRISB6 	= 1;			// PGEC3
	_TRISB7 	= 1;			// ACC DRDY
	_TRISB8 	= 1;			// ACC SCL 
	_TRISB9 	= 1;			// ACC SDA 
	_TRISB10 	= 0;			// SCP1000 CSB
	_TRISB11	= 1;			// SCP1000 MISO
	_TRISB12 	= 0;			// SCP1000 MOSI
	_TRISB13 	= 0;			// SCP1000 SCK
	_TRISB14 	= 1;			// AN10 - GYRO Y
	_TRISB15 	= 1;			// AN9 - GYRO X

//	TRISC
	_TRISC0 	= 1;			// AUX1
	_TRISC1 	= 0;			// AUX2
	_TRISC2 	= 0;			// AUX3
	_TRISC3 	= 0;			// Servo 5
	_TRISC4		= 0;			// dsPic TX Xbee RX
	_TRISC5 	= 1;			// dsPic RX Xbee TX
	_TRISC6 	= 1;			// ACC INT2
	_TRISC7 	= 0;			// Inutilizzato
	_TRISC8 	= 0;			// Inutilizzato
	_TRISC9 	= 1;			// SCP1000 DRDY - scavalcato
}

void dev_pinset_init()
{
//-----------------------------------------------------------------------------
//	Remappable Input PIN tables
//-----------------------------------------------------------------------------
	_U2RXR		= 2;			//Pin 23 : DSPRX_GPSTX, RP2
	_U1RXR 		= 21;			//Pin 38: DSPRX_XBEETX, RP21

//-----------------------------------------------------------------------------
//	Remappable Output PIN tables
//-----------------------------------------------------------------------------
	_RP3R 		= 0b00101;		// Pin 24 : DSPTX_GPSRX, RP3
	_RP20R	 	= 0b00011;		// Pin 37: DSPTX_XBEERX, RP20

//-----------------------------------------------------------------------------
//	PIN Analog/Digital set
//-----------------------------------------------------------------------------
	_PCFG0		= 1;			// PIN Digital
	_PCFG1		= 0;			// PIN Analog
	_PCFG2		= 0;			// PIN Analog
	_PCFG3		= 0;			// PIN Analog
	_PCFG4 		= 1;			// PIN Digital
	_PCFG5		= 1;			// PIN Digital
	_PCFG6		= 1;			// PIN Digital
	_PCFG7		= 1;			// PIN Digital
	_PCFG8		= 1;			// PIN Digital
	_PCFG9 		= 0;			// PIN Analog
	_PCFG10		= 0;			// PIN Analog
	_PCFG11		= 1;			// PIN Digital
	_PCFG12		= 1;			// PIN Digital
}

//-----------------------------------------------------------------------------
//	Disabilito il clock delle periferiche non usate
//-----------------------------------------------------------------------------
void dev_register_init()
{
	_T5MD				= 1;	// Timer5 Module Disable bit
	_T4MD				= 1;	// Timer4 Module Disable bit
	_T3MD				= 0;	// Timer3 Module Disable bit
	_T2MD				= 0;	// Timer2 Module Disable bit
	_T1MD 				= 1;	// Timer1 Module Disable bit
	_DCIMD 				= 1;	// DCI Module Disable bit
	_I2C1MD				= 0;	// I2C1 Module Disable bit
	_U1MD				= 0;	// UART1 Module Disable bit
	_U2MD				= 0;	// UART2 Module Disable bit
	_SPI1MD				= 0;	// SPI1 Module Disable bit
	_SPI2MD				= 1;	// SPI2 Module Disable bit
	_C1MD				= 1;	// ECAN1 Module Disable bit
	_AD1MD				= 0;	// ADC1 Module Disable bit
	_IC8MD				= 1;	// Input Capture 8 Module Disable bit
	_IC7MD				= 1;	// Input Capture 2 Module Disable bit
	_IC2MD 				= 1;	// Input Capture 2 Module Disable bit
	_IC1MD				= 1;	// Input Capture 1 Module Disable bit
	_OC4MD				= 1;	// Output Compare 4 Module Disable bit
	_OC3MD				= 1;	// Output Compare 3 Module Disable bit
	_OC2MD				= 1;	// Output Compare 2 Module Disable bit
	_OC1MD				= 1;	// Output Compare 1 Module Disable bit
	_CMPMD				= 1;	// Comparator Module Disable bit
	_RTCCMD				= 1;	// RTCC Module Disable bit
	_PMPMD				= 1;	// PMP Module Disable bit
	_CRCMD	 			= 1;	// CRC Module Disable bit
	_DAC1MD				= 1;	// DAC1 Module Disable bit
}
