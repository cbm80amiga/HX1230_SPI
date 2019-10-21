# HX1230_SPI
Fast and low resource 9-bit SPI library for HX1230 96x68 display (aka Nokia 1202, STE2007)

Doesn't use RAM as framebuffer - renders everything directly to LCD framebuffer

## Connections

|HX1230 pin|Pin name| Arduino|
|--|--|--|
|#1| RST    |D6 or any digital|
|#2| CE  |D7 or any digital|
|#3| N/C     ||
|#4| DIN | D11/MOSI |
|#5| CLK  |D13/SCK|
|#6| VCC| 3.3V|
|#7| BL| 3.3V or via resistor to any digital pin|
|#8 |GND|GND|
