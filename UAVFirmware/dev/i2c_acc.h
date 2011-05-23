/*
 * i2c_acc.h
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
#include "..\include\i2c\i2c.h"

//-----------------------------------------------------------------------------
//	Define
//-----------------------------------------------------------------------------
#define I2C_FRQ     400000L 			// I2C bus clock frequency, 400kHz

#define ACC_ADDR 0x3A					// Address accelerometer (0x1D) shifted to the left (0x3A)
#define READ_CMD    1
#define WRITE_CMD   0

#ifndef CIRC_BUF_ACC_LEN
	#define CIRC_BUF_ACC_LEN	64		// Length of circular buffer
	#define CIRC_BUF_ACC_DIV	6		// log2(CIRC_BUF_ACC_LEN) for the shift
#endif

//-----------------------------------------------------------------------------
//	Define accelerometer MMA7455 register
//-----------------------------------------------------------------------------
#define XOUTL 	0x00
#define XOUTH	0x01
#define YOUTL 	0x02
#define YOUTH 	0x03
#define ZOUTL 	0x04
#define ZOUTH 	0x05
#define XOUT8 	0x06
#define YOUT8 	0x07
#define ZOUT8 	0x08
#define STATUS	0x09
#define DETSRC 	0x0A
#define TOUT 	0x0B
#define I2CAD 	0x0D
#define USRINF 	0x0E
#define WHOAMI 	0x0F
#define XOFFL 	0x10
#define XOFFH 	0x11
#define YOFFL 	0x12
#define YOFFH 	0x13
#define ZOFFL 	0x14
#define ZOFFH 	0x15
#define MCTL 	0x16
#define INTRST 	0x17
#define CTL1 	0x18
#define CTL2 	0x19
#define LDTH 	0x1A
#define PDTH 	0x1B
#define PW 		0x1C
#define LT 		0x1D
#define TW 		0x1E

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
inline void acc_init();
void acc_write_reg(int8 reg, int8 data);

//-----------------------------------------------------------------------------
// Initialization I2C Hardware
//-----------------------------------------------------------------------------
inline void dev_i2c_init()
{
	uart_xbee_print("I2C\tInitializing...\t");

	OpenI2C1(I2C1_ON &					// Abilita il modulo I2C
		I2C1_7BIT_ADD &					// Indirizzamento a 7-bit
		I2C1_IPMI_DIS, 					// Disattiva IPMI
		(FCY /(I2C_FRQ))-1);			// Baud generator value

	IdleI2C1();
	delay_ms(2);						// Tempo di settaggio del BUS I2C
	acc_init();							// Inizializzazione dell'accelerometro
	
	uart_xbee_print("I2C\tInitializing completed\r\n");
	}
	
//-----------------------------------------------------------------------------
// Configuration of the accelerometer registers
//-----------------------------------------------------------------------------
inline void acc_init()
{
	attitude.acc_x.count = 0;
	attitude.acc_y.count = 0;
	attitude.acc_z.count = 0;
	
 	acc_write_reg(MCTL, 0b00000101);				// DRDY ON, ST OFF, 2G mode, misurazione continua	
//	acc_write_reg(CTL1, 0b00000000);				// Aggiornamento a 125 Hz
	acc_write_reg(CTL1, 0b10000000);				// Aggiornamento a 250 Hz
	acc_write_reg(XOFFL, 3);						// Calibra l'asse x dell'accelerometro
	acc_write_reg(YOFFL, 51);						// Calibra l'asse y dell'accelerometro
	acc_write_reg(ZOFFL, 0);						// Calibra l'asse z dell'accelerometro
}

//-----------------------------------------------------------------------------
// Update accelerometer values
//-----------------------------------------------------------------------------
void acc_up_xyz8()
{
	unsigned int media_x = 0;
	unsigned int media_y = 0;
	unsigned int media_z = 0;
	int8 k;
	
	StartI2C1();														// Mette la Start condition sul bus
	IdleI2C1();

	MasterWriteI2C1(ACC_ADDR | WRITE_CMD);								// Setta Master per trasmettere
	IdleI2C1();

	while(I2C1STATbits.ACKSTAT);										// Aspetta l'Acknowledge
	MasterWriteI2C1(XOUT8);												// Setta l'ind. del reg. da leggere
	IdleI2C1();

	while(I2C1STATbits.ACKSTAT);										// Aspetta l'Acknowledge
	RestartI2C1();														// Ripeti la Start condition
	IdleI2C1();

	MasterWriteI2C1(ACC_ADDR | READ_CMD);								// Setta Master per ricevere
	IdleI2C1();

	while(I2C1STATbits.ACKSTAT);										// Aspetta l'Acknowledge
	attitude.acc_x.Cbuff[attitude.acc_x.count++] = MasterReadI2C1();	// Memorizza il valore restituito
	IdleI2C1();

	AckI2C1();															// Manda un Acknowledge
	IdleI2C1();
	attitude.acc_y.Cbuff[attitude.acc_y.count++] = MasterReadI2C1();	// Memorizza il valore restituito
	IdleI2C1();

	AckI2C1();															// Manda un Acknowledge
	IdleI2C1();
	attitude.acc_z.Cbuff[attitude.acc_z.count++] = MasterReadI2C1() -10;// Memorizza il valore restituito
	IdleI2C1();

	NotAckI2C1();														// Manda un Not Acknowledge
	IdleI2C1();

	StopI2C1();															// Mette la Stop condition sul bus
	IdleI2C1();
	
	// Azzera i contatori del buffer circolare
	if( attitude.acc_x.count >= CIRC_BUF_ACC_LEN ) attitude.acc_x.count = 0;
	if( attitude.acc_y.count >= CIRC_BUF_ACC_LEN ) attitude.acc_y.count = 0;
	if( attitude.acc_z.count >= CIRC_BUF_ACC_LEN ) attitude.acc_z.count = 0;

	// Fa la somma del buffer circolare
	for(k = 0; k < CIRC_BUF_ACC_LEN; k++ )											
	{
		media_x += attitude.acc_x.Cbuff[k];
		media_y += attitude.acc_y.Cbuff[k];
		media_z += attitude.acc_z.Cbuff[k];
	}
	
	// Divide la somma dei valori per il numero di valori letti, Salva il valore calcolato	
	attitude.acc_x.read = media_x >> CIRC_BUF_ACC_DIV;
	attitude.acc_y.read = media_y >> CIRC_BUF_ACC_DIV;	
	attitude.acc_z.read = media_z >> CIRC_BUF_ACC_DIV;
}

//-----------------------------------------------------------------------------
// Scrittura di un singolo registro dell'accelerometro
//-----------------------------------------------------------------------------
void acc_write_reg(int8 reg, int8 data)
{	
	StartI2C1();
	IdleI2C1();

	MasterWriteI2C1(ACC_ADDR | WRITE_CMD);			// Setta Master per trasmettere allo Slave
	IdleI2C1();
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge

	MasterWriteI2C1(reg);							// Scrivi l'indirizzo del registro da leggere
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge
	IdleI2C1();

	MasterWriteI2C1(data);							// Imposta il dato da scrivere nel registro
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge

	StopI2C1();
	IdleI2C1();
}

//-----------------------------------------------------------------------------
// Lettura di un singolo registro dell'accelerometro
//-----------------------------------------------------------------------------
int8 acc_read_reg(int8 reg)
{
	int8 data = 0;

	StartI2C1();									// Mette la Start condition sul bus
	IdleI2C1();
	MasterWriteI2C1(ACC_ADDR | WRITE_CMD);			// Setta Master per trasmettere allo Slave
	IdleI2C1();
	
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge
	MasterWriteI2C1(reg);							// Scrivi l'indirizzo del registro da leggere
	IdleI2C1();
	
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge
	RestartI2C1();									// Ripeti la Start condition
	IdleI2C1();
	
	MasterWriteI2C1(ACC_ADDR | READ_CMD);			// Setta Master per ricevere dallo Slave
	IdleI2C1();
	
	while(I2C1STATbits.ACKSTAT);					// Aspetta l'Acknowledge
	data = MasterReadI2C1();						// Legge il valore restituito
	IdleI2C1();
	
	NotAckI2C1();									// Manda un Not Acknowledge
	IdleI2C1();
	
	StopI2C1();										// Mette la stop contidicon sul bus
	IdleI2C1();

	return (data);
}

//-----------------------------------------------------------------------------
// Lettura tramite timer dell'accelerometro - 200 Hz
//-----------------------------------------------------------------------------
void tmr_acc_read()
{
	static int f_div = 0;
	if (f_div < 2)
	{
		f_div++;
	}
	else 
	{
		f_div = 0;
		acc_up_xyz8();								// Aggiorno i valori dell'accelerometro
		
		// Calcolo l'inclinazione stimata dall'accelerometro, se entrambi sono 0 non esegue il calcolo
		if (attitude.acc_y.read || attitude.acc_z.read)
			attitude.roll.acc_angle = atan2f((float)attitude.acc_y.read, (float)attitude.acc_z.read)*RAD2DEG;
		if (attitude.acc_x.read || attitude.acc_z.read)
			attitude.pitch.acc_angle = atan2f(-(float)attitude.acc_x.read,(float)attitude.acc_z.read)*RAD2DEG;
		if (attitude.acc_x.read || attitude.acc_y.read)
			attitude.yaw.acc_angle = atan2f(-(float)attitude.acc_x.read, (float)attitude.acc_y.read)*RAD2DEG;
		f_div ++;
	}
}

