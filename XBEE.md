# xBee Pro #

<img src='http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/xbee_pro.jpg' alt='Schematics' width='250' height='202' />
<img src='http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/xbee_simple_board.jpg' alt='Schematics' width='250' height='193' />
<img src='http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/xbee_usb_board.jpg' alt='Schematics' width='250' height='219' />


L'UAV scambia con la stazione di terra una quantità di dati riguardanti la sua posizione e le impostazioni dell'autopilota.
Nella stazione di terra, il programma ha le seguenti funzioni:
  * Ricevere dall'aereo informazioni sullo stato della missione, come la posizione, la velocità e l'altezza dal suolo
  * Ricevere dall'aereo informazioni sullo stato dell'autopilota, come la velocità di crociera, l'altezza dal suolo e i Way Point (punti di navigazione).
  * Ricevere eventuali avvertimenti o allarmi, come la segnalazione di Way Point raggiunto o l'allarme bassa quota.

Il protocollo utilizzato per lo scambio di dati fra UAV e il computer è stato sviluppato prendendo spunto da quello NMEA; principalmente perché il software è già stato sviluppato per il GPS ed è sembrato insensato sviluppare un altro interprete per un nuovo protocollo.

Le stringhe cambiano di significato a seconda di chi le invia. Se è la stazione di terra che le invia sono intese come impostazioni, mentre se è l'autopilota ad inviarle allora sono intese come informazioni di stato.

Nota: Le stringhe incominciano con `$APxxx` se sono inviate dall'UAV e con `$GSxxx` se sono inviati da terra.

NAV contiene le informazioni sulla posizione del velivolo
`$NAV,time_utc,degrees_true,latitude,nos_lat,longitude,eow_long,speed_overg_kmh,alt_meters*CS\r\n`

  * `time_utc			-> Orario UTC`
  * `degrees_true		-> Direzione in gradi reali`
  * `latitude			-> Latitudine`
  * `nos_lat			-> N or S (Nord o Sud)`
  * `longitude			-> Longitudine`
  * `eow_long			-> E o W (Est o Ovest)`
  * `speed_overg_kmh		-> Velocità al suolo in Km/h`
  * `alt_meters			-> Altezza dal suolo in metri`
  * `CS				-> Checksum XOR a 8 bit`


PLT contiene le informazioni sullo stato dell'autopilota

`$AUTO,time_utc,alt_meters_or_bool,speed_kmh,wp_follow_bool,wp_lat,wp_NoS,wp_long,wp_EoW*CS\r\n`

  * `time_utc			-> Orario UTC`
  * `alt_meters_or_bool		-> Se il valore è 0 ALT è disattivato, altrimenti rappresenta i metri di ALT`
  * `speed_kmh			-> Velocità di crociera impostata`
  * `wp_follow_bool		-> L'autopilota è attivo (1) o disattivo (0)`
  * `wp_lat			-> Latitudine del Way Point`
  * `wp_nos			-> N or S (Nord o Sud)`
  * `wp_long			-> Longitudine del Way Point`
  * `wp_eow			-> E o W (Est o Ovest)`
  * `CS				-> Checksum XOR a 8 bit`


ALM contiene eventuali allarmi lanciati dall'aereo

`$ALARM,time_utc,wp_arrived_bool,alt_meters_bool*CS\r\n`

  * `time_utc			-> Orario UTC`
  * `wp_arrived_bool		-> L'UAV è arrivato al Way Point`
  * `alt_meters_bool		-> Allarme quota`
  * `CS				-> Checksum XOR a 8 bit`