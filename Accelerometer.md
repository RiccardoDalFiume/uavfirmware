# Accelerometro MMA 7455L #

![http://uavfirmware.googlecode.com/svn-history/r427/trunk/Docs/immagini/acc_MMA7455L.png](http://uavfirmware.googlecode.com/svn-history/r427/trunk/Docs/immagini/acc_MMA7455L.png)

L'MMA 7455L è un accelerometro MEMS a 3 assi prodotto dalla Freescale esclusivamente con package LGA-14, utilizzato in ambito industriale in catena di montaggio, non progettato per uso hobbistico.
Basti pensare alle sue dimensioni, 3mm x 5mm x 1mm, per avere un idea di quanto sia difficile stagnare un oggetto del genere.

Per ovviare a questo problema, abbiamo utilizzato un comodo prodotto di Futura Elettronica, un piccolo stampato con l'accelerometro già stagnato e tutti i componenti esterni necessari, interfacciabile tramite una strip con passo da 100 mils.

Questo modulo integra già le resistenze di pull-up del bus I2C, pertanto non le abbiamo aggiunte in madrescheda.

Il modulo opera ad una tensione di 3,3V, pari ai livelli di tensione del bus, quindi, come indicato nelle istruzioni, abbiamo chiuso il ponticello J1.

E' in grado di misurare accelerazioni fino a 8g, con una scala selezionabile tramite software a ±2, ±4 e ±8g, per ognuno dei 3 assi.