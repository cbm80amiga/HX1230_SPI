// HX1230 96x68 LCD library example
// Fonts for numbers demo, requires PropFonts library
// (c) 2019 Pawel A. Hernik
// YouTube video: https://youtu.be/Z7WyPKlUGgM

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

#include "term8x10_font.h"
#include "modern_dig_15x30b_font.h"

#include "term11x24dig_font.h"
#include "term11x24bolddig_font.h"
#include "term9x14pl_font.h"
#include "term8x14_font.h"

#include "modern_dig_15x32_font.h"
#include "modern_dig_15x32b_font.h"
#include "modern_dig_12x32_font.h"
#include "modern_dig_12x32b_font.h"
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

char buf[40];
unsigned long ms,cnt=2134;
int tim=10000;
int tim2=3000;

void setup() 
{
  Serial.begin(9600);
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);
  lcd.init();
  lcd.clrScr();
  //snprintf(buf,39,"0123456789");
  //printNumber(Bold13x20, 13, 0);
  //printNumber(modern_dig_15x30b, 15, 3);
  //printNumber(Term8x10, 8, 7);
  //delay(20000);
}

void printNumber(const uint8_t *f, int wd, int y)
{
  lcd.setFont(f);
  lcd.setDigitMinWd(wd);
  lcd.printStr(0,y,buf);
}

void preview() 
{
  cnt=2134;
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim2) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=123456;
    printNumber(times_dig_16x24, 15, 0);
    printNumber(modern_dig_15x32b, 15, 4);
    delay(150);
  }

  cnt=34;
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim2) {
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
  while(millis()-ms<tim2) {
    cnt+=1234;
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Seg7_13x23b, 13, 0);
    snprintf(buf,39,"%07lu",cnt);
    printNumber(Seg7f_12x23, 12, 3);
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Chicago15x18, 15, 6);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  ms = millis();
  while(millis()-ms<tim2) {
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
  while(millis()-ms<tim2) {
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
}

void loop() 
{
  preview();

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,1,"Modern 15x32");
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=123456;
    printNumber(modern_dig_15x32, 15, 3);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,1,"Modern 15x32b");
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=123456;
    printNumber(modern_dig_15x32b, 15, 3);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,1,"Modern 12x32b");
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%08lu",cnt);
    cnt+=123456;
    printNumber(modern_dig_12x32b, 13, 3);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,1,"Times 16x24");
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=123456;
    printNumber(times_dig_16x24, 15, 3);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"Seg7 12x21");
  lcd.printStr(ALIGN_CENTER,4,"Seg7 12x23b");
  ms = millis();
  while(millis()-ms<tim) {
    snprintf(buf,39,"%07lu",cnt);
    cnt+=1234;
    printNumber(Seg7_12x21, 12, 1);
    printNumber(Seg7_12x23b, 12, 5);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"Seg7f 12x23");
  lcd.printStr(ALIGN_CENTER,4,"Seg7 13x23b");
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=1234;
    snprintf(buf,39,"%07lu",cnt);
    printNumber(Seg7f_12x23, 12, 1);
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Seg7_13x23b, 14, 5);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"Bold 13x20");
  lcd.printStr(ALIGN_CENTER,4,"Chicago 15x18");
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=1234;
    snprintf(buf,39,"%07lu",cnt);
    printNumber(Bold13x20, 13, 1);
    snprintf(buf,39,"%06lu",cnt);
    printNumber(Chicago15x18, 15, 5);
    delay(150);
  }

  cnt=2134;
  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"Term 11x24");
  lcd.printStr(ALIGN_CENTER,4,"Term 11x24b");
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=123456;
    snprintf(buf,39,"%08lu",cnt);
    printNumber(Term11x24, 11, 1);
    printNumber(Term11x24b, 11, 5);
    delay(150);
  }

  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"Term 8x14");
  lcd.printStr(ALIGN_CENTER,4,"Term 9x14");
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=123456;
    snprintf(buf,39,"%010lu",cnt);
    printNumber(Term8x14PL, 8, 1);
    snprintf(buf,39,"%09lu",cnt);
    printNumber(Term9x14PL, 9, 5);
    delay(150);
  }

  lcd.clrScr();
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(ALIGN_CENTER,0,"C64 7x7");
  lcd.printStr(ALIGN_CENTER,2,"Small 5x7");
  lcd.printStr(ALIGN_CENTER,5,"Small 4x7 and 4x6");
  ms = millis();
  while(millis()-ms<tim) {
    cnt+=123456111111;
    snprintf(buf,39,"%012lu",cnt);
    printNumber(c64enh, 7, 1);
    snprintf(buf,39,"%016lu",cnt);
    printNumber(Small5x7PLBold, 5, 3);
    printNumber(Small5x7PL, 5, 4);

    snprintf(buf,39,"%019lu",cnt);
    printNumber(Small4x7PL, 4, 6);
    snprintf(buf,39,"%019lu",cnt);
    printNumber(Small4x6PL, 4, 7);
    snprintf(buf,39,"%012lu",cnt);
    delay(80);
  }
}
