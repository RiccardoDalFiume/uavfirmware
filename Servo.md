# Servi #

Tutti i 5 servo, motore compreso, sono comandati in PWM (Pulse Width Modulation).
Il segnale PWM di comando ha un periodo di 20ms, e la durata dell'impulso varia da 1 ms a 2 ms, in particolare:

  * i servo assumono la posizione angolare di 0° a 1,5 ms, -90° a 1 ms e +90° a 2 ms;
  * il motore è fermo a 1 ms e a piena potenza a 2 ms.

http://uavfirmware.googlecode.com/files/TEK00021.PNG

Il connettore di collegamento di un servo è a tre poli:
  * massa
  * alimentazione
  * segnale di comando (riferito a massa)

Il consumo di un servo dipende dalle sue dimensioni e dal carico al quale è applicato e generalmente va da una decina di mA a riposo, fino a qualche centinaia di mA sotto sforzo.
Il regolatore del motore, che genera la trifase per il brushless, come già detto, è controllato come un servo;
è alimentato direttamente dalla batteria (LiPo 11,1V, 1750mAh) e fornisce, direttamente sul pin di alimentazione del connettore, una tensione stabilizzata a 5V che alimenta la ricevente e i servo eliminando la necessità di un'ulteriore batteria (funzione BEC - Battery Elimination Circuit).


Qui vediamo tutti e 5 i segnali dei servo: notiamo che non sono sovrapposti a causa dell'algoritmo che genera il segnale di pilotaggio.

http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/osc/TEK00001.PNG

Siccome il dsPIC utilizzato non è un modello MC (Motor Controller) bensì un GP (General Purpose), il pilotaggio dei servo non avviene con i generatori di PWM hardware integrati, dato che sono solo 4, bensì sono generati via software con l'utilizzo di due timer, TIMER2 e TIMER3.
Per la scelta dei timer si è controllato che non ci fossero funzioni particolari associati a qualcuno di essi, questo per essere pronti nel caso che queste funzioni possano essere utili in futuro.

Il segnale dei servo viene generato nella seguente maniera:
  1. Alla prima occorrenza dell'interrupt del TIMER2 il pin del primo servo viene posto a livello logico alto, inoltre TIMER3 viene impostato con un tempo uguale alla durata dell'impulso del segnale PWM;
  1. All'occorrenza dell'interrupt del TIMER3 (da 1 a 2 ms dopo) il pin del servo viene posto a livello logico basso;
  1. All'occorrenza successiva del TIMER2 il ciclo ricomincia con il servo successivo.

Comandando ipoteticamente 8 servo, si ha un margine di 500us -> Tservo/Nservo = 20ms/8 = 2,5ms


Ecco le foto dei servo utilizzati:

[Servo1](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/servo1.jpg)
[Servo2](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/servo2.jpg)