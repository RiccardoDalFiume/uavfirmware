# BUS I2C #

Acronimo di **Inter Integrated Circuit** (pronuncia i-quadro-ci o i-due-ci), un sistema di comunicazione seriale bifilare utilizzato tra circuiti integrati.
Il classico bus I2C è composto da almeno un master ed uno slave (letteralmente "capo, padrone" e "sottoposto, schiavo").

Il protocollo hardware dell'I2C richiede due linee seriali di comunicazione:
  * SDA (Serial DAta line) per i dati
  * SCL (Serial Clock Line) per il clock (per la presenza di questo segnale l'I2C è un bus sincrono)

Va aggiunta una connessione di riferimento detta, talvolta impropriamente, GND e una linea di alimentazione Vdd a cui sono connessi i resistori di pull-up che può anche non essere condivisa da tutti i dispositivi.

L'I2C ha 7 bit di indirizzo (B1 è il bit più significativo, B7 il meno significativo) e quindi 128 possibili indirizzi diversi (detti nodi). Di questi però 16 sono riservati quindi i dispositivi che possono essere collegati sullo stesso bus sono al massimo 112.
Le velocità di trasmissione nel modo standard sono di 100 kbit/s e 10 kbit/s velocità del low-speed mode ma nulla impedisce di scendere a velocità più basse.
Revisioni del I2C hanno introdotto dispositivi con velocità di 400 kbit/s (detto Fast Mode) e 3,4 Mbit/s (detto High Speed mode). Anche le possibilità di indirizzamento di dispositivi sono state ampliate a 10 bit.


l master inizia lo scambio di informazioni inviando lo start bit (S) seguito dall'indirizzo dello slave con cui vuole comunicare (B1, ..., B7). Segue un bit (B8) che indica se vuole trasferire informazioni allo slave (scrivere, ammesso che il dispositivo permetta questa possibilità) o ricevere informazioni (leggere).
Nel primo caso il bit B8 tenuto basso dal master nel caso voglia ricevere informazioni rilascerà la linea dati (alto per la presenza del pull-up).
Se lo slave indirizzato (B1, ..., B7) esiste prende il controllo della linea dati sul successivo impulso alto del SCL e la forza bassa (ACK).
Il master sa quindi che il dispositivo selezionato ha ricevuto la richiesta ed è in attesa di rispondere.

Gli indirizzi e i dati per convenzione del bus sono trasmessi iniziando dal bit più significativo (B1) e terminando con il bit meno significativo.

Lo start e lo stop seguono le indicazioni della figura:
  * lo start (S) è costituito da una transizione da alto a basso del bus dati SDA mentre il clock (SCL) è alto;
  * lo stop (P) è rappresentato da una transizione da basso ad alto del SDA mentre il SCK è alto.

Se il master vuole ricevere informazioni da uno slave ad ogni byte ricevuto invia un ACK meno l'ultimo byte.
Dopo questo può inviare uno STOP bit (P) o uno START bit (S) se vuole mantenere il controllo del bus per un altro trasferimento.

http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/osc/TEK00025.PNG