// orig comments at end of file

#include <avr/pgmspace.h>
#include <WProgram.h>
#include <util/delay.h>
#include <stdlib.h>
#include "GLCD_ST7565.h"

#define PIN_SID  14
#define PIN_SCLK 4
#define PIN_A0   17
#define PIN_RST  7

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

// a 5x7 font table
extern byte PROGMEM font5x7[];

// the memory buffer for the LCD
byte gLCDbuf[1024];

// Switches from fast direct bit flipping to slower Arduino bit writes.
//#define slowspi

// This makes the library track where changes have occurred and only update the smallest rectangle required
// If you are writing direct to the gLCDbuf you will either need to turn this off, or call setupdatearea() with the
// area you have been working on so it can track the changes.
#define enablepartialupdate

// moves some of the enablepartialupdate logic further up the procedure tree. Removing it from setpixel and putting
// it into the higher level routines such as draw/fill rect/circle/triangle. Makes the sketch larger, but faster in
// graphical intensive applications.
#define tradesizeforspeed  // This needs enablepartialupdate


#ifdef enablepartialupdate
	byte xupdatemin;
	byte xupdatemax;
	byte yupdatemin;
	byte yupdatemax;
#endif

void GLCD_ST7565::begin() {
    st7565_init();
    st7565_command(CMD_DISPLAY_ON);
    st7565_command(CMD_SET_ALLPTS_NORMAL);
    st7565_set_brightness(0x15);
    clear();
}

void GLCD_ST7565::drawbitmap(byte x, byte y, 
                        const byte *bitmap, byte w, byte h,
                        byte color) {
  for (byte j=0; j<h; j++) {
    for (byte i=0; i<w; i++ ) {
      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
        setpixel(x+i, y+j, color);
      }
    }
  }
}

void GLCD_ST7565::drawstring(byte x, byte line, char *c) {
  while (*c) {
    drawchar(x, line, *c++);
    x += 6; // 6 pixels wide
    if (x + 6 >= LCDWIDTH) {
      x = 0;    // ran out of this line
      if (++line >= (LCDHEIGHT/8))
        return; // ran out of space :(
    }
  }
}

void  GLCD_ST7565::drawchar(byte x, byte line, char c) {
#ifdef enablepartialupdate
    if (x<xupdatemin) xupdatemin=x;
    if ((line<<3)<yupdatemin) yupdatemin=(line<<3);
    if (((line<<3)+6)>yupdatemax) yupdatemax=(line<<3)+6;
    if ((x+4)>xupdatemax) xupdatemax=x+4;
#endif
    byte* p = gLCDbuf + x + (line * 128);
    for (byte i =0; i<5; i++ )
        *p++ = pgm_read_byte(font5x7+(c*5)+i);
}

void GLCD_ST7565::drawstringx(byte x, byte y, char *c) {
	if ((y&7)==0) drawstring(x,(y>>3),c);  
  while (*c) {
    drawcharx(x, y, *c++);
    x += 6; // 6 pixels wide
    if (x + 6 >= LCDWIDTH) {
      x = 0;    // ran out of this line
      if ((y+8) >= LCDHEIGHT)
        return; // ran out of space :(
      else y=y+8;
    }
  }
}


void  GLCD_ST7565::drawcharx(byte x, byte y, char c) {
		if ((y&7)==0) drawchar(x,(y>>3),c);
		else if ((y+8)<LCDHEIGHT) {

			
#ifdef enablepartialupdate
	    if (x<xupdatemin) xupdatemin=x;
	    if (y<yupdatemin) yupdatemin=y;
	    if ((y+6)>yupdatemax) yupdatemax=y+6;
	    if ((x+4)>xupdatemax) xupdatemax=x+4;
#endif
	    byte line=y>>3;
			byte oldbyte;
			byte topmask=(0xff<<(8-y&7));
			byte bottommask=(0xff>>(y&7));
			byte* p = gLCDbuf + x + (line * 128);
	    for (byte i =0; i<5; i++ ) {
	      oldbyte=*p & topmask;
	      *p = oldbyte | (pgm_read_byte(font5x7+(c*5)+i)>>(y&7));
	      oldbyte=*(p+128) & bottommask;
	      *(p+128) = oldbyte | (pgm_read_byte(font5x7+(c*5)+i)<<(8-y&7));
	      p++;
	    }
	  }
}



// bresenham's algorithm - thx wikpedia <-- Pity you didn't quite get it right ;-)
void GLCD_ST7565::drawline(byte x0, byte y0, byte x1, byte y1, 
                      byte color) {
#ifdef tradesizeforspeed
	if (x0>xupdatemax) xupdatemax=x0;
	if (x0<xupdatemin) xupdatemin=x0;
	if (x1>xupdatemax) xupdatemax=x1;
	if (x1<xupdatemin) xupdatemin=x1;
		
	if (y0>yupdatemax) yupdatemax=y0;
	if (y0<yupdatemin) yupdatemin=y0;
	if (y1>yupdatemax) yupdatemax=y1;
	if (y1<yupdatemin) yupdatemin=y1;
#endif

  byte steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  byte dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
#ifdef tradesizeforspeed
    if (steep) {
      setpixelnoregiontrack(y0, x0, color);
    } else {
      setpixelnoregiontrack(x0, y0, color);
    }
#else
    if (steep) {
      setpixel(y0, x0, color);
    } else {
      setpixel(x0, y0, color);
    }
#endif
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// draw triangle
void GLCD_ST7565::drawtriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte colour) {
	drawline(x0,y0,x1,y1,colour);
	drawline(x1,y1,x2,y2,colour);
	drawline(x2,y2,x0,y0,colour);
}

void GLCD_ST7565::filltriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte colour) {
	byte points[LCDHEIGHT]; // 64 bytes taken to store line points needed for fill.
	if (y2<y1) { swap(y1,y2); swap(x1,x2); } // first we need to find the highest and lowest point
	if (y1<y0) { swap(y1,y0); swap(x1,x0); } // a little unrolled bubble will do for 3 points
	if (y2<y1) { swap(y1,y2); swap(x1,x2); } // Now y0 is the top and y2 is the bottom.
		
	drawtriangleline(x0,y0,x2,y2,1,points,colour); // The longest(vertical) line generate the entries in points array
	drawtriangleline(x0,y0,x1,y1,0,points,colour); // the other lines do the drawing
	drawtriangleline(x1,y1,x2,y2,0,points,colour);
}

void GLCD_ST7565::drawtriangleline(byte x0, byte y0, byte x1, byte y1, byte firstline, byte *points,
                      byte color) {
#ifdef tradesizeforspeed
	if (x0>xupdatemax) xupdatemax=x0;
	if (x0<xupdatemin) xupdatemin=x0;
	if (x1>xupdatemax) xupdatemax=x1;
	if (x1<xupdatemin) xupdatemin=x1;
		
	if (y0>yupdatemax) yupdatemax=y0;
	if (y0<yupdatemin) yupdatemin=y0;
	if (y1>yupdatemax) yupdatemax=y1;
	if (y1<yupdatemin) yupdatemin=y1;
#endif

  byte steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  byte dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;}

  for (; x0<=x1; x0++) {
    if (steep) {
      if (firstline==1) {
      	if ((x0>=0) && (x0<LCDHEIGHT)) points[x0]=y0;
      }
      else
	      drawline(y0,x0,points[x0],x0,color);
    } else {
      if (firstline==1) {
      	if ((y0>=0) && (y0<LCDHEIGHT)) points[y0]=x0;
      }
      else
	      drawline(x0,y0,points[y0],y0,color);
		}
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}


// filled rectangle
void GLCD_ST7565::fillrect(byte x, byte y, byte w, byte h, 
                      byte color) {
  // stupidest version - just pixels - but fast with internal buffer!
#ifdef tradesizeforspeed
	if (x<xupdatemin) xupdatemin=x;
	if (y<yupdatemin) yupdatemin=y;
	if ((x+w)>xupdatemax) xupdatemax=x+w;
	if ((y+h)>yupdatemax) yupdatemax=y+h;
#endif

  for (byte i=x; i<x+w; i++) {
    for (byte j=y; j<y+h; j++) {
#ifdef tradesizeforspeed
			setpixelnoregiontrack(i,j,color);
#else
      setpixel(i, j, color);
#endif
    }
  }
}

// draw a rectangle
void GLCD_ST7565::drawrect(byte x, byte y, byte w, byte h, 
                      byte color) {
  // stupidest version - just pixels - but fast with internal buffer!
#ifdef tradesizeforspeed
	if (x<xupdatemin) xupdatemin=x;
	if (y<yupdatemin) yupdatemin=y;
	if ((x+w)>xupdatemax) xupdatemax=x+w;
	if ((y+h)>yupdatemax) yupdatemax=y+h;	
#endif

  for (byte i=x; i<x+w; i++) {
#ifdef tradesizeforspeed
		setpixelnoregiontrack(i, y, color);
		setpixelnoregiontrack(i, y+h-1, color);
#else			
    setpixel(i, y, color);
    setpixel(i, y+h-1, color);
#endif
  }

  for (byte i=y; i<y+h; i++) {
#ifdef tradesizeforspeed
		setpixelnoregiontrack(x, i, color);
		setpixelnoregiontrack(x+w-1, i, color);
#else
    setpixel(x, i, color);
    setpixel(x+w-1, i, color);
#endif
  } 
}

// draw a circle outline
void GLCD_ST7565::drawcircle(byte x0, byte y0, byte r, 
                        byte color) {
#ifdef tradesizeforspeed
	if ((x0-r)<xupdatemin) xupdatemin=(x0-r);
	if ((y0-r)<yupdatemin) yupdatemin=(y0-r);
	if ((x0+r)>xupdatemax) xupdatemax=(x0+r);
	if ((y0+r)>yupdatemax) yupdatemax=(y0+r);	
#endif

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

#ifdef tradesizeforspeed
  setpixelnoregiontrack(x0, y0+r, color);
  setpixelnoregiontrack(x0, y0-r, color);
  setpixelnoregiontrack(x0+r, y0, color);
  setpixelnoregiontrack(x0-r, y0, color);
#else
  setpixel(x0, y0+r, color);
  setpixel(x0, y0-r, color);
  setpixel(x0+r, y0, color);
  setpixel(x0-r, y0, color);
#endif

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
#ifdef tradesizeforspeed
		setpixelnoregiontrack(x0 + x, y0 + y, color);
    setpixelnoregiontrack(x0 - x, y0 + y, color);
    setpixelnoregiontrack(x0 + x, y0 - y, color);
    setpixelnoregiontrack(x0 - x, y0 - y, color);
    
    setpixelnoregiontrack(x0 + y, y0 + x, color);
    setpixelnoregiontrack(x0 - y, y0 + x, color);
    setpixelnoregiontrack(x0 + y, y0 - x, color);
    setpixelnoregiontrack(x0 - y, y0 - x, color);
#else
    setpixel(x0 + x, y0 + y, color);
    setpixel(x0 - x, y0 + y, color);
    setpixel(x0 + x, y0 - y, color);
    setpixel(x0 - x, y0 - y, color);
    
    setpixel(x0 + y, y0 + x, color);
    setpixel(x0 - y, y0 + x, color);
    setpixel(x0 + y, y0 - x, color);
    setpixel(x0 - y, y0 - x, color);
#endif    
  }
}

void GLCD_ST7565::fillcircle(byte x0, byte y0, byte r, byte color) {

#ifdef tradesizeforspeed
	if ((x0-r)<xupdatemin) xupdatemin=(x0-r);
	if ((y0-r)<yupdatemin) yupdatemin=(y0-r);
	if ((x0+r)>xupdatemax) xupdatemax=(x0+r);
	if ((y0+r)>yupdatemax) yupdatemax=(y0+r);	
#endif

  int8_t f = 1 - r;
  int8_t ddF_x = 1;
  int8_t ddF_y = -2 * r;
  int8_t x = 0;
  int8_t y = r;

  for (byte i=y0-r; i<=y0+r; i++) {
    setpixel(x0, i, color);
  }

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    for (byte i=y0-y; i<=y0+y; i++) {
#ifdef tradesizeforspeed
      setpixelnoregiontrack(x0+x, i, color);
      setpixelnoregiontrack(x0-x, i, color);
#else
      setpixel(x0+x, i, color);
      setpixel(x0-x, i, color);
#endif
    } 
    for (byte i=y0-x; i<=y0+x; i++) {
#ifdef tradesizeforspeed
      setpixelnoregiontrack(x0+y, i, color);
      setpixelnoregiontrack(x0-y, i, color);
#else
      setpixel(x0+y, i, color);
      setpixel(x0-y, i, color);
#endif
    }    
  }
}

// the most basic function, set a single pixel
void GLCD_ST7565::setpixel(byte x, byte y, byte color) {
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    gLCDbuf[x+ (y/8)*128] |=  _BV(7-(y%8));  
  else
    gLCDbuf[x+ (y/8)*128] &= ~_BV(7-(y%8));

	#ifdef enablepartialupdate
		if (x<xupdatemin) xupdatemin=x;
		if (x>xupdatemax) xupdatemax=x;
		if (y<yupdatemin) yupdatemin=y;
		if (y>yupdatemax) yupdatemax=y;
	#endif
}

void GLCD_ST7565::setpixelnoregiontrack(byte x, byte y, byte color) {
#ifdef tradesizeforspeed
  if ((x >= LCDWIDTH) || (y >= LCDHEIGHT))
    return;

  // x is which column
  if (color) 
    gLCDbuf[x+ (y/8)*128] |=  _BV(7-(y%8));  
  else
    gLCDbuf[x+ (y/8)*128] &= ~_BV(7-(y%8));
#endif
}

void GLCD_ST7565::st7565_init(void) {
  // set pin directions
  pinMode(PIN_SID,  OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  pinMode(PIN_A0,   OUTPUT);
  pinMode(PIN_RST,  OUTPUT);

  digitalWrite(PIN_RST, LOW);
  _delay_ms(500);
  digitalWrite(PIN_RST, HIGH);

  st7565_command(CMD_SET_BIAS_7);
  st7565_command(CMD_SET_ADC_NORMAL);
  st7565_command(CMD_SET_COM_NORMAL);
  st7565_command(CMD_SET_DISP_START_LINE);
  st7565_command(CMD_SET_POWER_CONTROL);
  st7565_command(CMD_SET_RESISTOR_RATIO);
//  st7565_command(CMD_SET_BIAS_9);

	#ifdef enablepartialupdate
		xupdatemax=0;
		yupdatemax=0;
		xupdatemin=LCDWIDTH-1;
		yupdatemin=LCDHEIGHT-1;
	#endif
}

inline void GLCD_ST7565::spiwrite(byte c) {
#ifdef slowspi
  shiftOut(PIN_SID, PIN_SCLK, MSBFIRST, c);
#else
  for (byte mask = 0x80; mask != 0; mask >>= 1) {
    bitWrite(PORTC, 0, c & mask);
    // this is 15% faster, but it's too fast for this chip...
    //PIND = bit(4);
    //PIND = bit(4);
    // even plain set/clear is too fast, so slow down a bit more
    bitSet(PORTD, 4);
    bitSet(PORTD, 4);
    bitClear(PORTD, 4);
    bitClear(PORTD, 4);
  }
#endif
}

void GLCD_ST7565::st7565_command(byte c) {
#ifdef slowspi
  digitalWrite(PIN_A0, LOW);
#else
  bitClear(PORTC, 3);
#endif
  spiwrite(c);
}

void GLCD_ST7565::st7565_data(byte c) {
#ifdef slowspi
  digitalWrite(PIN_A0, HIGH);
#else
  bitSet(PORTC, 3);
#endif
  spiwrite(c);
}

void GLCD_ST7565::st7565_set_brightness(byte val) {
    st7565_command(CMD_SET_VOLUME_FIRST);
    st7565_command(CMD_SET_VOLUME_SECOND | (val & 0x3F));
}

void GLCD_ST7565::display(void) {
  byte c, p;
#ifdef enablepartialupdate
	if (xupdatemin<=xupdatemax) {
		for (p=(yupdatemin>>3);p<=(yupdatemax>>3);p++) {
	    st7565_command(CMD_SET_PAGE | (7-p));
	    st7565_command(CMD_SET_COLUMN_UPPER | (((xupdatemin+LCDUNUSEDSTARTBYTES) >> 4) & 0x0F));
	    st7565_command(CMD_SET_COLUMN_LOWER | ((xupdatemin+LCDUNUSEDSTARTBYTES) & 0x0F));
	    st7565_command(CMD_RMW);
//	    st7565_data(0xFF); // ummm, not sure what this is for!
// With column offset set to 4 you don't need or want this.
	    for(c = xupdatemin; c <=xupdatemax; c++) {
	      st7565_data(gLCDbuf[(128*p)+c]);
	    }
		}
		xupdatemax=0;			// now we have performed the screen update, reset the redraw region to nothing.
		yupdatemax=0;
		xupdatemin=LCDWIDTH-1;
		yupdatemin=LCDHEIGHT-1;
	}
#else
  for(p = 0; p < 8; p++) {
    st7565_command(CMD_SET_PAGE | (7-p));
    st7565_command(CMD_SET_COLUMN_LOWER | ( LCDUNUSEDSTARTBYTES & 0x0F));
    st7565_command(CMD_SET_COLUMN_UPPER | ((LCDUNUSEDSTARTBYTES >> 4) & 0x0F));
    st7565_command(CMD_RMW);
//    st7565_data(0xFF); // This seems only to be added as padding which wouldn't appear on screen
// With column offset set to 4 you don't need or want this.
    for(c = 0; c < 128; c++) {
      st7565_data(gLCDbuf[(128*p)+c]);
    }
  }
#endif
}

void GLCD_ST7565::setupdatearea(byte x0,byte y0,byte x1,byte y1, byte allowreduction=0) {
#ifdef enablepartialupdate
	if ((x0==0xff) && (allowreduction!=0)) {
		xupdatemax=0;			// reset area to nothing
		yupdatemax=0;
		xupdatemin=LCDWIDTH-1;
		yupdatemin=LCDHEIGHT-1;		
	}
	else {
		if (x0>x1) swap(x0,x1);
		if (y0>y1) swap(y0,y1);
	
		if (allowreduction!=0) {
			xupdatemin=x0;
			xupdatemax=x1;
			yupdatemin=y0;
			yupdatemax=y1;
		}
		else {
			if (x0<xupdatemin) xupdatemin=x0;
			if (x1>xupdatemax) xupdatemax=x1;
			if (y0<yupdatemin) yupdatemin=y0;
			if (y1>yupdatemax) yupdatemax=y1;
		}
	}
#endif
}

void GLCD_ST7565::updatedisplayarea(byte x0,byte y0,byte x1,byte y1, byte reset=0) {
#ifdef enablepartialupdate
  byte c, p;
	if ((x0<=x1) && (y0<=y1)) {
		for (p=(y0>>3);p<=(y1>>3);p++) {
	    st7565_command(CMD_SET_PAGE | (7-p));
	    st7565_command(CMD_SET_COLUMN_UPPER | (((x0+LCDUNUSEDSTARTBYTES) >> 4) & 0x0F));
	    st7565_command(CMD_SET_COLUMN_LOWER | ((x0+LCDUNUSEDSTARTBYTES) & 0x0F));
	    st7565_command(CMD_RMW);
	    for(c = x0; c <=x1; c++) {
	      st7565_data(gLCDbuf[(128*p)+c]);
	    }
		}
	}
	if ((reset!=0) || ((x0<=xupdatemin) && (x1>=xupdatemax) && (y0<=yupdatemin) && (y1>=yupdatemax))) {
		xupdatemax=0;			// if reset set, or the area painted covers the current region, then clear the partial update region
		yupdatemax=0;
		xupdatemin=LCDWIDTH-1;
		yupdatemin=LCDHEIGHT-1;		
	}
#endif
}

// clear everything
void GLCD_ST7565::clear(void) {
    memset(gLCDbuf, 0x00, 1024);
		#ifdef enablepartialupdate
			xupdatemin=0;	// set the partial update region to the whole screen
			yupdatemin=0;
			xupdatemax=LCDWIDTH-1;
			yupdatemax=LCDHEIGHT-1;
		#endif
}


// clear everything
void GLCD_ST7565::clear_white(void) {
    memset(gLCDbuf, 0XFF, 1024);
		#ifdef enablepartialupdate
			xupdatemin=0;	// set the partial update region to the whole screen
			yupdatemin=0;
			xupdatemax=LCDWIDTH-1;
			yupdatemax=LCDHEIGHT-1;
		#endif
}


// this doesnt touch the buffer, just clears the display RAM - might be handy
void GLCD_ST7565::clear_display(void) {
  byte p, c;
  
  for(p = 0; p < 8; p++) {
    st7565_command(CMD_SET_PAGE | p);
    for(c = LCDUNUSEDSTARTBYTES; c <(LCDWIDTH+LCDUNUSEDSTARTBYTES); c++) {
      st7565_command(CMD_SET_COLUMN_LOWER | (c & 0x0F));
      st7565_command(CMD_SET_COLUMN_UPPER | ((c >> 4) & 0x0F));
      st7565_data(0x00);
    }     
  }
}

void GLCD_ST7565::scrollup(byte y) {
	#ifdef enablepartialupdate
		xupdatemin=0;	// set the partial update region to the whole screen
		yupdatemin=0;
		xupdatemax=LCDWIDTH-1;
		yupdatemax=LCDHEIGHT-1;
	#endif
	if (y>7) {     // easy one, whole line scroll. This routine is used to do the whole line jumps.
		for (byte l=0;l<8-(y>>3);l++) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				*(p+x)=*(p+(y>>3)*128+x);
			}
		}
		for (byte l=8-(y>>3);l<8;l++) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				*(p+x)=0;
			}
		}
	}
	if ((y&7)!=0) {			// And now the fine scroll bit
		byte bits=y&7;
		for (byte l=0;l<7;l++) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				byte top=*(p+x)<<bits;
				byte bottom=*(p+x+128)>>(8-bits);
				*(p+x)=top | bottom;								
			}			
		}	
		byte* p = gLCDbuf +(7 * 128);
		for (byte x=0;x<LCDWIDTH;x++) {
			*(p+x)=*(p+x)<<bits;
		}
	}
}

void GLCD_ST7565::scrolldown(byte y) {
	#ifdef enablepartialupdate
		xupdatemin=0;	// set the partial update region to the whole screen
		yupdatemin=0;
		xupdatemax=LCDWIDTH-1;
		yupdatemax=LCDHEIGHT-1;
	#endif
	if (y>7) {     // easy one, whole line scroll.
		for (byte l=7;l>=(y>>3);l--) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				*(p+x)=*(p-(y>>3)*128+x);
			}
		}
		for (byte l=0;l<(y>>3);l++) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				*(p+x)=0;
			}
		}
	}
	if ((y&7)!=0) {			// And now the fine scroll bit
		byte bits=y&7;
		for (byte l=7;l>0;l--) {
			byte* p = gLCDbuf +(l * 128);
			for (byte x=0;x<LCDWIDTH;x++) {
				byte bottom=*(p+x)>>bits;
				byte top=*(p+x-128)<<(8-bits);
				*(p+x)=top | bottom;								
			}			
		}	
		byte* p = gLCDbuf;
		for (byte x=0;x<LCDWIDTH;x++) {
			*(p+x)=*(p+x)>>bits;
		}
	}
}

void GLCD_ST7565::scrollleft(byte x) {
	#ifdef enablepartialupdate
		xupdatemin=0;	// set the partial update region to the whole screen
		yupdatemin=0;
		xupdatemax=LCDWIDTH-1;
		yupdatemax=LCDHEIGHT-1;
	#endif
	for (byte l=0;l<=7;l++) {
		byte* p = gLCDbuf +(l * 128);
		for (byte b=0;b<(LCDWIDTH-x);b++) {
			*(p+b)=*(p+b+x);
		}
		for (byte b=(LCDWIDTH-x);b<LCDWIDTH;b++) {
			*(p+b)=0;
		}
	}
}

void GLCD_ST7565::scrollright(byte x) {
	#ifdef enablepartialupdate
		xupdatemin=0;	// set the partial update region to the whole screen
		yupdatemin=0;
		xupdatemax=LCDWIDTH-1;
		yupdatemax=LCDHEIGHT-1;
	#endif
	for (byte l=0;l<=7;l++) {
		byte* p = gLCDbuf +(l * 128);
		for (byte b=(LCDWIDTH-1);b>=x;b--) {
			*(p+b)=*(p+b-x);
		}
		for (byte b=0;b<x;b++) {
			*(p+b)=0;
		}
	}
}

/*
$Id$

GLCD_ST7565 LCD library!

High speed SPI by Jean-Claude Wippler
Partial screen update features, correction to Bresenham implimentation, triangle, and pixel vertical aligned
text functions by Steve Evans

Original library Copyright (C) 2010 Limor Fried, Adafruit Industries

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
