/*
 * pid.h
 *
 *  Created on: 13/05/2010
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
// Variabili globali
//-----------------------------------------------------------------------------
typedef struct {
	float coeff[3];				// Kp, Ki, Kd
	float errore[2];			// Ultime due stati dell'errore
	float integrale;			// Valore integrato dell'errore
	int command;				// Comando da fornire ai servo
	float misura;				// Valore reale del sistema
	float setpoint;				// Valore desiderato del sistema
} PID;

 PID PID_pitch, PID_roll;		// Definisce 2 strutture, pitch e roll

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define PID_dt 0.01

//-----------------------------------------------------------------------------
//	Prototipi
//-----------------------------------------------------------------------------
void pid(PID* pid_struct);

//-----------------------------------------------------------------------------
//	Inizializza il PID
//-----------------------------------------------------------------------------
void nav_pid_init()
{
	// Inizializza la struttura PID del pitch
	PID_pitch.command	= 0;			// Resetta l'output del pid
	PID_pitch.setpoint	= 0;			// Fissa il setpoint desiderato
	PID_pitch.errore[0]	= 0;			// Resetta i 2 stati d'errore
	PID_pitch.errore[1]	= 0;	
	PID_pitch.coeff[0]	= 0.4;			// Setta il coefficente Kp
	PID_pitch.coeff[1]	= 0;			// Setta il coefficente Ki
	PID_pitch.coeff[2]	= 0;			// Setta il coefficente Kd

	// Inizializza la struttura PID del roll	
	PID_roll.command	= 0;			// Resetta l'output del pid
	PID_roll.setpoint	= 3;			// Fissa il setpoint desiderato
	PID_roll.errore[0]	= 0;			// Resetta i 2 stati d'errore
	PID_roll.errore[1]	= 0;	
	PID_roll.coeff[0]	= 2;			// Setta il coefficente Kp
	PID_roll.coeff[1]	= 0;			// Setta il coefficente Ki
	PID_roll.coeff[2]	= 0;			// Setta il coefficente Kd
}
	
//-----------------------------------------------------------------------------
//	Calcola il PID totale
//-----------------------------------------------------------------------------
void pid_calc()
{
	// Calcolo il PID per il pitch
	PID_pitch.misura = attitude.pitch.acc_angle;			// Aggiorna il valore reale del sistema
	pid(&PID_pitch);										// Esegue il PID 
	servo_set(SRV_PITCH, PID_pitch.command);				// Comanda il servo con il valore calcolato dal PID

	// Calcolo il PID per il roll
	PID_roll.misura = attitude.roll.acc_angle;				// Aggiorna il valore reale del sistema
	pid(&PID_roll);											// Esegue il PID 
	servo_set(AIL_DX, PID_roll.command);					// Comanda i servo con il valore calcolato dal PID
	servo_set(AIL_SX, PID_roll.command);
}	

//-----------------------------------------------------------------------------
//	Calcola un PID generico
//-----------------------------------------------------------------------------
void pid( PID* pid_struct)
{	
//	pid_struct->errore[1] = pid_struct->errore[0];
	pid_struct->errore[0] = pid_struct->setpoint - pid_struct->misura;
//	pid_struct->integrale = pid_struct->integrale + ((pid_struct->errore[0] + pid_struct->errore[1])/2)*PID_dt;
	
	pid_struct -> command =  	
		pid_struct->coeff[0]	* pid_struct->errore[0]; 							// cmd(t) = Kp * e(t)    +
//		pid_struct->coeff[1]	* pid_struct->integrale; 							// 			Ki * I(e(t)) +
//		pid_struct->coeff[2]	* (pid_struct->errore[0] - pid_struct->errore[1]);	//			Kd * D(e(t))
}

