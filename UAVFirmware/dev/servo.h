/*
 * servo.h
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
//	Define
//-----------------------------------------------------------------------------
#define N_SERVO 5 +1

#define ZERO 	7500
#define MOT_OFF 5000

#define SRV_1 _LATA8
#define SRV_2 _LATB4
#define SRV_3 _LATA4
#define SRV_4 _LATA9
#define SRV_5 _LATC3

enum servo
{
	AIL_DX = 1,
	SRV_PITCH,
	MOT,
	SRV_TIM,
	AIL_SX
};

//-----------------------------------------------------------------------------
// Prototipi delle funzioni
//-----------------------------------------------------------------------------
void servo_set_us(int8 servo_num, int servo_set);

//-----------------------------------------------------------------------------
// Definizione array dei delay dei servi
//-----------------------------------------------------------------------------
static int servo_delay[N_SERVO] =
	{0,
	ZERO,			// Servo 1
	ZERO,			// Servo 2
	MOT_OFF,		// Servo 3
	ZERO,			// Servo 4
	ZERO,			// Servo 5
	};

//-----------------------------------------------------------------------------
// Alza il pin del servo - TIMER2
//-----------------------------------------------------------------------------
void servo_tmrA()
{
	static int8 srv = 1;

	switch(srv)
	{
		case 1:
			PR3 = servo_delay[1];		// Imposta il Th del servo (da 1 a 2 ms)
			SRV_1 = 1;					// Alza il pin del servo
			T3CONbits.TON = 1;			// Inizia il conteggio di TIMER3
			break;

		case 2:
			PR3 = servo_delay[2];		// Imposta il Th del servo (da 1 a 2 ms)
			SRV_2 = 1;					// Alza il pin del servo
			T3CONbits.TON = 1;			// Inizia il conteggio di TIMER3
			break;

		case 3:
			PR3 = servo_delay[3];		// Imposta il Th del servo (da 1 a 2 ms)
			SRV_3 = 1;					// Alza il pin del servo
			T3CONbits.TON = 1;			// Inizia il conteggio di TIMER3
			break;

		case 4:
			PR3 = servo_delay[4];		// Imposta il Th del servo (da 1 a 2 ms)
			SRV_4 = 1;					// Alza il pin del servo
			T3CONbits.TON = 1;			// Inizia il conteggio di TIMER3
			break;

		case 5:
			PR3 = servo_delay[5];		// Imposta il Th del servo (da 1 a 2 ms)
			SRV_5 = 1;					// Alza il pin del servo
			T3CONbits.TON = 1;			// Inizia il conteggio di TIMER3
			break;
	}
	
	if(srv >=8)
		srv=0;		//NB, dopo viene incrementato a 1, il servo n 0 nn esiste!
	srv++;	
}

//-----------------------------------------------------------------------------
// Abbassa i pin dei servo - TIMER3
//-----------------------------------------------------------------------------
void servo_tmrB()
{
	SRV_1 = 0;
	SRV_2 = 0;
	SRV_3 = 0;
	SRV_4 = 0;
	SRV_5 = 0;
}

//-----------------------------------------------------------------------------
// Imposta la durata dell'impulso PWM di un servo in us - MIN 1000 - MAX 2000
//-----------------------------------------------------------------------------
void servo_set_us(int8 servo_num, int servo_set)
{
	if(servo_set > 2000) servo_set = 2000;
	if(servo_set < 1000) servo_set = 1000;
	servo_delay[servo_num] = servo_set * 5;
}

//-----------------------------------------------------------------------------
// Imposta la durata dell'impulso PWM di un servo in % - MIN -100% - MAX +100%
//-----------------------------------------------------------------------------
void servo_set(int8 servo_num, int servo_set)							//	(servo_set da -100% a 100%)
{
	if(servo_set > 100) servo_set = 100;
	if(servo_set < -100) servo_set = -100;
//	servo_delay[servo_num] = ((servo_set * 5) + 1500) * 5;				// correzione offset e guadagno + adattamento al registro x il timer, non ottimizzata
	servo_delay[servo_num] = (servo_set * 25) + 7500;					// correzione offset e guadagno + adattamento al registro x il timer, ottimizzata
}

//-----------------------------------------------------------------------------
// Imposta la durata dell'impulso PWM del motore in % - MIN 0% - MAX +100%
//-----------------------------------------------------------------------------
void motor_set( int mot_set)
{
	if(mot_set > 100) mot_set = 100;
	if(mot_set < 0) mot_set = 0;
//	servo_delay[MOT] = ((mot_set * 10) + 1000) * 5;				// correzione offset e guadagno + adattamento al registro x il timer, non ottimizzata
	servo_delay[MOT] = (mot_set * 50) + 5000;						// correzione offset e guadagno + adattamento al registro x il timer, ottimizzata
}

