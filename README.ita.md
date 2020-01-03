Progetto SeismoCloud: http://www.seismocloud.com

[![Build Status](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu.svg?branch=master)](https://travis-ci.org/sapienzaapps/seismoclouddevice-nodemcu)

# Schede supportate

* NodeMCU 1.0 devkit (con modulo ESP-12E - ESP8266)

# Requisiti di rete

Il dispositivo acquisirà tutte le configurazioni IPv4 (indirizzo, maschera di rete, gateway, DNS) tramite DHCP.

Se si dispone di firewall nella rete, si prega di consentire queste porte (in uscita, verso Internet):

* TCP: 443, 1883

# Descrizione dello stato del LED

I LED possono trovarsi in questi stati diversi (celle vuote significa "OFF"):

| Verde | Giallo | Rosso | Stato
|:-----:|:------:|:-----:| ------
| ON    |        |       | Il dispositivo è inattivo e in ascolto
| ON    | ON     |       | Il dispositivo ha perso la connessione e sta tentando di riconnettersi
| ON    |        | ON    | È stata rilevata una vibrazione / vibrazione!
| ON    | ON     | ON    | Il dispositivo si sta collegando alla rete SeismoCloud
|       | ON     | ON    | Verifica di aggiornamenti o aggiornamenti
|       | ON     |       | Connessione alla rete Wi-Fi *
|       |        | ON    | Calibrazione in corso

Alla fine della sequenza di avvio, tutti e tre i LED lampeggeranno rapidamente per segnalare che è OK.

*: Se solo il LED giallo è acceso per più di 10 secondi, NodeMCU è in attesa di
configurazione della rete Wi-Fi (vedere il capitolo "Come caricare il software")

# Come costruire il dispositivo (hardware)

## Requisiti

* Rete Wi-Fi (WPA-PSK, WPA2-PSK, aperta)
* Arduino / Genuino IDE con ESP8266 sdk installato. Se non hai ESP8266 sdk:
  * Apri la finestra *Preferenze* (dal menù *File*)
  * Inserisci `http://arduino.esp8266.com/stable/package_esp8266com_index.json` nel campo *URL Board Manager aggiuntivi*. Puoi aggiungere più URL, separandoli con virgole.
  * Chiudi con "OK", apri *Gestione schede* dal menù *Strumenti* > *Scheda* e installa la piattaforma *esp8266* (e non dimenticare di selezionare la scheda *NodeMCU 1.0 (ESP-12E)* dal menù *Strumenti* > *Scheda* dopo l'installazione).
* Librerie IDE Arduino / Genuino: `WiFiManager, PubSubClient`
  * Apri *Includi librerie* dal menù *Sketch* e scegli *Gestisci librerie*
  * Digitare `WiFiManager` nella barra di ricerca, quindi installarlo facendo clic sul pulsante *Installa* in basso; lo stesso per `PubSubClient`
  * Chiudi la finestra
* Scheda devkit NodeMCU 1.0 con modulo ESP-12E
* Accelerometro MPU6050
* (opzionale) 3 LED (rosso-verde-giallo) con 3 resistori

Testato con `Arduino / Genuino IDE 1.8.10`, scheda SDK` esp8266 2.6.2`, librerie `PubSubClient 2.7` e` WiFiManager 0.15.0-beta`

## Cablaggio dell'accelerometro MPU6050

Collegare questi pin dall'accelerometro MPU6050 alla scheda NodeMCU:

* 3v3: 3v3
* GND: GND
* SDA: D1
* SCL: D2

## Cablaggio LED

Ricorda di mettere un resistore con LED (dopo / prima non è davvero importante) da limitare
corrente che scorre, altrimenti si potrebbe danneggiare la scheda NodeMCU.

Per impostazione predefinita, i pin LED sono:

* Pin D5: verde
* Pin D6: giallo
* Pin D7: rosso

# Come caricare il software

Per NodeMCU, devi scaricare la libreria `WifiManager` (usando *Sketch > includi libreria > gestore libreria*)

1. Scarica il codice sorgente (per versioni stabili, controlla l'ultimo tag git)
2. Aprire il progetto in Arduino IDE
3. Scegli i giusti valori per le opzioni **Porta** e **Scheda** nel menù **Strumenti** (se non sei sicuro, usa `NodeMCU 1.0`)
4. Compilare e caricare (secondo pulsante sotto i menù) nella scheda
5. Connettersi alla rete Wi-Fi `SeismoCloud` e configurare i parametri di rete del client Wi-Fi. Al momento del salvataggio, la scheda si riavvia e proverà a connettersi alla rete Wi-Fi. Se fallisce, puoi riconnetterti alla rete `SeismoCloud` e modificare / correggere i parametri di rete.
6. Apri l'app SeismoCloud, connettiti alla stessa rete della scheda e registra il tuo dispositivo!

# FAQ

## Il mio dispositivo ha solo il LED giallo acceso per più di 10 secondi

Il sensore non è riuscito a connettersi alla rete Wi-Fi. Seguire le istruzioni nel capitolo "Come caricare il software" passaggio 5

## Sono connesso alla rete Wi-Fi, ma non appare alcun portale di configurazione.

Prova ad aprire un browser e vai a http://192.168.4.1. Se il portale Wi-Fi non viene ancora visualizzato, scollegare la scheda da
la fonte di alimentazione per pochi secondi. Se il problema persiste, eseguire nuovamente il flashing della scheda cancellando la configurazione Wi-Fi utilizzando l'opzione relativa dal menù **Strumenti**.

# Licenza

Vedi il file `LICENSE`
