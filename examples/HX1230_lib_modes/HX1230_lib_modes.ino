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

#include "small4x7_font.h"

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
}

// --------------------------------------------------------------------------
char buf[40];

void loop() 
{
  int i;
  lcd.clrScr();
  lcd.setFont(Small4x7PL);

  lcd.printStr(ALIGN_CENTER, 4, "HX1230_INVERT_OFF", 1);
  lcd.displayInvert(false);
  delay(3000);
  lcd.printStr(ALIGN_CENTER, 4, "HX1230_INVERT_ON", 1);
  lcd.displayInvert(true);
  delay(3000);
  lcd.displayInvert(false);

  lcd.printStr(ALIGN_CENTER, 4, "HX1230_DISPLAY_OFF", 1);
  delay(1000);
  lcd.displayOn(false);
  delay(1000);
  lcd.printStr(ALIGN_CENTER, 4, "HX1230_DISPLAY_ON", 1);
  lcd.displayOn(true);
  delay(2000);

  for(i=0;i<=30;i+=5) {
    snprintf(buf,40,"CONTRAST %d", i);
    lcd.printStr(ALIGN_CENTER, 4, buf, 1);
    lcd.setContrast(i);
    delay(2000);
  }
  lcd.printStr(ALIGN_CENTER, 4, "CONTRAST 16", 1);
  lcd.setContrast(16);
  delay(2000);

  lcd.printStr(0, 0, "0");
  lcd.printStr(0, 1, "1");
  lcd.printStr(0, 2, "2");
  lcd.printStr(0, 3, "3");
  lcd.printStr(0, 4, "4");
  lcd.printStr(0, 5, "5");
  lcd.printStr(0, 6, "6");
  lcd.printStr(0, 7, "7");
  lcd.printStr(0, 8, "8");

  lcd.printStr(ALIGN_CENTER, 4, " ROTATE 180 ");
  lcd.setRotate(2);
  delay(3000);
  lcd.printStr(ALIGN_CENTER, 4, "   ROTATE 0  ");
  lcd.setRotate(0);
  delay(2000);

  for(i=0;i<64;i++) {
    snprintf(buf,40,"    SCROLL %02d    ", i);
    lcd.printStr(ALIGN_CENTER, 4, buf);
    lcd.setScroll(i);
    delay(500);
  }
  lcd.printStr(ALIGN_CENTER, 4, "  SCROLL 0  ");
  lcd.setScroll(0);
  delay(2000);
}
