// Low resource HX1230 96x68 LCD library
// (C)2019 by Pawel A. Hernik

/*
  HX1230 96x68 LCD connections (header on bottom, from left):
  #1 RST - D6 or any digital
  #2 CE  - D7 or any digital
  #3 N/C
  #4 DIN - D11/MOSI 
  #5 CLK - D13/SCK
  #6 VCC - 3V3
  #7 BL  - 3V3 or any digital via resistor
  #8 GND - GND
*/

#ifndef _HX1230_SPI__H
#define _HX1230_SPI__H 

// define USESPI for hw 9-bit SPI version, instead sw bit banging will be used
#define USESPI 1

#if USESPI==1
#include <SPI.h>
#endif

#include <Arduino.h>
#include <avr/pgmspace.h>

#define SCR_WD  96
#define SCR_HT  68

#define ALIGN_LEFT    0
#define ALIGN_RIGHT  -1 
#define ALIGN_CENTER -2

#define CMD 0x00
#define DAT 0x80

#define HX1230_POWER_ON         0x2F // internal power supply on
#define HX1230_POWER_OFF        0x28 // internal power supply off
#define HX1230_CONTRAST         0x80 // 0x80 + (0..31)
#define HX1230_SEG_NORMAL       0xA0 // SEG remap normal
#define HX1230_SEG_REMAP        0xA1 // SEG remap reverse (flip horizontal)
#define HX1230_DISPLAY_NORMAL   0xA4 // display ram contents
#define HX1230_DISPLAY_TEST     0xA5 // all pixels on
#define HX1230_INVERT_OFF       0xA6 // not inverted
#define HX1230_INVERT_ON        0xA7 // inverted
#define HX1230_DISPLAY_ON       0XAF // display on
#define HX1230_DISPLAY_OFF      0XAE // display off
#define HX1230_SCAN_START_LINE  0x40 // scrolling 0x40 + (0..63)
#define HX1230_COM_NORMAL       0xC0 // COM remap normal
#define HX1230_COM_REMAP        0xC8 // COM remap reverse (flip vertical)
#define HX1230_SW_RESET         0xE2 // connect RST pin to GND and rely on software reset
#define HX1230_NOP              0xE3 // no operation
#define HX1230_COL_ADDR_H       0x10 // x pos (0..95) 4 MSB
#define HX1230_COL_ADDR_L       0x00 // x pos (0..95) 4 LSB
#define HX1230_PAGE_ADDR        0xB0 // y pos, 8.5 rows (0..8)

// ---------------------------------
class HX1230_SPI {
public:
  HX1230_SPI(byte rst, byte cs);
  HX1230_SPI(byte rst, byte cs, byte din, byte clk);

  inline void sendSPI(uint8_t v, uint8_t dc) __attribute__((always_inline)); // costs about 350B of flash
  inline void sendCmd(uint8_t cmd);
  inline void sendData(uint8_t data);
  
  void init();
  void gotoXY(byte x, byte y);
  void sleep(bool mode);
  void setContrast(byte val);
  void setScroll(byte val);
  void displayInvert(bool mode);
  void displayOn(bool mode);
  void displayMode(byte val);
  void setRotate(int mode);
  void clrScr();
  int drawBuf(const uint8_t *bmp, int x, uint8_t y8, uint8_t wd, uint8_t ht8);
  int drawBitmap(const uint8_t *bmp, int x, uint8_t y8, uint8_t wd, uint8_t ht8);
  int drawBitmap(const uint8_t *bmp, int x, uint8_t y8);
  int fillWin(int x, uint8_t y8, uint8_t wd, uint8_t ht8, uint8_t data);

  void setFont(const uint8_t* f);
  void setCR(uint8_t _cr) { cr = _cr; }
  void setInvert(uint8_t _inv) { invertCh = _inv; }
  void setInvertMask(uint8_t mask) { invertMask = mask; }
  void setFontMinWd(uint8_t wd) { minCharWd = wd; }
  void setCharMinWd(uint8_t wd) { minCharWd = wd; }
  void setDigitMinWd(uint8_t wd) { minDigitWd = wd; }
  int printStr(int x, uint8_t y8, const char *txt, int clrLine=0);
  int printChar(int x, uint8_t y8, uint8_t ch);
  int charWidth(uint8_t _ch, bool last=true);
  int strWidth(const char *txt);
  unsigned char convertPolish(unsigned char _c);
  static bool isNumber(uint8_t ch);
  static bool isNumberExt(uint8_t ch);
  void setIsNumberFun(bool (*fun)(uint8_t)) { isNumberFun=fun; }
  
public:
   
//private:
  bool (*isNumberFun)(uint8_t ch);
  const uint8_t* font; 
  uint8_t xSize;
  uint8_t ySize;
  uint8_t ySize8;
  uint8_t firstCh;
  uint8_t lastCh;
  uint8_t minCharWd;
  uint8_t minDigitWd;
  uint8_t cr;  // carriage return mode for printStr
  uint8_t dualChar;
  uint8_t invertCh,invertMask;
  //uint8_t buf9[9] = {0};

private:
  uint8_t csPin, rstPin, dinPin, clkPin;
#ifdef __AVR__
  uint8_t csMask, dinMask, clkMask;
  volatile uint8_t *csPort, *dinPort, *clkPort;
#endif
};
#endif

