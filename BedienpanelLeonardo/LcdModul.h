/*
  LcdModul.h

  Copyright (c) 2024, SD

*/

#pragma once
#include <Arduino.h>
#include <inttypes.h>
#include "Print.h"
#include "rgb_lcd.h"

#ifndef __LCD_MODUL_H__
#define __LCD_MODUL_H__

#define __LCD_MODUL_VERSION__ 11

#define LCDM_STANDARDFARBE  0
#define LCDM_STANDARDFARBE_R  255
#define LCDM_STANDARDFARBE_G  0
#define LCDM_STANDARDFARBE_B  0

#define LCDM_BLAU 1
#define LCDM_BLAU_R 17  // Rotwert
#define LCDM_BLAU_G 65  // Grünwert
#define LCDM_BLAU_B 138  // Blauwert

#define LCDM_GELB 2
#define LCDM_GELB_R 250
#define LCDM_GELB_G 115
#define LCDM_GELB_B 2

//        lcd.setRGB (2, 206, 0);               //Hindergrund farbe auf grün setzen
#define LCDM_GRUEN 3
#define LCDM_GRUEN_R 2
#define LCDM_GRUEN_G 206
#define LCDM_GRUEN_B 0


// Wrapper-Klasse für das RGB-LCD-Modul

class LcdModulClass //: public rgb_lcd 
{
public:
    LcdModulClass();
    ~LcdModulClass();

    void begin(int cols, int rows);
    void setRGB(unsigned char r, unsigned char g, unsigned char b);               // set rgb      
    //using rgb_lcd::setRGB;
    void setCursor(int line, int column);                         //Cursor auf Zeile 1 setzen
    //using rgb_lcd::setCursor;
    //void write(uint8_t value);
    //using rgb_lcd::write;

    void Prosit();
    void setFunkVerbindungOK(bool funkverbindungOK);
    void setLiednummerAuswahl(int liednummer);
    void setZusatztext(String zusatztext);
    void setHintergrundfarbe(int farbe);

    void show();

    size_t write(uint8_t);
    size_t write(const uint8_t *, size_t);
    // size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);
    size_t print(unsigned char, int = DEC);
    size_t print(int, int = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long, int = DEC);
    size_t print(unsigned long, int = DEC);
    size_t print(double, int = 2);
    size_t print(const Printable&);

    // size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long, int = DEC);
    size_t println(unsigned long, int = DEC);
    size_t println(double, int = 2);
    size_t println(const Printable&);
    size_t println(void);


private:
  
  bool lcdm_wifi = false;
  bool lcdm_prosit = false;
  int  lcdm_liednummerAuswahl = 0;
  String fixedTextL1 ;
  String zusatzTextL2;  // z.B. "ein Prosit"
    //void loop();
};






#endif