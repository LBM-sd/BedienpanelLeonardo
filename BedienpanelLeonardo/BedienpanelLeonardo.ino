/*
  BedienpanelLeonardo.cpp

  Copyright (c) 2024, SD

  Version 1.6
*/

#include "Arduino.h"
#include "Keypad.h"
#include "Wire.h"
#include <SoftwareSerial.h>

// für RGB-LCD-Modul
#include "LcdModul.h"

// Für Funk-Modul
#include "Funkmodul.h"
FunkmodulClass funkmodul;

// für Blink-LED
#include "Led3.h"
Led3 ledRot(12);
Led3 ledOrange(13);


// Codes für die Kommunkation von Sender und Empfänger 
#include "LBM_Display.h"

#define PIN_A0   (14)
#define PIN_A1   (15)


//rgb_lcd lcd;
LcdModulClass lcd;
SoftwareSerial device(0,1);

const int colorR = 255;                        //Hindergrund beleichtung Farbe rot
const int colorG = 0;                          //Hindergrund beleichtung Farbe grün
const int colorB = 0;                          //Hindergrund beleichtung Farbe blau

const byte ROWS = 4;                           // Anzahl der Zeilen des Tastenfelds
const byte COLS = 4;                           // Anzahl der Spalten des Tastenfelds

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = { 5, 4, 3, 2};       // Die Pins, an denen die Spalten angeschlossen sind   
byte colPins[COLS] = { 9, 8, 7, 6};         // Die Pins, an denen die Zeilen angeschlossen sind

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


String neuerText;
String aktuelleAuswahl = "000";
 
void setup() {
  pinMode(PIN_A0, INPUT_PULLUP);
  pinMode(PIN_A1, INPUT_PULLUP);

  Serial.begin(9600);                          //Ermöglicht das auslesen und senden per computer 
  Serial.println("LeonardoSide");
  Serial.println("============");

  lcd.begin(16, 2);                            // Anzahl der Zeichen pro Zeile und Anzahl der Zeilen des Displays

  // set up the lcd's number of columns and rows:
  lcd.setRGB(colorR, colorG, colorB);          //Hintergrundfarbe auf Standard setzen      
  lcd.setCursor(0, 0);                         //Cursor auf Zeile 1 setzen
  lcd.print("N");                              //Scheriben von Buchstabe "N"
  lcd.write((unsigned char)1);                 //Hinzufügen von sympol affe (ä)
  lcd.print("chstes Lied ");                   //schreiben von sats"chstes Lied"
  lcd.write((unsigned char)0);                 //hinzufügen von sympol Note
  lcd.setCursor(0, 1);                         //Cursor auf Zeile 2 setzen   

  lcd.setHintergrundfarbe(LCDM_GELB);

  ledOrange.configBlink(100, 200);

  Serial.begin(9600);                          //Ermöglicht das auslesen und senden per computer 
  device.begin(9600);                          //ermöglicht die komunikation zwischen den Arduinos
  keypad.addEventListener(keypadEvent);        // Add an event listener for this keypad  

  neuerText = "000";
  aktuelleAuswahl = "000";

  funkmodul = FunkmodulClass(11,10);    // CE_PIN=11 CSN_PIN=10
  funkmodul.init(false, true, 4);

  funkmodul.sendInteger(LBM_WIFI_CHECK);
}

void loop() {
  char key = keypad.getKey();
  
  if (Serial.available()) {
    key = Serial.read();
  }

  if (key != NO_KEY){
    Serial.println(key);                       //gibt die nächste taste nur über PC aus

    switch(key){
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        neuerText = neuerText + key;
        if (neuerText.length()>=4) {
          //Serial.print("neuerText:");
          //Serial.println(neuerText);
          neuerText = neuerText.substring(1,4);
          //Serial.print("neuerText:");
          //Serial.println(neuerText);
          lcd.setCursor(0, 1);                   // Cursor auf die zweite Zeile setzen
          lcd.print("                    ");     //Löschen der numern auf dem lcd display 
          lcd.setCursor(0, 1);                   // Cursor auf die zweite Zeile setzen
          lcd.print(neuerText);
        }
        if (neuerText.length()>=1 && neuerText.length()<=3) {
            lcd.setLiednummerAuswahl(neuerText.toInt());
            lcd.show();
        }
        break;
      case 'A':
        lcd.setRGB(colorR, colorG, colorB);
        sendeDaten(LBM_STANDBY_OFF);                  //Ausschalten vom standby punkt
        sendeDaten(LBM_LEDS_OFF);                   //led´s aus
        ledOrange.off();
        break;
      case 'B':
        lcd.setRGB (17, 65, 138);             //Hindergrund farbe auf blau setzen
        sendeDaten(LBM_LED_BLUE_BLINK);                   //Senden von Farbe blau Blinken  
        sendeDaten(LBM_STANDBY_OFF);                  //Ausschalten vom standbuy punkt
        ledOrange.setMode(Blink);                    //kleine Led am Bedinteil blinken
        break;
      case 'C':
        lcd.setRGB (2, 206, 0);               //Hindergrund farbe auf grün setzen
        lcd.setHintergrundfarbe(LCDM_GRUEN);               //Hindergrund farbe auf grün setzen
        sendeDaten(LBM_LED_GREEN_BLINK);                   //Senden von Farbe grün Blinken 
        sendeDaten(LBM_STANDBY_OFF);                  //Ausschalten vom standbuy punkt
        ledOrange.setMode(Blink);                    //kleine Led am Bedinteil blinken
        break;
      case 'D':
        //Senden an Arduino Mega
        sendeDaten(40);                   //Senden Von Zahl 40 
        lcd.setCursor(0, 1);                   // Cursor auf die zweite Zeile setzen
        lcd.print("                    ");     //Löschen der numern auf dem lcd display 
        lcd.setCursor(0, 1);                   // Cursor auf die zweite Zeile setzen
        lcd.print("40");     //Löschen der numern auf dem lcd display 
        lcd.setCursor(4, 1);                  //Cursor auf die zweite Zeile setzen
        lcd.setRGB(250, 115, 2);              //Hindergrund farbe auf gelb setzen 
        lcd.print("Ein Prosit ");             //Text anzeigen 
        lcd.write((unsigned char)2);          //Setzen von icon 
        sendeDaten(LBM_STANDBY_OFF);                  //Ausschalten vom standbuy punkt
        sendeDaten(LBM_LED_BLUE_BLINK);                   //Senden von Farbe blau Blinken
        ledOrange.setMode(Blink);                    //kleine Led am Bedinteil blinken
        break;
        
      case '*':
        if (neuerText != "") {
          //Serial.print("Sende");              //Auslesbahr über computer terminal 
          //Serial.println(neuerText.toInt());  //Auslesbahr über computer terminal 
          sendeDaten(neuerText.toInt());
          sendeDaten(LBM_STANDBY_OFF);                //Ausschalten vom standbuy punkt
          lcd.setCursor(15, 0);
          lcd.write((unsigned char)3);
          delay(1000);
          lcd.setCursor(15, 0);
          lcd.print(" ");         

          aktuelleAuswahl = neuerText;
        }
        break;
      case '#':
        lcd.setCursor(0, 1);                   // Cursor auf die zweite Zeile setzen
        lcd.print("                    ");     //Löschen der numern auf dem lcd display 
        sendeDaten(LBM_STANDBY_ON);            //Standbuy led an 
        sendeDaten(LBM_LEDS_OFF);              //Blinkende Led´s aus 
        sendeDaten(LBM_CLEAR_NUMBERS);         //Löschen der nummern auf dem 7-segment display
        lcd.setRGB(colorR, colorG, colorB);    // Display auf Standart Farbe setzen
        aktuelleAuswahl = "";
        neuerText = "";
        ledOrange.off();                            //kleine Led am Bedinteil blinken aus 
        break;
    }
  }
  

  if (funkmodul.isConnected()) {
    // Funkverbindung ist ok
    // auf Display anzeigen:
    //lcd.setCursor(16, 0);
    //lcd.print(3); // WIFI-Zeichen
    lcd.setFunkVerbindungOK(true);
  } else {
    lcd.setFunkVerbindungOK(false);
  }
  ledRot.update();
  ledOrange.update();

  key=NO_KEY;
}


void sendeDaten(int zahl) {
  if (zahl == LBM_WIFI_CHECK || zahl == LBM_WIFI_CHECK_OK || zahl == LBM_WIFI_CHECK_FAIL) {
    // diese Codes nicht über Kabel senden
    // (dienen der Prüfung der Funk-Verbindung)
    ledRot.on();
  } else {
    // über Kabel senden
    device.println(zahl);
    ledRot.off();
  }
  // über Funk senden
  funkmodul.sendInteger(zahl);
}


void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
  case PRESSED:
    if (key == '#') {
//      lcd.setCursor(0, 1); // Cursor auf die zweite Zeile setzen
//      lcd.print("                    ");
   }
    if (key == 'D') {
//      lcd.setCursor(4, 1); // Cursor auf die zweite Zeile setzen
//      lcd.print("Ein Prosit ");
//      lcd.write((unsigned char)2);
//      lcd.setRGB(250, 115, 2);
  }
  if (key == 'A') {
//    lcd.setRGB(colorR, colorG, colorB);
    }
  if (key == 'B') {
//    lcd.setRGB (17, 65, 138);
    }
  if (key == 'C') {
//    lcd.setRGB (2, 206, 0);
    }
 }
}
