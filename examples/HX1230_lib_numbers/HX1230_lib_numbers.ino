// HX1230 96x68 LCD library example
// Digits demo
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

#include "term11x24dig_font.h"
#include "term11x24bolddig_font.h"
#include "term9x14pl_font.h"

#include "modern_dig_15x32_font.h"
//#include "modern_dig_12x32_font.h"
#include "times_dig_16x24_font.h"

#include "c64enh_font.h"
#include "small5x7_font.h"
#include "small5x7bold_font.h"
#include "term8x10_font.h"
#include "term7x14_font.h"
#include "small4x7_font.h"
#include "small4x6_font.h"

#include "seg7_12x21dig_font.h"
//#include "seg7_12x23dig_font.h"
#include "seg7_12x23bdig_font.h"
#include "seg7_13x23bdig_font.h"
#include "seg7fill_12x23dig_font.h"
#include "bold13x20digtop_font.h"
#include "chicago15x18digtop_font.h"

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
}

char buf[40];
unsigned long ms,cnt=2134;
int tim=6000;

void printNumber(const uint8_t *f, int wd, int y)
{
    lcd.setFont(f);
    lcd.setDigitMinWd(wd);
    lcd.printStr(0,y,buf);
}

void loop() 
{
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=123456;
    snprintf(buf,39,"%09lu",cnt);
    printNumber(Term9x14PL, 9, 0);
    snprintf(buf,39,"%08lu",cnt);
    printNumber(Term11x24, 11, 2);
    printNumber(Term11x24b, 11, 5);
    delay(150);
  }

  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=123456111111;
    snprintf(buf,39,"%012lu",cnt);
    printNumber(c64enh, 7, 0);
    snprintf(buf,39,"%016lu",cnt);
    printNumber(Small5x7PLBold, 5, 1);
    printNumber(Small5x7PL, 5, 2);
    snprintf(buf,39,"%019lu",cnt);
    printNumber(Small4x7PL, 4, 3);
    snprintf(buf,39,"%019lu",cnt);
    printNumber(Small4x6PL, 4, 4);
    snprintf(buf,39,"%012lu",cnt);
    printNumber(Term7x14PL, 7, 5);
    snprintf(buf,39,"%010lu",cnt);
    printNumber(Term8x10, 8, 7);
    delay(80);
  }

  cnt=2134;
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%08lu",cnt);
    cnt+=123456;
    printNumber(times_dig_16x24, 15, 0);
    printNumber(modern_dig_15x32, 15, 4);
    delay(150);
  }
  
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=1234;
    printNumber(Seg7_12x23b, 12, 0);
    printNumber(Seg7_12x21, 12, 3);
    //printNumber(Seg7_12x23, 12, 3);
    printNumber(Bold13x20, 13, 6);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=1234;
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Seg7_13x23b, 13, 0);
    snprintf(buf,39,"%07lu",cnt);
    printNumber(Seg7f_12x23, 12, 3);
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Chicago15x18, 15, 6);
    delay(150);
  }

}
