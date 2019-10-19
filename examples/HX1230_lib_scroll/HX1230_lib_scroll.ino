// HX1230 96x68 LCD library example
// (c) 2019 Pawel A. Hernik

/*
  HX1230 96x68 LCD connections (header on bottom, from left):
  #1 RST - D6 or any digital
  #2 CE  - D7 or any digital
  #3 N/C
  #4 DIN - D11/MOSI 
  #5 CLK - D13/SCK
  #6 VCC - 3V3
  #7 BL  - 3V3 or any digital
  #8 GND - GND
*/


#define LCD_RST 6
#define LCD_CS  7
#define LCD_BL  8
#define LCD_DIN 11
#define LCD_CLK 13

#include "HX1230_SPI.h"
// define USESPI in above header for HW 9-bit SPI

//HX1230_SPI lcd(LCD_RST, LCD_CS, LCD_DIN, LCD_CLK);
HX1230_SPI lcd(LCD_RST, LCD_CS);

#if USESPI==1
#include <SPI.h>
#endif

#include "small5x7bold_font.h"
#include "term7x14_font.h"

const uint8_t icon1 [] PROGMEM = { 16,2,
0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xDE, 0xCC, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80,
0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7B, 0x33, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
};

const uint8_t icon2 [] PROGMEM = { 16,2,
0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xCC, 0xDE, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x33, 0x7B, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01,
};

const uint8_t icon3 [] PROGMEM = { 16,2,
0xFC, 0x0C, 0x14, 0x24, 0x44, 0x84, 0x04, 0x04, 0x04, 0x04, 0x84, 0x44, 0x24, 0x14, 0x0C, 0xFC,
0x3F, 0x30, 0x28, 0x24, 0x22, 0x21, 0x21, 0x22, 0x22, 0x21, 0x21, 0x22, 0x24, 0x28, 0x30, 0x3F,
};

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
}

// --------------------------------------------------------------------------

void loop() 
{
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  for(int i=0;i<=96; i++) {
    lcd.printStr(i, 0, "   Scroll test");
    lcd.printStr(i, 1, "abcdefghijklmnopqrtsuvwxyz");
    lcd.printStr(i, 2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    lcd.printStr(i, 3, "01234567890");
    lcd.drawBitmap(icon1, i+00, 4);
    lcd.drawBitmap(icon1, i+16, 4);
    lcd.drawBitmap(icon3, i+32, 4);
    lcd.drawBitmap(icon2, i+48, 4);
    lcd.drawBitmap(icon2, i+64, 4);
    if(i>0) lcd.fillWin(i-1,0,1,6,0); else delay(4000);
    delay(10);
  }
  lcd.setFont(Term7x14PL);
  for(int i=96-1;i--;) {
    lcd.printStr(i, 0, "Scroll test");
    lcd.printStr(i, 2, "ABCDEFGHIJKL");
    lcd.printStr(i, 4, "01234567890");
    delay(10);
  }
  delay(4000);
  lcd.setFont(Term7x14PL);
  for(int i=0;i<=96; i++) {
    lcd.printStr(i, 0, "Scroll test");
    lcd.printStr(i, 2, "ABCDEFGHIJKLMN");
    lcd.printStr(i, 4, "01234567890");
    if(i>0) lcd.fillWin(i-1,0,1,6,0);
    delay(10);
  }
  lcd.setFont(Small5x7PLBold);
  for(int i=96-1;i--;) {
    lcd.printStr(i, 0, "   Scroll test");
    lcd.printStr(i, 1, "abcdefghijklmnopqrtsuvwxyz");
    lcd.printStr(i, 2, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    lcd.printStr(i, 3, "01234567890");
    lcd.drawBitmap(icon1, i+00, 4);
    lcd.drawBitmap(icon1, i+16, 4);
    lcd.drawBitmap(icon3, i+32, 4);
    lcd.drawBitmap(icon2, i+48, 4);
    lcd.drawBitmap(icon2, i+64, 4);
    delay(10);
  }
}

