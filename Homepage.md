## Progetto UAV Firmware ##
Questo progetto ha lo scopo di sviluppare il software e l'hardware necessario per far funzionare efficacemente un autopilota per aeromodellismo.

Il progetto è composto dai seguenti componenti:

> #### UAVFirmware ####
> UAVFirmware è il software del microcontrollore (un Microchip dsPic33FJ128GP804) a bordo dell'aereo.
> Il codice è scritto in C ed sviluppato in maniera modulare, separando la parte di gestione dell'hardware da quella di stabilizzazione e navigazione. Grazie a questi accorgimenti, il software è reso facilmente portabile su altri microcontrollori o su altre architetture, limitando le modifiche necessarie al solo codice di interfacciamento con l'hardware.

> #### UAVMainframe ####
> UAVMainframe è il programma che permette il controllo da terra dell'aereo.
> Il software gestisce la comunicazione con l'aereo: l'impostazione dei piani di volo, l'applicazione di eventuali correzioni, la ricezione delle coordinate dell'aereo e il monitoraggio delle periferiche.


## UAV Firmware project ##
This project is aimed at developing the software needed to build an autopilot system for model planes.

The project is composed by the following components:

> #### UAVFirmware ####
> UAVFirmware is the software of the microcontroller (a Microchip dsPic33FJ128GP804) aboard.
> The code is written in C and has been modularised, separating out the hardware management from the stabilisation and navigation one. Thanks to these solutions, it is easily portable to other microcontrollers or architectures, restricting the necessary modifications to the hardware interface only.

> #### UAVMainframe ####
> UAVMainframe is the program used to control the plane from land.
> The software takes care of the communication between plane and land: the flight plans setting, the application of corrections, the plane geographic coordinate reception and the monitoring of the devices.