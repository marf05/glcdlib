/*
$Id$

ST7565 LCD library!

Copyright (C) 2010 Limor Fried, Adafruit Industries

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

// some of this code was written by <cstone@pobox.com> originally; it is in the public domain.
*/

#include <WProgram.h>
#include "GLCD_ST7565_commands.h"

#define BLACK					0
#define WHITE					1

#define LCDWIDTH				128
#define LCDHEIGHT				64
#define LCDUNUSEDSTARTBYTES 4

#define SCROLLUP 			1
#define SCROLLDOWN		2
#define SCROLLLEFT		3
#define SCROLLRIGHT		4

class GLCD_ST7565 {
 public:
  GLCD_ST7565 () {}

  void begin();
  void backLight(byte level);
  void clearDisplay(void);
  void clearWhite();
  void clear();
  void display();

  void setPixel   (uint8_t x , uint8_t y   , uint8_t color);
  void fillCircle (uint8_t x0, uint8_t y0  , uint8_t r , uint8_t color);
  void drawCircle (uint8_t x0, uint8_t y0  , uint8_t r , uint8_t color);
  void drawRect   (uint8_t x , uint8_t y   , uint8_t w , uint8_t h , uint8_t color);
  void fillRect   (uint8_t x , uint8_t y   , uint8_t w , uint8_t h , uint8_t color);
  void drawLine   (uint8_t x0, uint8_t y0  , uint8_t x1, uint8_t y1, uint8_t color);
  void drawChar   (uint8_t x , uint8_t line, char c );
  void drawString (uint8_t x , uint8_t line, char *c);
  void drawString_P (byte x, byte line, char *c);
  void drawTriangle (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t colour);
  void fillTriangle (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t colour);
  void updateDisplayArea(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t reset);
  void setUpdateArea(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1, uint8_t allowReduction);
		
  void drawBitmap (uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color);
 
  void drawCharX(uint8_t x, uint8_t y, char c);
  void drawStringX(uint8_t x , uint8_t y, char *c);
  void drawStringX_P (byte x, byte y, char *c);
	void scroll(byte direction,uint8_t pixels);  

 protected:
  void st7565_Init(void);
  void st7565_Command(uint8_t c);
  void st7565_Data(uint8_t c);
  void st7565_Set_Brightness(uint8_t val);

 private:
  void SPIWrite(uint8_t c);
  void setPixelNoRegionTrack (uint8_t x, uint8_t y, uint8_t color);
  void drawTriangleLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t firstLine, uint8_t *points,
                      uint8_t color);
  void scrollUp(uint8_t y);
  void scrollDown(uint8_t y);
  void scrollLeft(uint8_t x);
  void scrollRight(uint8_t x);
  
};
