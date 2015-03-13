# GPS FV-M11 #

Il modulo FV-M11 è un ricevitore GPS con antenna integrata, caratterizzato da un'elevata sensibilità in grado di elaborare fino a 32 canali.
Questa versione è dotata di connettore con attacco MMCX per antenna esterna.

Questo GPS è programmabile in modo da selezionare le stringhe NMEA da trasmettere e a che frequenza. In particolare è possibile trasmettere i dati riguardanti la posizione fino ad un massimo di 5 volte al secondo.

Il GPS si interfaccia in seriale al nostro circuito, sulla UART2.

![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/fv-m11.jpg](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/fv-m11.jpg)
![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/Schermata-Mini%20GPS.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/Schermata-Mini%20GPS.png)

Il programma utilizzato per la configurazione è Mini GPS.

Come è possibile vedere in figura il GPS è configurato per aggiornare 5 volte al secondo le stringhe NMEA abilitate quali: RMC, GGA e ZDA.
Il baud rate è fissato a 57600.

Il protocollo NMEA 0183 (o più comunemente NMEA) è uno standard di comunicazione di dati utilizzato soprattutto in nautica e nella comunicazione di dati satellitari GPS. Questo protocollo si basa sul principio che la fonte, detta talker, può soltanto inviare i dati (sentences, stringhe) e la ricevente, detta listeners, può soltanto riceverli.

Tutte le stringhe hanno una struttura del tipo:
`$PREFISSO,dato1,dato2 ... datoN, *CHECKSUM <CR>< LF>`.
La stringa inizia sempre con `$` e termina sempre con `<CR>< LF>`. Ogni frase è lunga al massimo 80 caratteri.

Il prefisso è la prima parte della stringa, che serve a specificare di che tipo è il talker, ad esempio, autopilota, dispositivo GPS, controllo velocità, controllo direzione, ecc.
In caso dell'utilizzo di un dispositivo GPS, il prefisso è GP seguito dal tipo della frase. Tutte le frasi vengono identificate con 3 lettere (es: RMC,GGA, ecc.).

Il checksum viene calcolato escludendo il carattere di inizio stringa e il carattere **.
L'algoritmo usato è l' exclusive OR 8 bit, componendo il risultato in 2 lettere o numeri.**

Le stringhe sono in totale più di 50, la maggior parte proprietarie di ciascun dispositivo.

Le stringhe standard e più comuni del protocollo sono:
  * `$GPRMC`	→ Informazioni per la navigazione minime raccomandate
  * `$GPGGA` 	→ Global Positioning System Fix
  * `$GPGSA` 	→ Diluizione della precisione del GNSS e Satelliti attivi
  * `$GPGLL` 	→ Posizione geografica – Latitudine / Longitudine
  * `$GPGSV`  	→ Satelliti visibili
  * `$GPZDA` 	→ Ora e Data



Le stringhe NMEA, una volta interpretate e validate con il checksum vengono scritte nelle rispettive struct:

  * GGA
    * `time_utc		→ Orario UTC`
    * `latitude		→ Latitudine`
    * `nos_lat		→ N o S (Nord o Sud)`
    * `longitude		→ Longitudine`
    * `eow_long		→ E o W (Est o Ovest)`
    * `gps_quality	→ Indicatore qualità, 0 – fix non disp., 1 - GPS fix, 2 – GPS Differenziale`
    * `n_satellites	→ Numero di satelliti visibili, 00 - 12`
    * `hdop		→ Diluizione orizzontale della precisione`
    * `altitude_msl	→ Altezza sopra il livello del mare`
    * `meters1		→ Unità di misura dell'altezza, metri`
    * `altitude_wgs84	→ Separazione geoidale, differenza tra l'ellissoide WGS-84 e l'altezza MSL`
    * `meters2		→ Unità di misura delle separazione geoidale, metri`
    * `checksum		→ Checksum`

  * RMC
    * `time_utc		→ Orario UTC`
    * `status		→ Stato, V = Navigation receiver warning`
    * `latitude		→ Latitudine`
    * `nos_lat		→ N o S (Nord o Sud)`
    * `longitude		→ Longitudine`
    * `eow_long		→ E o W (Est o Ovest)`
    * `speed_overg_knots	→ Velocità al suolo, nodi`
    * `degrees_true	→ Direzione in gradi sessagesimali`
    * `date		→ Data, ggmmaa`
    * `deegres_magnvar	→ Variazione magnetica, gradi sessagesimali`
    * `eow_deg		→ E o W (Est o Ovest)`
    * `checksum		→ Checksum`

  * ZDA
    * `time_utc		→ Orario UTC`
    * `day		→ Giorno, 01 to 31`
    * `month		→ Mese, 01 to 12`
    * `year		→ Anno`
    * `hour_local		→ Ore zona attuale`
    * `min_local		→ Minuti zona attuale`
    * `checksum		→ Checksum`