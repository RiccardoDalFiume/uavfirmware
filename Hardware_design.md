# Introduzione #

Il supporto hardware sul quale si fonda l'intero progetto è basato da un DSPic della Microchip, il dsPIC33FJ128GP804.


![http://uavfirmware.googlecode.com/files/DSPic_hw3.png](http://uavfirmware.googlecode.com/files/DSPic_hw3.png)


# Dettagli #

Ordinati secondo il tipo di connessione che utilizzano ci sono:

  * SPI
    * Accelerometro MMA 7455L [DS1](http://uavfirmware.googlecode.com/files/MMA7455L.pdf) [DS2](http://uavfirmware.googlecode.com/files/5300-M-ACC.pdf)
  * I2C
    * Sensore di pressione e temperatura SCP1000-D01 (Altimetro)
  * A/D Converter
    * Giroscopio IGD300 [DS1](http://uavfirmware.googlecode.com/files/DS_IDG300.pdf) [DS2](http://uavfirmware.googlecode.com/files/IDG300-Breakout.pdf)
    * Sensore tensione batteria
    * Sensore corrente batteria
    * Sensore di temperatura esterno
> [DS1](http://uavfirmware.googlecode.com/files/SCP1000.pdf) [DS2](http://uavfirmware.googlecode.com/files/SCP1000-Breakout-v13.pdf)
  * UART
    * GPS FV-M11 [DS1](http://uavfirmware.googlecode.com/files/FV-M11_Manual.pdf) [DS2](http://uavfirmware.googlecode.com/files/FV-M11_Spec.pdf)
    * XBee PRO II 100mw [DS](http://uavfirmware.googlecode.com/files/XBee-PRO%20OEM%20RF%202%2C4GHZ.pdf)
  * Digital output
    * Servi (da 1 a 5)

I componenti sono stati piazzati in un circuito stampato di formato 120x30mm; tali dimensioni sono necessarie a causa dei ristretti spazi della cellula dell'aereo.

Oltre ai componenti elencati sopra, nel circuito stampato è presente anche un circuito di by-pass che permette di escludere il dsPIC e abilitare il controllo manuale tramite un semplice radiocomando da modellismo.

# Sbrogliato #

Qui sotto riporto la versione C0.3 del circuito stampato, che è una delle candidate per la produzione finale.
[Madrescheda D0.8 TOP+BOTTOM](http://uavfirmware.googlecode.com/files/Madrescheda_top_and_bottom.png)
[Madrescheda D0.8 BOTTOM](http://uavfirmware.googlecode.com/files/Madrescheda_bottom.png)
[Madrescheda D0.8 TOP](http://uavfirmware.googlecode.com/files/Madrescheda_top.png)