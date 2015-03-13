# Ricavare l'inclinazione dalla velocità angolare #

Un giroscopio è un dispositivo che misura la velocità angolare di un corpo.
I giroscopi usati in situazioni critiche (come un aereo di linea) sono molto avanzati e complicati.
Fortunatamente per noi esistono alcune alternative piccole e a basso prezzo che li possono sostituire.
Sono fabbricati utilizzando la tecnologia MEMS (Micro Electro-Mechanical Systems) da grandi compagnie come la InvenSense.

Posizione, velocità e accelerazione sono collegati: derivando la posizione abbiamo la velocità:

`dx/dt = v t`

Può sembrare meno ovvio, ma è la stessa cosa con gli angoli. Mentre la velocità esprime quanto rapidamente cambia la posizione per unità di tempo, la velocità angolare esprime quanto rapidamente cambia un angolo nel tempo.


`⍵ = dϴ/dt`


![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/velocit%C3%A0_ang.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/velocit%C3%A0_ang.png)


Ottenere l'angolo è abbastanza semplice, basta integrare ambo i membri:

`∫ ⍵ dt = ∫ dϴ    ->    ⍵t = ϴ`



In realtà i giroscopi soffrono di un difetto chiamato “drift”, letteralmente deriva.
Questo significa che nel tempo, il valore letto da un giroscopio fermo (chiamato bias), tende a divergere dal suo valore iniziale.

![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/drift_gyro.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/drift_gyro.png)

Per avere un'idea di quanto influisce questo fenomeno, riportiamo qui sotto un grafico che mostra come la l'integrazione della velocità angolare è influenzata dal drift.
In 4500 campioni (il giroscopio è stato letto tramite un A/D Converter) che corrispondono a 12 secondi, l'errore è di circa 30 gradi.

Da questi dati viene da chiedersi cosa possiamo farcene di uno strumento che è affetto da un errore non quantizzabile, ricordiamo però che l'accelerometro non è affetto da deriva.