# Introduzione #

![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/software_design.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/software_design.png)


Il codice è scritto in C30, una versione modificata del più comune linguaggio di programmazione C.
L'intero programma è strutturato in maniera da essere più modulare possibile, separando la parte di codice necessaria all'interfacciamento con l'hardware (colore rosso nello schema) da quella generica (azzurra o arancione), come il software di navigazione e le librerie generiche.

Il codice è organizzato in sottocartelle:
  * “/”	→ è la cartella principale che contiene tutti i file;
  * “/dev”	→ contiene il codice di configurazione delle periferiche come l'ADC, l'I2C,  l'SPI e l'UART;
  * “/nav”	→ contiene il codice del motore di navigazione, il PID e Kalman;
  * “/include” → contiene varie librerie necessarie al funzionamento del programma.

> ### main.c ###

> Contiene la funzione main() dalla quale il microcontrollore inizia ad eseguire il codice all'avvio.

> ### devices.h ###

> Il file devices.h funziona da contenitore per tutti gli header relatici alle periferiche.
> #### microcontroller.h ####
> Contiene le impostazioni dell'oscillatore, delle porte I/O, imposta il tipo di pin (analogico o digitale), collega le periferiche a specifici pin, inoltre disabilita le periferiche non utilizzate per risparmiare energia.
> #### timers.h ####
> Configura il TIMER2 e TIMER3: il primo è utilizzato come riferimento per azionare i servo, leggere i sensori ed eseguire il motore di navigazione. Il secondo viene utilizzato solamente dai servo.
> #### adc.h ####
> Configura il convertitore analogico digitale per la lettura da giroscopio, batteria, sensore corrente e sensore di temperatura esterno.
> #### servo.h ####
> Contiene il codice specifico per generare i segnali PWM necessari al comando dei servo.
> #### i2c\_acc.h ####
> Contiene il codice per configurare il modulo I2C e le funzioni per la lettura dell'accelerometro.
> #### spi\_scp1000.h ####
> Configura il modulo SPI.
> #### uart\_xbee.h ####
> Configura la UART1 e contiene le relative funzioni per la lettura e la scrittura.
> #### uart\_xbee\_term.h ####
> Contiene l'interprete (parser) del protocollo di comunicazione tra terra e UAV.
> #### uart\_gps.h ####
> Configura la UART2 e contiene le relative funzioni per la lettura e la scrittura.
> #### uart\_gps\_nmea.h ####
> Contiene l'interprete (parser) del protocollo NMEA 0183.


> ### nav.h ###
> Il file nav.h contiene il motore di navigazione e gli algoritmi di auto-pilotaggio e stabilizzazione dell'aereo nonché tutti i controlli di posizione e movimento.

> #### kalman.h ####
> Contiene il codice relativo al filtro di Kalman.
> #### pid.h ####
> Contiene il codice relativo al PID.