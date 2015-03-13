# UART #

La gestione dell'UART è probabilmente la parte più complicata del progetto, nonché quella che ha impiegato il tempo maggiore.
L'UART, acronimo di Universal Asynchronous Receiver Transmitter permette di interfacciarsi in full-duplex con diverse interfacce, fra cui la RS-232 che utilizzeremo sia per ricevere i dati dal GPS sia per interfacciarsi con l'XBee.

Esistono 3 modi per comandare una seriale:
  * Polling
  * Interrupt
  * DMA

Il **polling** è il metodo più semplice, ma anche il meno efficace e il più sensibile agli errori.
In ricezione consiste nel controllare ciclicamente la presenza di caratteri nel buffer di ricezione ed eventualmente salvarli in un vettore, mentre in trasmissione, il processore resta utilizzato per tutto il tempo impiegato dalla seriale per inviare i dati.

Vantaggi:
  * Estrema semplicità di programmazione.

Svantaggi:
  * Elevato utilizzo di tempo processore;
  * Scarsa affidabilità.
Questo metodo è stato immediatamente scartato a causa dei suoi svantaggi.

L'**interrupt** è superiore rispetto al polling: consiste nell'avviare automaticamente, ogni 1, 2 o 4 caratteri ricevuti (a seconda delle impostazioni), una funzione di gestione che si occupa di memorizzare in vettori i dati ricevuti.
Sfruttando questo metodo il processore viene impegnato ad ogni carattere ricevuto, per tempi brevi.

Vantaggi:
  * Codice di gestione semplice;
  * Possibilità di analizzare ogni carattere appena ricevuto, semplificando moltissimo il riconoscimento e la gestione delle stringhe;
  * Le stringhe ricevute possono assumere qualsiasi lunghezza, senza dover modificare il codice.

Svantaggi:
  * Necessità di gestire accuratamente le priorità degli interrupt, per evitare di perdere caratteri;
  * I tempi della routine di interrupt devono essere minimi;
  * In progetti che utilizzano molti interrupt, con necessità di real time, questo metodo è difficile da implementare poiché la ridotta priorità comporterebbe inevitabili perdite di caratteri.


Il **DMA** (Direct Memory Access) risolve gli svantaggi del metodo interrupt, poiché il trasferimento dei dati dal buffer di ricezione alla RAM avviene su un BUS dati autonomo e su una memoria a doppio accesso DPSRAM (Dual Ported SRAM) e quindi senza rubare cicli al processore (Cycle-Stealing).

Il DMA del dsPIC utilizzato ha 8 canali, ognuno è configurato per agire in una sola direzione: vale a dire che un canale è occupato per leggere dal buffer di ricezione della seriale e scrivere sulla DPSRAM e uno è occupato per a leggere dalla DPSRAM e copiare del buffer di trasmissione della seriale.

Esistono svariate modalità di utilizzo del DMA, noi ne useremo solo due:
  * One-Shot
  * Ping-Pong

Nella modalità One-Shot, il canale DMA è settato con due parametri:
  * indirizzo della prima cella di memoria;
  * numero di caratteri da trasferire.

Dopo aver trasferito il blocco di caratteri, il DMA attiva un interrupt e il canale si disabilita fino al successivo utilizzo.
Va da se che la modalità **One-Shot** è usata praticamente solo per la trasmissione.
Per la trasmissione, il DMA è il metodo in assoluto migliore, utilizza il processore per tempi brevissimi e il trasferimento della stringa, anche lunga svariate centinaia di caratteri, essendo svolto via hardware è trasparente e non impegna il processore come nel caso dell'interrupt.

La modalità **Ping-Pong** utilizza due buffer, A e B. Quando il DMA ha finito di riempire il buffer A, scatta l'interrupt che ha il compito di cambiare il buffer su il quale il DMA scrive (da A a B) e di analizzare e salvare i dati contenuti nel buffer A.
Quando anche il buffer B è pieno, l'interrupt cambierà nuovamente il buffer di scrittura da B ad A (che ora conterrà dati vecchi già analizzati) e il ciclo si ripete.

Questa modalità è molto comoda per la lettura dei dati da seriale, dato che il processore è utilizzato solo al termine del riempimento di uno dei due buffer, che può essere lungo anche svariate centinaia di caratteri e non al massimo 4 come nel caso di lettura con semplice interrupt.
Purtroppo utilizzando il metodo Ping-Pong, si rischia di rimanere con una stringa a metà fra i due buffer, rendendo impossibile la lettura fino al riempimento del secondo buffer a meno di non utilizzare una codifica di canale ridondante; inoltre la ricomposizione delle stringhe richiede un algoritmo piuttosto complesso da implementare.

Vantaggi:
  * Ricezione via hardware dei dati senza impegno per il processore;
  * La routine di gestione del DMA viene chiamata solo al riempimento del buffer, con una frequenza molto minore rispetto al metodo interrupt, permettendo di essere gestita a bassa priorità;

Svantaggi:
  * L'algoritmo per la ricostruzione delle stringhe è complesso da implementare;
  * Necessità di utilizzare una codifica di canale ridondante per evitare ritardi nella ricezione delle stringhe;

All'inizio dello sviluppo si è scelto il metodo DMA, ma a causa dello scarso tempo a disposizione per sviluppare il progetto e la complessità del codice di gestione (solo per la parte di ricezione), si è passati al metodo dell'interrupt, vista la semplicità del codice. In una futura versione del progetto verrà probabilmente reintrodotto il metodo DMA.