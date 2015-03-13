# Punto di partenza #

Il modello usato è un Easy star.

![http://easystar.altervista.org/Easystar.jpg](http://easystar.altervista.org/Easystar.jpg)

La tipologia di UAV che abbiamo progettato è costruito sulla base di un comune aereo da modellismo che originariamente era composto dalla seguente attrezzatura: l'aereo è di modeste dimensioni e adatto ai neofiti per la sua elevata portanza e stabilità; la spinta è fornita da un'elica quadripala azionata da un motore “brushless” alimentato da una batteria Li-Po.

Il primo grosso vantaggio dei motori brushless riguarda la vita attesa del motore, dato che le spazzole sono il "punto debole" di un motore in corrente continua. L'assenza di spazzole elimina anche la principale fonte di rumore elettromagnetico presente negli altri motori in continua che pregiudicherebbe il corretto funzionamento della strumentazione di volo.

Le batterie Li-Po (Polimeri di Litio) si differenziano dalle litio-ione per la minore pericolosità per fatto che il polimero solido non è infiammabile (a differenza del solvente organico che le cellule a Li-Ion utilizzano), inoltre le Li-Po hanno una bassissima resistenza interna che consente l'erogazione di grandi correnti e di conseguenza grandi potenze (centinaia di Watt).
Dal momento che hanno un impacchettamento più denso senza spazi tra le celle cilindriche e senza contenitore, la densità energetica delle batterie Li-Po è maggiore di più del 20% rispetto ad una Litio-Ione classica ed è circa tre volte migliore rispetto alle batterie NiCd (Nichel-Cadmio) e NiMH (Nichel-Metal Idrato).


Le parti mobili di pilotaggio, necessarie per poter controllare il moto dell'aereo sono:
  * Timone direzionale
  * Timone di profondità
  * Alettoni
Esse sono azionate da dei comuni servo-attuatori da modellismo, uno per il “Timone direzionale”, uno per il “Timone di profondità” e due per gli “Alettoni”.
La potenza del motore, così come la posizione dei servo, è comandata in PWM (Pulse Width Modulation), perciò ai fini del progetto considereremo il motore come un servo-attuatore.

Durante il volo manuale, l'aereo è pilotato attraverso un radiocomando Futaba FC-18 v3, il quale è dotato di ingressi digitali che torneranno utili per cambiare il pilotaggio da “Automatico” a “Manuale” da terra.


# Cosa è stato aggiunto #

Un UAV deve poter comunicare con la base a terra per riferire il suo stato di missione e la sua posizione, perciò nell'equipaggiamento sono indispensabili:
IMU, Unità di misurazione inerziale
Modulo di comunicazione

Qualora l'UAV perda i segnali provenienti dalla stazione di guida a terra esso deve autonomamente portarsi verso un luogo sicuro oppure orbitare su una zona definita e tentare un atterraggio d'emergenza una volta esaurita la batteria.

Nel progetto, i componenti summenzionati fanno parte di un unico circuito stampato auto-costruito che contiene tutta la logica di controllo; questo stampato è il cuore di tutto il sistema.
Per ragioni di semplicità d'ora in poi esso verrà chiamato con il suo nome di progetto, cioè “Madrescheda”.


[Radiocomando Futaba FC-18 ](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/radio1.jpg)

[Ricevente a 8 canali posizionata a bordo dell'aereo](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/ricevente1.JPG)

### Specifiche del modello ###

| Wing Length | 54 inches | 1,3716 meters |
|:------------|:----------|:--------------|
| Wing Area | 372 square inches | 0,24 m^2|
| Wing Loading | 10.76 oz. / sq. ft. | 3,28 kg/m^2 |
| Fuselage Length | 34 in. | 0,8636 meters |
| Weight (English) | 24 oz. | 680 grams |
| Flight Characteristics | Trainer |
| Target User | Beginner |

### Equipaggiamento utilizzato ###

| Motor | 190 watt 28mm Outrunner |
|:------|:------------------------|
| Speed Control | 17 amp |
| Propeller | 5.5 x 4.5 |
| Battery | 3S 1750 mAh LiPo |
| Servos | (2) HS-82HB or (2) HS-85BB |
| Receiver | Mini 6S |