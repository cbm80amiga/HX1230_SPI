// HX1230 library example
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
#include "term8x14_font.h"
#include "digits4x7_font.h"

const uint8_t npn [] PROGMEM = {28,4,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10,
0x10, 0x10, 0x10, 0x20, 0x20, 0xA0, 0x40, 0xA0, 0x10, 0x0F, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
0xE0, 0x9C, 0x82, 0x81, 0x80, 0x80, 0x80, 0x80, 0xFE, 0xFE, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02,
0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x1C, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x38, 0x40, 0x80,
0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x02, 0x04, 0x08, 0x90, 0xA0, 0xC0, 0xF0, 0x00, 0x00, 0x00,
0x80, 0x40, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04,
0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x05, 0x02, 0x05, 0x08, 0xF0, 0x00, 0x00
};

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
}

char buf[40];
void loop() 
{
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.setDigitMinWd(8);
  lcd.printStr(0, 0, "ąćęłóśźż 123");
  lcd.printStr(0, 1, "ĄĆĘŁÓŚŹŻ 123");
  lcd.setDigitMinWd(6);
  lcd.printStr(0, 2, "1234567890123");
  lcd.setFontMinWd(5);
  lcd.printStr(0, 3, "abAwij1234");
  lcd.setFontMinWd(0);
  lcd.printStr(0, 4, "abAwij1234");
  lcd.setDigitMinWd(0);
  lcd.printStr(0, 5, "abAwij1234");
  lcd.setDigitMinWd(6);
  lcd.setCharMinWd(6);
  lcd.printStr(0, 6, "abAwij1234");
  lcd.setDigitMinWd(7);
  lcd.setCharMinWd(7);
  lcd.printStr(0, 7, "abAwij1234");
  delay(5000);
  
  lcd.clrScr();
  lcd.setFont(Term8x14PL);
  lcd.printStr(42+12, 4, "Żółw1");
  lcd.drawBitmap(npn, 3, 0);
  lcd.drawBitmap(ind, 11, 7);
  lcd.fillWin(60,0,96-60,3,0xaa);
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(0,2,"B");
  lcd.printStr(30,0,"E");
  lcd.printStr(30,3,"C");
  lcd.printStr(5,7,"0");
  lcd.printStr(40,7,"1");
  lcd.setFont(Digits4x7);
  lcd.setDigitMinWd(4);
  
  for(int i=0; i<300; i++) {
    snprintf(buf,39,"%06d",random(1,1000000));
    lcd.printStr(55+12,7,buf);
    delay(150);
  }
}
