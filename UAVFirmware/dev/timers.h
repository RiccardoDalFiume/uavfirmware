/*
 * timers.h
 *
 *  Created on: 11/05/2010
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
#include "servo.h"

//-----------------------------------------------------------------------------
//	Prototypes
//-----------------------------------------------------------------------------
void tmr_acc_read();
void tmr_gyro_read();
void tmr_nav();

//-----------------------------------------------------------------------------
// Configure Timer
//-----------------------------------------------------------------------------
inline void dev_timer_init()
{
	uart_xbee_print("TIMER\tInitialization...\t");
	
	//-----------------------------------------------------------------------------
	// Configure TIMER2
	//-----------------------------------------------------------------------------
	T2CONbits.TCS 	= 0;			// Internal Clock FCY
	T2CONbits.T32 	= 0;			// 16 bit mode
	T2CONbits.TCKPS = 1;			// Prescaler = 8
	T2CONbits.TGATE = 0;			// Disable Gate mode - Using a single timer
	T2CONbits.TSIDL = 1;			// Discontinue timer operation when device enters Idle mode
	PR2 = 12500;					// Interrupt every 2,5ms
	TMR2 = 0;						// Clean TIMER2 counter

	_T2IP = 5;						// Set interrupt priority (0=min, 7=max)
	_T2IF = 0;						// Clean TIMER2 interrupt flag
	_T2IE = 1;						// Enable TIMER2 interrupt
	T2CONbits.TON = 1;				// Enable TIMER2 counter
		
	//-----------------------------------------------------------------------------
	// Configure TIMER3
	//-----------------------------------------------------------------------------
	T3CONbits.TCS 	= 0;			// Internal Clock FCY
	T3CONbits.TCKPS = 1;			// Prescaler = 8
	T3CONbits.TGATE = 0;			// Disable Gate mode - Using a single timer
	T3CONbits.TSIDL = 1;			// Discontinue timer operation when device enters Idle mode
	PR3 = 7500;						// Interrupt every 1,5ms (servo default)
	TMR3 = 0;						// Clean TIMER3 counter
	
	_T3IP = 7;						// Set interrupt priority (0=min, 7=max)
	_T3IF = 0;						// Clean TIMER3 interrupt flag
	_T3IE = 1;						// Enable TIMER3 interrupt
	T3CONbits.TON = 0;				// Enable TIMER3 counter
	
	uart_xbee_print("TIMER\tInitialization completed\r\n");
}

//-----------------------------------------------------------------------------
// Interrupt of TIMER2
//-----------------------------------------------------------------------------
void __attribute__((__interrupt__)) __attribute__((no_auto_psv)) _T2Interrupt(void)
{
	_T2IF = 0;						// Clean Timer2 interrupt flag
	servo_tmrA();					// Manage servo - TMR3
	tmr_acc_read();					// Read accelerometer 	- 200 Hz
	tmr_gyro_read();				// Read gyroscope 		- 200 Hz
	tmr_nav();						// Calculate route data	- 100 Hz
}

//-----------------------------------------------------------------------------
// Interrupt of TIMER3
//-----------------------------------------------------------------------------
void __attribute__((__interrupt__)) __attribute__((no_auto_psv)) _T3Interrupt(void)
{
	_T3IF = 0;						// Clean TIMER3 interrupt flag
	servo_tmrB();					// Call the servo function
	T3CONbits.TON = 0;				// Disable TIMER3 counter
}
