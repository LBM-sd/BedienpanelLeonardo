/*
  LcdModul.cpp

  Copyright (c) 2024, SD

*/
#include "LcdModul.h"

// LCD-Display-Objekt 
rgb_lcd lcdDisplay;


byte lcd_musicalnote[8] = {
    0b00000,
    0b00110,
    0b00101,
    0b00100,
    0b00100,
    0b00100,
    0b11100,
    0b11100
};                                             //Musik Note

byte lcd_affe[8] = {
    0b01010,
    0b00000,
    0b01110,
    0b00001,
    0b01111,
    0b10001,
    0b01111,
    0b00000
};                                             //Buchstabe "Ä"

byte lcd_bier[8] = {
    0b00000,
    0b11100,
    0b10110,
    0b10101,
    0b10101,
    0b10110,
    0b11100,
    0b00000
 };                                            //Bierkrug  

byte lcd_funk[8] = {
    0b00000,
    0b01110,
    0b10001,
    0b00100,
    0b01010,
    0b00000,
    0b00100,
    0b00000  
 };                                            //Funkwellen             

byte lcd_funk_no[8] = {
    0b00000,
    0b00100,
    0b00000,
    0b00100,
    0b01010,
    0b00100,
    0b00100,
    0b00000  
 };                                            //Pfeil nach unten             


LcdModulClass::LcdModulClass()
{
  //             1234567890123456
  fixedTextL1 = "Nächstes Lied:";
  // Die nächsten Befehle NIEMALS hier im Konstruktor ausführen,
  // ansonsten stürtzt Sketch ab!!
  //  lcdDisplay.setColorWhite();
  //  lcdDisplay.setPWM(1, 255);  // geht nicht, da kein Begin() ausgeführt wurde!
  //  lcdDisplay.setPWM(2, 255);
  //  lcdDisplay.setPWM(3, 255);
}

LcdModulClass::~LcdModulClass()
{
    //delete _display;
}

void LcdModulClass::begin(int cols, int rows) {
  lcdDisplay.begin(cols, rows);

  lcdDisplay.createChar(0,lcd_musicalnote);               //Erstellen der Sondersymbole 
  lcdDisplay.createChar(1,lcd_affe);
  lcdDisplay.createChar(2,lcd_bier);
  lcdDisplay.createChar(3,lcd_funk);
  lcdDisplay.createChar(4,lcd_funk_no);

  lcdDisplay.setColorWhite();
  lcdDisplay.home();
  lcdDisplay.print("LCD-Modul V");
  lcdDisplay.print(__LCD_MODUL_VERSION__);
  delay(500);
  lcdDisplay.setColorWhite();
  lcdDisplay.setPWM(1, 255);
  lcdDisplay.setPWM(2, 255);
  lcdDisplay.setPWM(3, 255);
}


void LcdModulClass::setRGB(unsigned char r, unsigned char g, unsigned char b)      
{
  Serial.print("setRGB:");
  Serial.print(r);
  Serial.print("/");
  Serial.print(g);
  Serial.print("/");
  Serial.println(b);
  lcdDisplay.setRGB(r,g,b);
}

void LcdModulClass::setCursor( int column, int row)                         //Cursor setzen
{
  lcdDisplay.setCursor(column, row);
}

void LcdModulClass::show() {
  // Zeile 1:
  // Fester Text: Nächstes Lied + Musiknote
  lcdDisplay.home();
  lcdDisplay.print("N");                              //Scheriben von Buchstabe "N"
  lcdDisplay.write((unsigned char)1);                 //Hinzufügen von sympol affe (ä)
  lcdDisplay.print("chstes Lied ");                   //schreiben von sats"chstes Lied"
  lcdDisplay.write((unsigned char)0);                 //hinzufügen von sympol Note


  // Zeile 2:
  lcdDisplay.setCursor(0, 1); // Zeile 2
  lcdDisplay.print(lcdm_liednummerAuswahl);
  lcdDisplay.setCursor(3, 1); // Zeile 2, Pos4 = immer Leerzeichen, da Lieder nur 3-stellig sein sollen
  lcdDisplay.print(" ");

  lcdDisplay.print(zusatzTextL2);
  // Zusatzfunktione Prosit
  if (lcdm_prosit) {
    lcdDisplay.setCursor(3, 1); // Zeile 2, Pos4 = immer Leerzeichen, da Lieder nur 3-stellig sein sollen
    lcdDisplay.print("Ein Prosit! ");
    // Masskrug anzeigen
    lcdDisplay.write((unsigned char)2);
  }

  // Evtl. Funkverbindungssymbol anzeigen
  lcdDisplay.setCursor(16, 1);
  if (lcdm_wifi) {
    lcdDisplay.write((unsigned char)3);
  } else {
    lcdDisplay.write((unsigned char)4);
  }


}


void LcdModulClass::setFunkVerbindungOK(bool funkverbindungOK) {
  lcdm_wifi = false;
}

void LcdModulClass::setZusatztext(String zusatztext) {
  zusatzTextL2 = zusatztext;
}

void LcdModulClass::setLiednummerAuswahl(int liednummer) {
  lcdm_liednummerAuswahl = liednummer;
}

void LcdModulClass::setHintergrundfarbe(int farbe) {
  if (farbe == LCDM_STANDARDFARBE) {
    lcdDisplay.setRGB(LCDM_STANDARDFARBE_R, LCDM_STANDARDFARBE_G, LCDM_STANDARDFARBE_B);
  } else if (farbe == LCDM_BLAU) {
    lcdDisplay.setRGB(LCDM_BLAU_R, LCDM_BLAU_G, LCDM_BLAU_B);
  } else if (farbe == LCDM_GRUEN) {
    lcdDisplay.setRGB(LCDM_GRUEN_R, LCDM_GRUEN_G, LCDM_GRUEN_B);
  } else if (farbe == LCDM_GELB) {
    lcdDisplay.setRGB(LCDM_GELB_R, LCDM_GELB_G, LCDM_GELB_B);
  } else {
    lcdDisplay.setRGB(LCDM_STANDARDFARBE_R, LCDM_STANDARDFARBE_G, LCDM_STANDARDFARBE_B);
  }
  lcdDisplay.display();
}



// IMPEMENTIERUNG DES PRINT-Moduls
size_t LcdModulClass::write(uint8_t x) {
  return lcdDisplay.write(x);
}

size_t LcdModulClass::write(const uint8_t *buffer, size_t size)
{
  return lcdDisplay.write(buffer, size);
}

// size_t LcdModulClass::print(const __FlashStringHelper *ifsh)
// {
//   return lcdDisplay.print(ifsh);
// }

size_t LcdModulClass::print(const String &s)
{
  return lcdDisplay.print(s);
}

size_t LcdModulClass::print(const char str[])
{
  return lcdDisplay.print(str);
}

size_t LcdModulClass::print(char c)
{
  return lcdDisplay.print(c);
}

size_t LcdModulClass::print(unsigned char b, int base)
{
  return lcdDisplay.print((unsigned long) b, base);
}

size_t LcdModulClass::print(int n, int base)
{
  return lcdDisplay.print((long) n, base);
}

size_t LcdModulClass::print(unsigned int n, int base)
{
  return lcdDisplay.print((unsigned long) n, base);
}

size_t LcdModulClass::print(long n, int base)
{
  return lcdDisplay.print(n,base);
}

size_t LcdModulClass::print(unsigned long n, int base)
{
  return lcdDisplay.print(n, base);
}

size_t LcdModulClass::print(double n, int digits)
{
  return lcdDisplay.print(n, digits);
}

// size_t LcdModulClass::println(const __FlashStringHelper *ifsh)
// {
//   return lcdDisplay.print(ifsh)
// }

size_t LcdModulClass::print(const Printable& x)
{
  return lcdDisplay.print(x);
}

size_t LcdModulClass::println(void)
{
  return lcdDisplay.println();
}

size_t LcdModulClass::println(const String &s)
{
  return lcdDisplay.println(s);
}

size_t LcdModulClass::println(const char c[])
{
  return lcdDisplay.println(c);
}

size_t LcdModulClass::println(char c)
{
  return lcdDisplay.println(c);
}

size_t LcdModulClass::println(unsigned char b, int base)
{
  return lcdDisplay.println(b,base);
}

size_t LcdModulClass::println(int num, int base)
{
  return lcdDisplay.println(num,base);
}

size_t LcdModulClass::println(unsigned int num, int base)
{
  return lcdDisplay.println(num,base);
}

size_t LcdModulClass::println(long num, int base)
{
  return lcdDisplay.println(num,base);
}

size_t LcdModulClass::println(unsigned long num, int base)
{
  return lcdDisplay.println(num,base);
}

size_t LcdModulClass::println(double num, int digits)
{
  return lcdDisplay.println(num,digits);
}

size_t LcdModulClass::println(const Printable& x)
{
  return lcdDisplay.println(x);
}

