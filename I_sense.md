# Sensore corrente batteria #

Il sensore di corrente della batteria non è stato implementato poiché non essenziale al funzionamento del velivolo. Sarà probabilmente disponibile in una futura versione dell'aereo.
Il sensore di corrente ha la funzione di misurare il consumo istantaneo di corrente della batteria: questa misura, unita con quella della tensione consente di conoscere la potenza istantanea utilizzata.
Integrando la potenza è possibile quindi risalire all'energia utilizzata ed, in ultima analisi, a quella rimasta se si calcola approssimativamente la capacità della batteria.

```
 P = dW/dt   ->   ∫P dt = ∫dW   ->   W = P T


 dove:
  W è l'energia;
  P è la potenza;
  T è l'intervallo di tempo.
```

Quindi se ogni intervallo di tempo T calcolo la potenza P e la moltiplico per T medesimo (considerando P constante durante l'intervallo di tempo T) ho una buona stima di quanta energia è stata utilizzata.