## Organizzazione della Repository ##

I vari moduli del programma sono stati divisi in cartelle, almeno durante tutto lo sviluppo della prima alpha.
La scelta è stata fatta per poter lasciare intatto e compilabile il core della centralina.


## Le cartelle in dettaglio ##

Nella cartella "trunk" sono presenti i file che seguono lo sviluppo standard.
Qui sotto
```
  trunk
   `- Circuit                     // Contiene la versione corrente del circuito
   |   `- CAM                     // Contiene i file *.cam per la realizzazione dei gerber
   |   `- Circuit_D               // Contiene lo schematico e lo sbroglio di Madrescheda
   |   `- Librerie                // Contiene le librerie con i componenti non standard
   `- Docs                        // Contiene la tesina e le mappe mentali
   |   `- immagini                // Contiene le immagini della tesina
   `- Software                    // Contiene il codice del progetto
   |   `- UAVFirmware             // Contiene il codice di UAVFirmware
   |   |   `- dev                 // Contiene il codice di configurazione hardware
   |   |   `- include             // Contiene le librerie per l'utilizzo dell'hardware
   |   |   `- nav                 // Contiene i file del motore di navigazione
   |   `- UAVMainframe_LabView    // Contiene i file di LabView di UAVMainframe
   |   `- prove                   // Contiene alcune prove di codice
   |       `- comm                // Contiene prove di comunicazione seriale in UNIX
   |       `- kml                 // Contiene un assemblatore di file *.kml
   |       `- terminal            // Contiene le prime prove del terminale
```