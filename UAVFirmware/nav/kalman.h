/*
 * kalman.h
 *
 *  Created on: 09/04/2010
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
//	Variabili Globali
//-----------------------------------------------------------------------------
typedef struct Kalman
{
   // Le variabili rappresentano la matrice degli stati x
   float x_angle,
         x_bias;

   // Matrice degli errori di covarianza*/
   float P_00,
         P_01,
         P_10,
         P_11;   
   
   /* 
    * Q is a 2x2 matrix of the covariance. Because we
    * assume the gyro and accelero noise to be independend
    * of eachother, the covariances on the / diagonal are 0.
    *
    * Covariance Q, the process noise, from the assumption
    *    x = F x + B u + w
    * with w having a normal distribution with covariance Q.
    * (covariance = E[ (X - E[X])*(X - E[X])' ]
    * We assume is linair with dt
    */
   float Q_angle, Q_gyro;
   /*
    * Covariance R, our observation noise (from the accelerometer)
    * Also assumed to be linair with dt
    */
   float R_angle;
}Gyro1DKalman;

struct Kalman filter_roll;
struct Kalman filter_pitch;

//-----------------------------------------------------------------------------
//	Define vari
//-----------------------------------------------------------------------------
#define kalman_dt 0.01

//-----------------------------------------------------------------------------
// Prototipi funzioni
//-----------------------------------------------------------------------------
void init_Gyro1DKalman(Gyro1DKalman *filterdata, float Q_angle, float Q_gyro, float R_angle);

//-----------------------------------------------------------------------------
// Configurazione timer
//-----------------------------------------------------------------------------
void nav_kalman_init()
{
//	init_Gyro1DKalman(&filter_roll, 0.0001, 0.0003, 0.69); //originale
//	init_Gyro1DKalman(&filter_pitch, 0.0001, 0.0003, 0.69); //originale
	init_Gyro1DKalman(&filter_roll , 0.02, 0.04, 0.3);
	init_Gyro1DKalman(&filter_pitch, 0.02, 0.04, 0.3); 
}

//-----------------------------------------------------------------------------
// Funzioni di kalman
//-----------------------------------------------------------------------------
void init_Gyro1DKalman(Gyro1DKalman *filterdata, float Q_angle, float Q_gyro, float R_angle)
{
	filterdata -> Q_angle = Q_angle;
	filterdata -> Q_gyro  = Q_gyro;
	filterdata -> R_angle = R_angle;
}

/*
* The predict function. Updates 2 variables:
* our model-state x and the 2x2 matrix P
*     
* x = [ angle, bias ]' 
* 
*   = F x + B u
*
*   = [ 1 -dt, 0 1 ] [ angle, bias ] + [ dt, 0 ] [ dotAngle 0 ]
*
*   => angle = angle + dt (dotAngle - bias)
*      bias  = bias
*
*
* P = F P transpose(F) + Q
*
*   = [ 1 -dt, 0 1 ] * P * [ 1 0, -dt 1 ] + Q
*
*  P(0,0) = P(0,0) - dt * ( P(1,0) + P(0,1) ) + dt² * P(1,1) + Q(0,0)
*  P(0,1) = P(0,1) - dt * P(1,1) + Q(0,1)
*  P(1,0) = P(1,0) - dt * P(1,1) + Q(1,0)
*  P(1,1) = P(1,1) + Q(1,1)
*/

void ars_predict(Gyro1DKalman *filterdata, float dotAngle)
{
	filterdata->x_angle += kalman_dt * (dotAngle - filterdata->x_bias);
	
	filterdata->P_00 += - kalman_dt * (filterdata->P_10+filterdata->P_01)+filterdata->Q_angle*kalman_dt;
	filterdata->P_01 += - kalman_dt * filterdata->P_11;
	filterdata->P_10 += - kalman_dt * filterdata->P_11;
	filterdata->P_11 += + filterdata -> Q_gyro * kalman_dt;
}

/*
*  The update function updates our model using 
*  the information from a 2nd measurement.
*  Input angle_m is the angle measured by the accelerometer.
*
*  y = z - H x
*
*  S = H P transpose(H) + R
*    = [ 1 0 ] P [ 1, 0 ] + R
*    = P(0,0) + R
* 
*  K = P transpose(H) S^-1
*    = [ P(0,0), P(1,0) ] / S
*
*  x = x + K y
*
*  P = (I - K H) P
*
*    = ( [ 1 0,    [ K(0),
*          0 1 ] -   K(1) ] * [ 1 0 ] ) P
*
*    = [ P(0,0)-P(0,0)*K(0)  P(0,1)-P(0,1)*K(0),
*        P(1,0)-P(0,0)*K(1)  P(1,1)-P(0,1)*K(1) ]
*/

float ars_update(Gyro1DKalman *filterdata, float angle_m)
{
	static float y=0;              
	
	static float S=0;
	static float K_0=0;
	static float K_1=0;
	
	y = angle_m - filterdata->x_angle;
	
	S = filterdata->P_00 + filterdata->R_angle;
	K_0 = filterdata->P_00 / S;
	K_1 = filterdata->P_10 / S;
	
	filterdata->x_angle +=  K_0 * y;
	filterdata->x_bias  +=  K_1 * y;
	
	filterdata->P_00 -= K_0 * filterdata->P_00;
	filterdata->P_01 -= K_0 * filterdata->P_01;
	filterdata->P_10 -= K_1 * filterdata->P_00;
	filterdata->P_11 -= K_1 * filterdata->P_01;

	return filterdata->x_angle;
}

void kalman_calc()
{
	// Kalman roll
	ars_predict( &filter_roll , attitude.gyro_y.adc );								// Kalman predict   
	attitude.roll.angle = ars_update( &filter_roll , attitude.roll.acc_angle );		// Kalman update + result
	
	// Kalman pitch
	ars_predict(&filter_pitch, attitude.gyro_x.adc);								// Kalman predict
	attitude.pitch.angle = ars_update(&filter_pitch, attitude.pitch.acc_angle );	// Kalman update + result
}

