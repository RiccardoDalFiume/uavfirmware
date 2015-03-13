# Attenuazione del rumore bianco tramite buffer circolare #

Il rumore bianco è molto frequente in elettronica poiché è prodotto, fra le altre cose, dalle resistenze.
Non c'è da stupirsi quindi se le misure dell'accelerometro e del giroscopio sono largamente affette da questo tipo di rumore.

Il rumore bianco ha le seguenti caratteristiche:
  * ha la stessa potenza a tutte le frequenze;
  * ha un vettore medio nullo.

Il rumore bianco che affligge l'accelerometro è causato principalmente da due elementi:
  * imperfezioni di costruzione;
  * vibrazioni dovute al movimento dei servo, al motore, ma soprattutto all'attrito dell'aria.

Allo stesso modo anche il giroscopio è affetto da rumore bianco, a causa di:
  * imperfezioni di costruzione;
  * disturbi del segnale analogico convertito dall'A/D Converter;
  * oscillazioni dovute al movimento dei servo, al motore, ma soprattutto all'attrito dell'aria.

In entrambi i casi è possibile contenere l'influenza del rumore bianco sulla misura, per esempio, facendo la media del maggior numero di valori possibili; generalmente sono sufficienti da 8 a 32 valori.
Per il giroscopio è sufficiente una media di 8 valori, mentre per l'accelerometro ne utilizzeremo 64 a causa dell'elevata rumorosità.

**NOTA:** i numeri scelti sono potenze di 2 poiché durante il calcolo della media l'operazione di “shift” all'interno del processore avviene più rapidamente di una normale divisione.

Per il giroscopio, considerata l'elevata velocità di acquisizione del convertitore A/D, è sufficiente una semplice media di 8 valori per avere una lettura stabile.

Purtroppo utilizzare un numero elevato di valori per il calcolo di una media può essere limitante, come nel caso dell'accelerometro.
Dato che la frequenza massima di lettura dell'accelerometro è 250Hz, ottenere un valore “pulito” impiegherebbe 64 volte il tempo di una singola lettura, con una frequenza di aggiornamento ridotta a 4 Hz (250Hz/64), che renderebbe inutilizzabile la misura.

Per risolvere questo problema viene utilizzato un buffer circolare, simile ad un FIFO.
Il buffer circolare, a differenza del FIFO, non sposta tutti i suoi valori ad ogni inserimento, ma si limita a sovrascrivere la cella di memoria del valore più vecchio con il nuovo.
Dopo aver inserito il nuovo valore, viene calcolata la media di tutto il buffer, così facendo riusciamo ad ottenere una misura “pulita” senza ridurre la frequenza di aggiornamento.