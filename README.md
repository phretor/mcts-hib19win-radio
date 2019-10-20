# Mini Capture the Signal per HackInBo 2019 - Winter
Questo repository contiene istruzioni e firmware per preparare il trasmettitore usato nel Mini Capture the Signal per HackInBo 2019 - Winter.

Il trasmettitore trasmette un segnale che i partecipanti dovranno decifrare per passare da un livello all'altro del gioco. Inoltre, il trasmettitore si mette in modalità ricezione per qualche secondo, in attesa del segnale "giusto" da parte dei giocatori. A fronte di questo segnale, il trasmettitore trasmetterà l'ultimo "segreto" per concludere il gioco.

Per gestire il gioco vi servirà anche l'applicazione web: [https://github.com/phretor/mcts-hib19win-webapp](https://github.com/phretor/mcts-hib19win-webapp).

# Hardware
* Una **scheda di sviluppo** compatibile Arduino: io ho usato [questa](https://www.adafruit.com/product/3027) per comodità, perché volevo anche gestire il nodo via GPRS, ma una qualsiasi scheda va bene.
* Un **modulo radio** [RFM69HCW](https://www.hoperf.com/modules/rf_transceiver/RFM69HCW.html), a parte [come questo](https://www.adafruit.com/product/3230), o integrato nella scheda madre [come questa](https://www.adafruit.com/product/3077).

# Software
* [PlatformIO](https://platformio.org)

# Istruzioni
1. Assemblare l'hardware ed accertarsi delle sue funzionalità di base.
2. Collegarlo al computer ed assicurarsi che la porta seriale sia vista correttamente:
```bash
$ pio device list
... qui deve comparire una seriale nuova
```
3. Compilare il firmware:
```bash
$ git clone https://github.com/phretor/mcts-hib19win-radio
$ cd mcts-hib19win-radio
$ make
```
4. Caricare il firmware sulla scheda:
```bash
$ make upload
```
5. Controllarne il corretto funzionamento:
```bash
$ make monitor
```
