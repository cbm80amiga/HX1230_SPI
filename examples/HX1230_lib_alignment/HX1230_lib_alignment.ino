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

//#include "fonts_all.h"
#include "small5x7bold_font.h"
#include "small4x7_font.h"
#include "digits4x7_font.h"

const uint8_t ind [] PROGMEM = {27,1,
0x40, 0x40, 0x40, 0x78, 0x04, 0x02, 0x02, 0x04, 0x78, 0x04, 0x02, 0x02, 0x04, 0x78, 0x04, 0x02,
0x02, 0x04, 0x78, 0x04, 0x02, 0x02, 0x04, 0x78, 0x40, 0x40, 0x40, 
};

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
  lcd.setDigitMinWd(0);
}

// --------------------------------------------------------------------------
char buf[40];
void loop() 
{
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_LEFT, 0, "Align left");
  lcd.drawBitmap(ind, ALIGN_LEFT, 1);
  lcd.printStr(ALIGN_RIGHT, 2, "Align right");
  lcd.drawBitmap(ind, ALIGN_RIGHT, 3);
  lcd.printStr(ALIGN_CENTER, 4, "Align center");
  lcd.drawBitmap(ind, ALIGN_CENTER, 5);
  delay(4000);
  
  lcd.clrScr();
  lcd.setFont(Small4x7PL);
  lcd.setFontMinWd(0); // default proportional font
  lcd.printStr(0, 0, "abcdefghijklmnopqrtsuvwxyz");
  lcd.setFontMinWd(4); // force fixed width
  lcd.printStr(0, 1, "abcdefghijklmnopqrtsuvwxyz");
  lcd.setFontMinWd(5);
  lcd.printStr(0, 2, "abcdefghijklmnopqrtsuvwxyz");
  lcd.setFontMinWd(0);
  
  for(int i=0;i<500;i++) {
    snprintf(buf,19,"%05u",(unsigned int) random(1,100000));
    //lcd.fillWin(0,3,84,3,0xaa);
    //lcd.fillWin(0,3,84,3,0x00);
    lcd.setFont(Digits4x7);
    lcd.setDigitMinWd(0);
    lcd.printStr(ALIGN_LEFT, 3, buf);
    lcd.printStr(ALIGN_RIGHT, 3, buf);
    lcd.setDigitMinWd(4);
    lcd.printStr(ALIGN_LEFT, 4, buf);
    lcd.printStr(ALIGN_RIGHT, 4, buf);
    lcd.setDigitMinWd(5);
    lcd.printStr(ALIGN_LEFT, 5, buf);
    lcd.printStr(ALIGN_RIGHT, 5, buf);
    lcd.setFont(Small4x7PL);
    lcd.setFontMinWd(0); // default proportional font
    lcd.printStr(ALIGN_CENTER, 3, "prop");
    lcd.printStr(ALIGN_CENTER, 4, "fix4");
    lcd.printStr(ALIGN_CENTER, 5, "fix5");
    delay(300);
  }
  delay(1000);
}
