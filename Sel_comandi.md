# Selezione comandi MAN/AUTO #

La possibilità di selezionare chi pilota l'aereo (radiocomando o autopilota) è una funzione critica e assolutamente indispensabile sia durante le fasi di decollo e atterraggio, durante le quali l'aereo è pilotato manualmente, sia durante il debug del software di navigazione se si dovessero verificare comportamenti imprevisti, in modo da evitare brutti incidenti.
Per essere sicuri di poter assumere i comandi anche in caso di guasto alla logica di controllo, abbiamo ideato un circuito “a monte” dell'autopilota in grado si scollegare elettricamente i servo dall'autopilota e comandarli con i comandi ricevuti dalla ricevente.

Concettualmente, il circuito multiplexa i comandi della radio e dell'autopilota, selezionando cosa trasmettere ai servo, in base ad un segnale inviato dalla radio-trasmittente a terra.
La radio-trasmittente da noi impiegata (Futaba FC-18) supporta fino a 8 canali tra analogici e digitali: ogni servo utilizza un canale, per un totale di 5, quindi, sfruttando un canale digitale della radio, comandiamo il circuito di selezione dei comandi con uno dei 3 canali restanti.

Come per i servo, l'informazione è trasmessa in PWM, pertanto lo 0 logico del segnale digitale corrisponde ad un impulso di 1 ms, mentre l'1 logico ad un impulso di 2 ms.
Il circuito a bordo di “madrescheda” decodifica l'informazione, sfruttando un monostabile ed un Flip-Flop D, il quale comanda i selettori dei multiplexer utilizzati per fornire ai servo i segnali di controllo.
Il monostabile genera un impulso di riferimento a 1,5 ms (tempo intermedio fra i due stati quindi con minor probabilità di errore), al termine dei quali il flip-flop D riporta in uscita (ai selettori dei mux) il valore di tensione del canale.

<img src='http://uavfirmware.googlecode.com/files/Sel_comandi.png' alt='Schematics' width='920' height='234' />

Riassumendo:
  * il monostabile (triggerato sul fronte di salita e uscita normale alta) porta l'uscita a 0V per 1,5 ms;
  * Il Flip-Flop D è triggerato sul fronte di salita, pertanto, dopo 1,5 ms l'uscita del monostabile si alza e il FF D memorizza e applica in uscita il livello logico che ha in ingresso.

http://uavfirmware.googlecode.com/files/TEK00021.PNG

Se la radio invia uno 0 logico l'impulso è di 1 ms; dopo 1,5 ms il segnale è quindi a 0 V.
Pertanto il FF D memorizza il livello, ed i pin di selezione dei mux sono comandati con un livello logico basso.

Analogamente se la radio invia un 1 logico, corrispondente ad un impulso di 2 ms, dopo 1,5 ms il flip-flop D rileva un livello logico alto ed i pin di selezione dei mux sono comandati con un livello logico alto.