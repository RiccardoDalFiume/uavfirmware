# Sensore di tensione #

Il sensore di tensione della batteria ha la funzione di misurare la tensione della batteria con la quale è alimentata l'elettronica.
Conoscere la tensione è molto importante:
per prevenire uno spegnimento prematuro di madrescheda;
per evitare di danneggiare la batteria al LiPo sottoponendola ad una scarica eccessiva;

Per effettuare la misurazione abbiamo utilizzato uno dei convertitori A/D del dsPIC, collegato ad un semplice partitore senza usare alcun Op-Amp, dato che la corrente in ingresso all'A/D è molto bassa.

Nella figura a fianco, in alto a sinistra, si può notare il partitore con le due resistenze all'1% di precisione e il condensatore, necessario per ridurre eventuali spike o rumori durante la lettura dell'A/D.


<img src='http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/Partitore_batteria.png' alt='Schematics' width='522' height='286' />

