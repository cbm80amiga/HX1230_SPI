#include "HX1230_SPI.h"

#define CONVERT_POLISH

#ifdef __AVR__
#define CS_IDLE    *csPort |= csMask
#define CS_ACTIVE  *csPort &= ~csMask
#else
#define CS_IDLE     digitalWrite(csPin, HIGH)
#define CS_ACTIVE   digitalWrite(csPin, LOW)
#endif

const uint8_t initData[] PROGMEM = {
    HX1230_POWER_ON,
    HX1230_CONTRAST+16,
    HX1230_INVERT_OFF,
    HX1230_DISPLAY_NORMAL,
    HX1230_DISPLAY_ON,
    HX1230_SCAN_START_LINE+0,
    HX1230_PAGE_ADDR+0,
    HX1230_COL_ADDR_H+0,
    HX1230_COL_ADDR_L+0 
};
// ---------------------------------
HX1230_SPI::HX1230_SPI(byte rst, byte cs)
{
  rstPin = rst;
  csPin  = cs;
}
// ---------------------------------
HX1230_SPI::HX1230_SPI(byte rst, byte cs, byte din, byte clk)
{
  rstPin = rst;
  csPin  = cs;
  clkPin = clk;
  dinPin = din;
}
// ---------------------------------
// dc=0 -> command
// dc=1 -> data
#if USESPI==1
// hardware SPI: 9 bits are sent in 2 bytes, still faster than software implementation (108-140fps)
inline void HX1230_SPI::sendSPI(uint8_t v, uint8_t dc)
{
  CS_ACTIVE;
  SPI.transfer((v>>1)|dc);
  SPI.transfer(v<<7);
  CS_IDLE;
}
#else
// software 9-bit SPI, faster on AVR (55fps)
inline void HX1230_SPI::sendSPI(uint8_t v, uint8_t dc)
{
  byte b;
#ifdef __AVR__
  uint8_t dinMaskN=~dinMask;  // faster than global variables !?
  uint8_t clkMaskN=~clkMask;
  if(dc) *dinPort|=dinMask; else *dinPort&=dinMaskN;
  *clkPort|=clkMask; *clkPort&=clkMaskN;
#else
  digitalWrite(dinPin, dc);
  digitalWrite(clkPin, HIGH); digitalWrite(clkPin, LOW);
#endif  
  for(b=0; b<8; b++) {  // send 8 data bits
#ifdef __AVR__
    if((v & 0x80)) *dinPort|=dinMask; else *dinPort&=dinMaskN;
    *clkPort|=clkMask; *clkPort&=clkMaskN;
#else
    digitalWrite(dinPin, (v & 0x80));
    digitalWrite(clkPin, HIGH); digitalWrite(clkPin, LOW);
#endif
    v <<= 1;
  }
}
#endif
// ----------------------------------------------------------------
inline void HX1230_SPI::sendCmd(uint8_t cmd)
{
  sendSPI(cmd,CMD);
}
// ----------------------------------------------------------------
inline void HX1230_SPI::sendData(uint8_t data)
{
  sendSPI(data,DAT);
}
// ----------------------------------------------------------------
void HX1230_SPI::init()
{
  isNumberFun = &isNumber;
  cr = 0;
  font = NULL;
  dualChar = 0;
  pinMode(csPin, OUTPUT);
  pinMode(dinPin, OUTPUT);
  pinMode(clkPin, OUTPUT);
  
#ifdef __AVR__
//#if USESPI==0
  dinPort = portOutputRegister(digitalPinToPort(dinPin));
  dinMask = digitalPinToBitMask(dinPin);
  clkPort = portOutputRegister(digitalPinToPort(clkPin));
  clkMask = digitalPinToBitMask(clkPin);
  csPort  = portOutputRegister(digitalPinToPort(csPin));
  csMask  = digitalPinToBitMask(csPin);
#endif

  if(rstPin<255) {
    pinMode(rstPin, OUTPUT);
    digitalWrite(rstPin, HIGH);
    delay(50);
    digitalWrite(rstPin, LOW);
    delay(5);
    digitalWrite(rstPin, HIGH);
    delay(10);
  }
  
#if USESPI==1
  SPI.begin();
#ifdef __AVR__
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  //SPI.setDataMode(SPI_MODE0);
#endif
#endif
  
  CS_ACTIVE;
  for(int i=0; i<sizeof(initData); i++) sendCmd(pgm_read_byte(initData+i));
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::gotoXY(byte x, byte y)
{
  CS_ACTIVE;
  sendCmd(HX1230_PAGE_ADDR | y);
  sendCmd(HX1230_COL_ADDR_H | (x >> 4));
  sendCmd(HX1230_COL_ADDR_L | (x & 0xf));
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::sleep(bool mode)
{
  if(mode) {
    clrScr();
    CS_ACTIVE;
    sendCmd(HX1230_DISPLAY_OFF); // power down
    sendCmd(HX1230_DISPLAY_TEST); // all pixels on (saves power)
    CS_IDLE;
  } else {
    CS_ACTIVE;
    for(int i=0; i<sizeof(initData); i++) sendCmd(pgm_read_byte(initData+i));
    CS_IDLE;
  }
}
// ----------------------------------------------------------------
// 0..31
void HX1230_SPI::setContrast(byte val)
{
  CS_ACTIVE;
	sendCmd(HX1230_CONTRAST|(val&0x1f));
  CS_IDLE;
}
// ----------------------------------------------------------------
// 0..63
void HX1230_SPI::setScroll(byte val)
{
  CS_ACTIVE;
	sendCmd(HX1230_SCAN_START_LINE|(val&0x3f));
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::setRotate(int mode)
{
  CS_ACTIVE;
  if(mode==2) { // supported rotate 180 deg only
  	sendCmd(HX1230_SEG_REMAP);
  	sendCmd(HX1230_COM_REMAP);
  } else {
  	sendCmd(HX1230_SEG_NORMAL);
  	sendCmd(HX1230_COM_NORMAL);
  }
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::displayInvert(bool mode)
{
  CS_ACTIVE;
	sendCmd(mode ? HX1230_INVERT_ON : HX1230_INVERT_OFF);
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::displayOn(bool mode)
{
  CS_ACTIVE;
	sendCmd(mode ? HX1230_DISPLAY_ON : HX1230_DISPLAY_OFF);
  CS_IDLE;
}
// ----------------------------------------------------------------
// val=HX1230_POWER_ON, HX1230_POWER_OFF, HX1230_DISPLAY_NORMAL, HX1230_DISPLAY_TEST
//     HX1230_INVERT_OFF, HX1230_INVERT_ON, HX1230_DISPLAY_ON, HX1230_DISPLAY_OFF
void HX1230_SPI::displayMode(byte val)
{
  CS_ACTIVE;
	sendCmd(val);
  CS_IDLE;
}
// ----------------------------------------------------------------
void HX1230_SPI::clrScr()
{
  gotoXY(0,0);
  CS_ACTIVE;
  for (int i=0; i<SCR_WD*(SCR_HT+7)/8/8; i++) {
    sendData(0); sendData(0); sendData(0); sendData(0);
    sendData(0); sendData(0); sendData(0); sendData(0);
  }
  CS_IDLE;
  gotoXY(0,0);
}
// ---------------------------------
#define ALIGNMENT \
  if(x==-1)\
    x = SCR_WD-wd; \
  else if(x<0) \
    x = (SCR_WD-wd)/2; \
  if(x<0) x=0; \
  if(x>=SCR_WD || y8>=(SCR_HT+7)/8) return 0; \
  if(x+wd>SCR_WD) wd = SCR_WD-x; \
  if(y8+ht8>(SCR_HT+7)/8) ht8 = (SCR_HT+7)/8-y8

int HX1230_SPI::fillWin(int x, uint8_t y8, uint8_t wd, uint8_t ht8, uint8_t val)
{
  ALIGNMENT;
  for(int i=0; i<ht8; i++) {
    gotoXY(x,y8+i);
	  CS_ACTIVE;
	  for(int j=0; j<wd; j++) sendSPI(val,DAT);
    CS_IDLE;
  }
  return x+wd;
}
// ---------------------------------
int HX1230_SPI::drawBuf(const uint8_t *bmp, int x, uint8_t y8, uint8_t wd, uint8_t ht8)
{
  uint8_t wdb = wd;
  ALIGNMENT;
  for(int i=0; i<ht8; i++) {
    gotoXY(x,y8+i);
  	CS_ACTIVE;
	  for(int x=0; x<wd; x++) sendData(*(bmp+wdb*i+x));
    //sendData(bmp+wdb*i, wd);
    CS_IDLE;
  }
  return x+wd;
}
// ---------------------------------
int HX1230_SPI::drawBitmap(const uint8_t *bmp, int x, uint8_t y8, uint8_t wd, uint8_t ht8)
{
  uint8_t wdb = wd;
  ALIGNMENT;
  for(int i=0; i<ht8; i++) {
    gotoXY(x,y8+i);
  	CS_ACTIVE;
	  for(int x=0; x<wd; x++) sendData(pgm_read_byte(bmp+wdb*i+x));
    CS_IDLE;
  }
  return x+wd;
}
// ---------------------------------
int HX1230_SPI::drawBitmap(const uint8_t *bmp, int x, uint8_t y8)
{
  uint8_t wd = pgm_read_byte(bmp+0);
  uint8_t ht = pgm_read_byte(bmp+1);
  return drawBitmap(bmp+2, x, y8, wd, ht);
}
// ---------------------------------
// text rendering stuff
// ---------------------------------
void HX1230_SPI::setFont(const uint8_t* f)
{
  font     = f;
  xSize    =-pgm_read_byte(font+0);
  ySize    = pgm_read_byte(font+1);
  firstCh  = pgm_read_byte(font+2);
  lastCh   = pgm_read_byte(font+3);
  ySize8   = (ySize+7)/8;
  minCharWd  = 0;
  minDigitWd = 0;
  cr = 0;
  invertCh = 0;
  invertMask = 0xff;
}
// ---------------------------------
// clrLine - clears screen before and after text
int HX1230_SPI::printStr(int x, uint8_t y8, char *txt, int clrLine)
{
  if(!font) return 0;
  int xpos = x;
  int ypos = y8;
  int wd = strWidth(txt);
  if(xpos==-1)
    xpos = SCR_WD-wd;  // align to right
  else if(xpos<0)
    xpos = (SCR_WD-wd)/2;  // center
  if(xpos<0) xpos=0;
  if(clrLine && xpos>0) fillWin(0,y8,xpos,ySize8,0);
  while (*txt) {
    if(*txt==10) { // \n clears area to the end of line and jumps to the next line
      fillWin(xpos,ypos,SCR_WD-xpos,ySize8,0);
      xpos = 0;
      ypos += ySize8;
      txt++;
    } else {
      int wd = printChar(xpos, ypos, *txt++);
      xpos += wd;
      if(cr && xpos>=SCR_WD) {
        xpos = 0;
        ypos += ySize8;
        if(ypos>SCR_HT/8)
          ypos = 0;
      }
    }
  }
  if(clrLine && SCR_WD-x>0) fillWin(xpos,y8,SCR_WD-xpos,ySize8,0);
  return xpos;
}
// ---------------------------------
int HX1230_SPI::strWidth(char *txt)
{
  if(!font || !txt || !*txt) return 0;
  int wd = 0;
  char ch;
  while(*txt)
    if(*txt<' ') txt++; else wd+=charWidth((ch=*txt++));
  return wd-(charWidth(ch,true)-charWidth(ch,false));
}
// ---------------------------------
bool HX1230_SPI::isNumber(uint8_t ch)
{
  return isdigit(ch) || ch==' ';
}
// ---------------------------------
bool HX1230_SPI::isNumberExt(uint8_t ch)
{
  return isdigit(ch) || ch=='-' || ch=='+' || ch=='.' || ch==' ';
}
// ---------------------------------
int HX1230_SPI::charWidth(uint8_t _ch, bool last)
{
#ifdef CONVERT_POLISH
  int ch = convertPolish(_ch);
#else
  int ch = _ch;
#endif
  if(!font || ch<firstCh || ch>lastCh) return 0;
  int idx = 4 + (ch - firstCh)*(xSize*ySize8+1);
  int wd = pgm_read_byte(font + idx);
  int wdL = 0, wdR = 1; // default spacing before and behind char
  if((*isNumberFun)(ch)) {
    if(minDigitWd>wd) {
      wdL = (minDigitWd-wd)/2;
      wdR += (minDigitWd-wd-wdL);
    }
  } else
  if(minCharWd>wd) {
    wdL = (minCharWd-wd)/2;
    wdR += (minCharWd-wd-wdL);
  }
  return last ? wd+wdL+wdR : wd+wdL+wdR-1;  // last!=0 -> get rid of last empty columns
}
// ---------------------------------
int HX1230_SPI::printChar(int x, uint8_t row, uint8_t _ch)
{
#ifdef CONVERT_POLISH
  int ch = convertPolish(_ch);
#else
  int ch = _ch;
#endif
  
  if(!font || ch<firstCh || ch>lastCh || x>=SCR_WD || row>=(SCR_HT+7)/8) return 0;

  int j,i, idx = 4 + (ch - firstCh)*(xSize*ySize8+1);
  int wd = pgm_read_byte(font + idx++);
  int wdL = 0, wdR = 1; // default spacing before and behind char
  if((*isNumberFun)(ch)) {
    if(minDigitWd>wd) {
      wdL = (minDigitWd-wd)/2;
      wdR += (minDigitWd-wd-wdL);
    }
  } else
  if(minCharWd>wd) {
    wdL = (minCharWd-wd)/2;
    wdR += (minCharWd-wd-wdL);
  }
  if(x+wd+wdL+wdR>SCR_WD) wdR = max(SCR_WD-x-wdL-wd, 0);
  if(x+wd+wdL+wdR>SCR_WD) wd = max(SCR_WD-x-wdL, 0);
  if(x+wd+wdL+wdR>SCR_WD) wdL = max(SCR_WD-x, 0);
  for(j=0; j<ySize8; j++) {
    gotoXY(x, row+j);
    CS_ACTIVE;
    if(!invertCh) {
      for(i=0; i<wdL; i++) sendSPI(0,DAT);
      for(i=0; i<wd; i++)  sendSPI(pgm_read_byte(font+idx+i*ySize8+j),DAT);
      for(i=0; i<wdR; i++) sendSPI(0,DAT);
    } else {
      for(i=0; i<wdL; i++) sendSPI(invertMask,DAT);
      for(i=0; i<wd; i++)  sendSPI(pgm_read_byte(font+idx+i*ySize8+j)^invertMask,DAT);
      for(i=0; i<wdR; i++) sendSPI(invertMask,DAT);
    }
    CS_IDLE;
  }
  return wd+wdL+wdR;
}
// ---------------------------------
unsigned char HX1230_SPI::convertPolish(unsigned char _c)
{
  unsigned char pl, c = _c;
  if(c==196 || c==197 || c==195) {
	  dualChar = c;
    return 0;
  }
  if(dualChar) { // UTF8 coding
    switch(_c) {
      case 133: pl = 1+9; break; // '¹'
      case 135: pl = 2+9; break; // 'æ'
      case 153: pl = 3+9; break; // 'ê'
      case 130: pl = 4+9; break; // '³'
      case 132: pl = dualChar==197 ? 5+9 : 1; break; // 'ñ' and '¥'
      case 179: pl = 6+9; break; // 'ó'
      case 155: pl = 7+9; break; // 'œ'
      case 186: pl = 8+9; break; // 'Ÿ'
      case 188: pl = 9+9; break; // '¿'
      //case 132: pl = 1; break; // '¥'
      case 134: pl = 2; break; // 'Æ'
      case 152: pl = 3; break; // 'Ê'
      case 129: pl = 4; break; // '£'
      case 131: pl = 5; break; // 'Ñ'
      case 147: pl = 6; break; // 'Ó'
      case 154: pl = 7; break; // 'Œ'
      case 185: pl = 8; break; // ''
      case 187: pl = 9; break; // '¯'
      default:  return c; break;
    }
    dualChar = 0;
  } else   
  switch(_c) {  // Windows coding
    case 165: pl = 1; break; // ¥
    case 198: pl = 2; break; // Æ
    case 202: pl = 3; break; // Ê
    case 163: pl = 4; break; // £
    case 209: pl = 5; break; // Ñ
    case 211: pl = 6; break; // Ó
    case 140: pl = 7; break; // Œ
    case 143: pl = 8; break; // 
    case 175: pl = 9; break; // ¯
    case 185: pl = 10; break; // ¹
    case 230: pl = 11; break; // æ
    case 234: pl = 12; break; // ê
    case 179: pl = 13; break; // ³
    case 241: pl = 14; break; // ñ
    case 243: pl = 15; break; // ó
    case 156: pl = 16; break; // œ
    case 159: pl = 17; break; // Ÿ
    case 191: pl = 18; break; // ¿
    default:  return c; break;
  }
  return pl+'~'+1;
}
// ---------------------------------
