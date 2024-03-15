/*
  FunkmodulClass.cpp

  Copyright (c) 2024, SD

*/
#include "Funkmodul.h"

// Für Funk-Modul
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define FUNKM_DEFAULT_CE_PIN  11 //14 //A0
#define FUNKM_DEFAULT_CSN_PIN 10 //15 //A1

const byte slaveAddress[][5] = {{'R', 'x', 'A', 'A', 'A'}, {'R', 'x', 'A', 'A', 'B'},
    {'R', 'x', 'A', 'A', 'C'}, {'R', 'x', 'A', 'A', 'D'}};

bool fm_connections_ok[] = { false, false, false, false };

RF24 fm_rf24_radio; // Create a Radio

char dataToSend[10] = "756      ";
char dataReceived[10]; // this must match dataToSend in the TX
char txNum = '0';
int ackData[2] = {-1, -1}; // to hold the two values coming from the slave
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second
// Für Funk-Modul -ENDE-

int curCE_PIN = FUNKM_DEFAULT_CE_PIN;
int curCSN_PIN = FUNKM_DEFAULT_CSN_PIN;

FunkmodulClass::FunkmodulClass()
{
  // Default-Constructor
  fm_rf24_radio = RF24(curCE_PIN, curCSN_PIN, RF24_SPI_SPEED);
  fm_connection_ok = false; // erstmal die Verbindung als nicht ok kennzeichnen (wird ok nach dem ersten erfolgreichen Senden)
}

FunkmodulClass::FunkmodulClass(int ce_pin, int csn_pin)
{
  curCE_PIN = ce_pin;
  curCSN_PIN = csn_pin;
  fm_rf24_radio = RF24(curCE_PIN, curCSN_PIN, RF24_SPI_SPEED);
  fm_connection_ok = false; // erstmal die Verbindung als nicht ok kennzeichnen (wird ok nach dem ersten erfolgreichen Senden)
}

FunkmodulClass::~FunkmodulClass()
{
    //delete _display;
}

void FunkmodulClass::init(bool read, bool write, int receiverNr)
{
  // analoge Pins auf digital umstellen
 // pinMode(FUNKM_DEFAULT_CE_PIN, INPUT_PULLUP);
 // pinMode(FUNKM_DEFAULT_CSN_PIN, OUTPUT);
  // Für Funk-Modul
  fm_rf24_radio.begin(curCE_PIN, curCSN_PIN);
  fm_rf24_radio.setDataRate( RF24_250KBPS );
  fm_rf24_radio.enableAckPayload();
  fm_rf24_radio.setRetries(5,5); // delay, count 
  // 5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload



  if (write) {
    // Sender intitialisieren
    // NEU: 11.03.24 /sd
    int numSlaves = 4;  // max. 4 Empfänger
    fm_rf24_radio.enableDynamicPayloads() ;
    fm_rf24_radio.setAutoAck( true ) ;
    //fm_rf24_radio.openWritingPipe(slaveAddress);
    // NEU: 11.03.24 /sd -ENDE-
    if (!read) {
        fm_rf24_radio.stopListening();
    }
  }
  if (read) {
    // Empfaenger initialisieren

    fm_rf24_radio.openReadingPipe(receiverNr, slaveAddress[receiverNr]);
    fm_rf24_radio.enableAckPayload();
    // NEU: 11.03.24 /sd
    fm_rf24_radio.enableDynamicPayloads() ;
    fm_rf24_radio.setAutoAck(true);
    // NEU: 11.03.24 /sd -ENDE-

    fm_rf24_radio.startListening();
  }


/*    if (isValidFunkmodul()) {
        //auto constructor = display_types[_display_type];
        //_display = constructor(reset, clk, data, cs);
        //_display->begin();
        //setContrast(DISPLAY_CONTRAST);
        //setStatus(true);
        //_diagram.init(scheduler, _display);

        scheduler.addTask(_loopTask);
        _loopTask.setCallback(std::bind(&FunkmodulClass::loop, this));
        _loopTask.setIterations(TASK_FOREVER);
        _loopTask.setInterval(_period);
        _loopTask.enable(); 
    }
    */
}


bool FunkmodulClass::sendInteger(int zahl)  {
  Serial.print("Funkmodul.sendInteger: ");
  String str = String(zahl) + '\n';

  int str_len = str.length() + 1; 
  char dataToSend[str_len];
  str.toCharArray(dataToSend, str_len);

  fm_connection_ok = false; // Verbindung erstmal auf Falsch setzen => ok, wenn mind. 1 Receiver antwortet
  bool rslt;
  // NEU 11.03.2024 /sd
  for (byte n = 1; n < 3; n++) {  
    rslt = fm_rf24_radio.write( &dataToSend, sizeof(dataToSend) );
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt) {
        if ( fm_rf24_radio.isAckPayloadAvailable() ) {
            fm_rf24_radio.read(&ackData, sizeof(ackData));
            newData = true;
            fm_connection_ok = true; // es ist eine Verbindung ok
            fm_connections_ok[n] = true; // aktuelle Verbindung als ok markieren
        }
        else {
            Serial.println("  Acknowledge but no data ");
            fm_connections_ok[n] = false; // aktuelle Verbindung als NICHT ok markieren
        }
        //updateMessage();
    }
    else {
        Serial.println("  Tx failed");
        fm_connections_ok[n] = false; // Diese Verbindung als NICHT ok markieren
        // fm_connection_ok = false; // aktuelle Verbindung als nicht mehr ok kennzeichnen
    }
  }
  prevMillis = millis();
  delay(100);
  return fm_connection_ok;
}


bool fm_data_available = false;
int  fm_buffer = 0;



void FunkmodulClass::loop()  {
  if ( fm_rf24_radio.available() ) {
    fm_rf24_radio.read( &dataReceived, sizeof(dataReceived) );
    Serial.println((int)dataReceived[0]);
    if ((int)dataReceived[0] > 0) {
      Serial.print("Send Reply ");
      updateReplyData();
      Serial.print("Data received: ");
      Serial.println(dataReceived);
      //Data = String(dataReceived);
      Serial.println("Daten per Funk");
      fm_data_available = true;
      fm_buffer = atoi(dataReceived);
      delay(10);
    }
  }
}

void FunkmodulClass::updateReplyData() {
    // Per Funk eine Antwort schicken, dass die Daten angekommen sind:
    // aus dem Beispiel übernommen
    // irgdenwann mal durch was sinnvolleres ersetzen
    ackData[0] -= 1;
    ackData[1] -= 1;
    if (ackData[0] < 100) {
        ackData[0] = 109;
    }
    if (ackData[1] < -4009) {
        ackData[1] = -4000;
    }
    fm_rf24_radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}

bool FunkmodulClass::available()  {
  return fm_data_available;
}

bool FunkmodulClass::isConnected() {
  return fm_connection_ok;
}

int FunkmodulClass::readInteger()  {
  int result = fm_buffer;
  fm_buffer = 0;
  fm_data_available = false;
  return result;
}
