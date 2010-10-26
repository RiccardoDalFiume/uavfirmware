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
//	Prototipi
//-----------------------------------------------------------------------------
void tmr_acc_read();
void tmr_gyro_read();
void tmr_nav();

//-----------------------------------------------------------------------------
// Configurazione timer
//-----------------------------------------------------------------------------
void dev_timer_init()
{
	uart_xbee_print("TIMER\tInitializing...\t");
	
	//-----------------------------------------------------------------------------
	// Configurazione TIMER2
	//-----------------------------------------------------------------------------
	T2CONbits.TCS 	= 0;			// Clock interno FCY
	T2CONbits.T32 	= 0;			// Modalità 16 bit
	T2CONbits.TCKPS = 1;			// Prescaler = 8
	T2CONbits.TGATE = 0;			// Modalità Gate disattivata - Utilizzo di un singolo timer
	T2CONbits.TSIDL = 1;			// Discontinue timer operation when device enters Idle mode
	PR2 = 12500;					// Interrupt ogni 2,5ms
	TMR2 = 0;						// Azzera il conteggio di TIMER2

	_T2IP = 5;						// Priorità dell'interrupt (0=min, 7=max)
	_T2IF = 0;						// Pulisci il flag di interrupt
	_T2IE = 1;						// Abilità l'interrupt per TIMER2
	T2CONbits.TON = 1;				// Abilitazione conteggio TIMER2
		
	//-----------------------------------------------------------------------------
	// Configurazione TIMER3
	//-----------------------------------------------------------------------------
	T3CONbits.TCS 	= 0;			// Clock interno FCY
	T3CONbits.TCKPS = 1;			// Prescaler = 8
	T3CONbits.TGATE = 0;			// Modalità Gate disattivata - Utilizzo di un singolo timer
	T3CONbits.TSIDL = 1;			// Discontinue timer operation when device enters Idle mode
	PR3 = 7500;						// Interrupt ogni 1,5ms (servo default)
	TMR3 = 0;						// Azzera il conteggio di TIMER3
	
	_T3IP = 7;						// Priorità dell'interrupt (0=min, 7=max)
	_T3IF = 0;						// Pulisci il flag di interrupt
	_T3IE = 1;						// Abilità l'interrupt per TIMER3
	T3CONbits.TON = 0;				// Abilitazione conteggio TIMER3
	
	uart_xbee_print("TIMER\tInitializing completed\r\n");
}

//-----------------------------------------------------------------------------
// Interrupt di TIMER2
//-----------------------------------------------------------------------------
void __attribute__((__interrupt__)) __attribute__((no_auto_psv)) _T2Interrupt(void)
{
	_T2IF = 0;						// Pulisce il flag di interrupt Timer2
	servo_tmrA();					// Gestisce i servo - TMR3
	tmr_acc_read();					// Legge l'accelerometro 	- 200 Hz
	tmr_gyro_read();				// Legge il giroscopio 		- 200 Hz
	tmr_nav();						// Calcola dati navigazione	- 100 Hz
}

//-----------------------------------------------------------------------------
// Interrupt di TIMER3
//-----------------------------------------------------------------------------
void __attribute__((__interrupt__)) __attribute__((no_auto_psv)) _T3Interrupt(void)
{
	_T3IF = 0;						// Pulisce il flag di interrupt TIMER3
	servo_tmrB();					// Chiama la routine di gestione dei servo
	T3CONbits.TON = 0;				// Disattiva conteggio TIMER3
}
