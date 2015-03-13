# Ricavare l'inclinazione dall'accelerazione #

Come il nome lascia ad intendere, un accelerometro misura l'accelerazione lungo un asse predefinito, perciò con un accelerometro possiamo misurare l'accelerazione di gravità.

![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/inclinazione_accelerometro.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/inclinazione_accelerometro.png)

La freccia rossa rappresenta l'accelerazione di gravità della terra, quella blu mostra come l'accelerometro la misura.
Notare che l'asse di questo accelerometro è perpendicolare al velivolo (ovviamente, perchè è posizionato all'interno di esso).


L' angolo ϴ tra l'accelerazione misurata e quella reale è collegato al “pitch” (beccheggio) del velivolo.

`pitch = ϴ +90°`

Se noi conosciamo ϴ, possiamo quindi ricavare il pitch; dato che noi conosciamo con buona precisione l'accelerazione gravitazionale terrestre, un semplice calcolo ci permette di ottenere il nostro angolo di pitch.


`a_misurata = cos(ϴ)⋅g    ->    ϴ = acos[(a_asse-z)/g]`

Siccome → `pitch = ϴ +90°    ->    pitch = asin[(a_asse-z)/g]`

Abbiamo ora calcolato il pitch del nostro aereo utilizzando un accelerometro, calcolare il “roll” (rollio) è praticamente la stessa cosa, ma utilizzando l'asse perpendicolare a quello usato per il pitch.

Purtroppo la realtà è un po' differente dall'esempio semplificato qui sopra.
La funzione inversa del seno non può restituire tutti i 360° che l'angolo di pitch può assumere:
un velivolo con la punta verso il cielo e uno con la punta verso terra darebbero entrambi lo stesso angolo di pitch 0 (zero).
Per porre rimedio abbiamo bisogno di un asse in più per distinguere i due casi.
La tangente inversa a due argomenti ci assicura che l'angolo risultante sia nel quadrante corretto.
Quindi:

`pitch = atan2[(a_asse-y/g),(a_asse-z/g)]`

Nota:
In trigonometria la funzione a due argomenti atan2 rappresenta una variazione dell'arcotangente.
Comunque presi gli argomenti reali x e y non nulli, atan2(x,y) indica l'angolo in radianti tra l'asse positivo delle x in un piano e un punto di coordinate (x,y) giacente su di esso.
L'angolo è positivo se antiorario e negativo se in verso orario.
Questa funzione quindi restituisce un valore compreso nell'intervallo (-π,+π). La funzione è definita per tutte le coppie di valori reali (x,y) eccetto la coppia (0,0).

Ora i problemi sembrerebbero tutti risolti, purtroppo l'accelerazione gravitazionale non è l'unica ad agire sul nostro velivolo. Basti pensare alla forza centripeta che si ottiene durante una virata.
Sono perciò necessari dei giroscopi per correggere questo errore nel breve periodo (utili anche per eliminare gli effetti delle vibrazioni nell'accelerometro).
Su un periodo di tempo più lungo, abbiamo bisogno di una fisica più complessa per stimare queste forze e compensare la misura.