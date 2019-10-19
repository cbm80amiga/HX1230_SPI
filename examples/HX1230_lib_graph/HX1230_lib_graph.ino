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

// --------------------------------------------------------------------------
byte scr[96*9];  // full frame buffer - for demo
byte scrWd = 96;
byte scrHt = 9;

void clrBuf()
{
  int ii=0;
  for(int i=0;i<scrWd*scrHt;i++) scr[ii++]=0;
}
// --------------------------------------------------------------------------
void drawPixel(int16_t x, int16_t y, uint16_t color) 
{
  if(x<0 || x>=scrWd || y<0 || y>=68) return;
  switch (color) {
    case 1: scr[x+(y/8)*scrWd] |=  (1 << (y&7)); break;
    case 0: scr[x+(y/8)*scrWd] &= ~(1 << (y&7)); break; 
    case 2: scr[x+(y/8)*scrWd] ^=  (1 << (y&7)); break; 
  }
}
// --------------------------------------------------------------------------

char buf[40];
byte patCnt=0;
byte pat[]={3,6,0xc,0x18,0x30,0x60,0xc0,0x81};
void loop() 
{
  int i,j;
  lcd.clrScr();
  lcd.setFont(Term8x14PL);
  lcd.printStr(55+10, 5, "Cnt:");
  lcd.drawBitmap(npn, 3, 0);
  lcd.drawBitmap(ind, 50+10, 0);
  lcd.setFont(Small5x7PLBold);
  lcd.printStr(0,2,"B");
  lcd.printStr(30,0,"E");
  lcd.printStr(30,3,"C");
  lcd.printStr(44+10,0,"0");
  lcd.printStr(78+10,0,"1");
  lcd.setFont(Digits4x7);
  lcd.setDigitMinWd(4);
  
  // 60x16 pixels graph
  scrWd = 60;
  scrHt = 3;
  for(j=0;j<400;j++) {
    clrBuf();  
    for(i = 0;i<scrWd;i++) {
      int y = sin(i/5.0)*sin(j/10.0+i/10.0)*12+12;
      drawPixel(i,y,1);
    }
    lcd.drawBuf(scr,0,5,scrWd,scrHt);
    snprintf(buf,39,"%06d",random(1,1000000));
    if((j%4)==0) lcd.printStr(55+10,7,buf);
    lcd.fillWin(40,1,96-40,4,pat[patCnt]);
    patCnt++; if(patCnt==8) patCnt=0; 
    delay(30);
  }
  
  // full screen graph
  scrWd = 96;
  scrHt = 9;
  for(j=0;j<300;j++) {
    clrBuf();  
    for(i = 0;i<scrWd;i++) {
      int y = sin(i/10.0)*sin(j/10.0+i/20.0)*34+34;
      drawPixel(i,y,1);
      drawPixel(i,y+1,1);
      drawPixel(i+1,y,1);
      drawPixel(i+1,y+1,1);
      drawPixel(i,0,1);
      drawPixel(i,68-1,1);
    }
    for(i=1;i<68-1;i++) {
      drawPixel(0,i,1);
      drawPixel(scrWd-1,i,1);
    }
    lcd.drawBuf(scr,0,0,scrWd,scrHt);
    lcd.setFont(Small5x7PLBold);
    lcd.printStr(ALIGN_CENTER,1,"GRAPH");
    delay(20);
  }

}
