# A/D Converter #

La conversione Analogico/Digitale sul dsPIC è realizzata da un unico convertitore Analogico/Digitale di tipo SAR, il cui ingresso è multiplexato fra tutti i pin analogici.
Lo schema qui sotto rappresenta sinteticamente il funzionamento del modulo ADC di un dsPIC33F.

![http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/schema_adc.png](http://uavfirmware.googlecode.com/svn/trunk/Docs/immagini/schema_adc.png)


Il modulo ADC è formato da 4 canali, ogni canale raggruppa un certo numero di pin analogici:
  * CH0 contiene tutti i pin, da AN0 a AN12;
  * CH1 contiene i pin AN1, AN4, AN6, AN9;
  * CH2 contiene i pin AN2, AN5, AN7, AN10;
  * CH3 contiene i pin AN3, AN6, AN8, AN11;

La tensione del pin analogico convertito dall'A/D può essere o riferita a massa, oppure riferita alla tensione di un altro pin analogico, ottenendo in questa seconda modalità, una lettura differenziale; come si può notare dallo schema.
Ogni canale può acquisire un solo pin analogico alla volta, questo perché ogni canale ha a disposizione un solo S/H (Sample and Hold).

Esistono diverse modalità di funzionamento del modulo ADC, alcune di queste possono essere anche molto complesse, per semplicità ci occuperemo solo della modalità “singola acquisizione e singola conversione”, da noi utilizzata.

Una volta configurati i registri per la modalità “singola acquisizione e singola conversione”, per ottenere una lettura è quindi necessario:
  * Selezionale il canale corretto, lo 0 nel nostro caso;
  * Avviare il campionamento, ora il S&H memorizza la tensione letta;
  * Avviare la conversione della tensione memorizzata dal S&H;
  * Memorizzare il valore convertito.

Il valore convertito, che sia del giroscopio, della batteria o del sensore di corrente, è ora disponibile per l'elaborazione.