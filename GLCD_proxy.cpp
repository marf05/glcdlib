// GLCD_proxy Remote LCD library.
// $Id$

#include <avr/pgmspace.h>
#include <WProgram.h>
#include <util/delay.h>
#include <stdlib.h>
#include <RF12.h>
#include "GLCD_proxy.h"
#include "GLCD_proxymsgs.h"

static byte msg[66];

void GLCD_proxy::begin() {
    clear();    
}

void GLCD_proxy::drawBitmap(byte x, byte y, 
                        const byte *bitmap, byte w, byte h,
                        byte color) {
/* Need to work out the messaging for bitmap sending
	  for (byte j=0; j<h; j++) {
	    for (byte i=0; i<w; i++ ) {
	      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
	        setPixel(x+i, y+j, color);
	      }
	    }
	  }
*/
}

void GLCD_proxy::backLight(byte level) {
	  msg[0]=REMOTE_GLCD_BACKLIGHT;
    sendLCDMessage(1);
}

void GLCD_proxy::drawString(byte x, byte line, char *c) {
  	msg[0]=REMOTE_GLCD_DRAWSTRING;
  	msg[1]=x;
  	msg[2]=line;
  	byte p=4;
    while (*c)
        msg[p++]=*c++;
  	msg[3]=p-4;
  	sendLCDMessage(p);
}

void GLCD_proxy::drawString_p(byte x, byte line, char *c) {
  	msg[0]=REMOTE_GLCD_DRAWSTRING;
  	msg[1]=x;
  	msg[2]=line;
  	byte p=4;
    for (;;) {
        char ch = pgm_read_byte(c++);
        if (ch == 0)
            break;
        msg[p++]=ch;
    }
  	msg[3]=p-4;
  	sendLCDMessage(p);
}

void  GLCD_proxy::drawChar(byte x, byte line, char c) {
  	msg[0]=REMOTE_GLCD_DRAWCHAR;
  	msg[1]=x;
  	msg[2]=line;
  	msg[3]=c;
    sendLCDMessage(4);
}

void GLCD_proxy::drawLine(byte x0, byte y0, byte x1, byte y1, byte color) {
  	msg[0]=REMOTE_GLCD_DRAWLINE;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=x1;
  	msg[4]=y1;
  	msg[5]=color;
  	sendLCDMessage(6);
}

void GLCD_proxy::drawTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
  	msg[0]=REMOTE_GLCD_DRAWTRIANGLE;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=x1;
  	msg[4]=y1;
  	msg[5]=x2;
  	msg[6]=y2;
  	msg[7]=color;	
  	sendLCDMessage(8);
}

void GLCD_proxy::fillTriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
  	msg[0]=REMOTE_GLCD_FILLTRIANGLE;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=x1;
  	msg[4]=y1;
  	msg[5]=x2;
  	msg[6]=y2;
  	msg[7]=color;	
  	sendLCDMessage(8);
}

void GLCD_proxy::fillRect(byte x, byte y, byte w, byte h, byte color) {
  	msg[0]=REMOTE_GLCD_FILLRECT;
  	msg[1]=x;
  	msg[2]=y;
  	msg[3]=w;
  	msg[4]=h;
   	msg[5]=color;	
  	sendLCDMessage(6);
}

void GLCD_proxy::drawRect(byte x, byte y, byte w, byte h, byte color) {
  	msg[0]=REMOTE_GLCD_DRAWRECT;
  	msg[1]=x;
  	msg[2]=y;
  	msg[3]=w;
  	msg[4]=h;
  	msg[5]=color;	
  	sendLCDMessage(6);
}

void GLCD_proxy::drawCircle(byte x0, byte y0, byte r, byte color) {
  	msg[0]=REMOTE_GLCD_DRAWCIRCLE;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=r;
  	msg[4]=color;	
  	sendLCDMessage(5);
}

void GLCD_proxy::fillCircle(byte x0, byte y0, byte r, byte color) {
  	msg[0]=REMOTE_GLCD_FILLCIRCLE;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=r;
  	msg[4]=color;	
  	sendLCDMessage(5);
}

void GLCD_proxy::setPixel(byte x, byte y, byte color) {
  	msg[0]=REMOTE_GLCD_SETPIXEL;
  	msg[1]=x;
  	msg[2]=y;
  	msg[3]=color;	
  	sendLCDMessage(4);
}

void GLCD_proxy::display() {
  	msg[0]=REMOTE_GLCD_DISPLAY;
  	sendLCDMessage(1);	
}

void GLCD_proxy::setUpdateArea(byte x0,byte y0,byte x1,byte y1, byte allowReduction) {
  	msg[0]=REMOTE_GLCD_SETUPDATEAREA;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=x1;
  	msg[4]=y1;
  	msg[5]=allowReduction;	
  	sendLCDMessage(6);
}

void GLCD_proxy::updateDisplayArea(byte x0,byte y0,byte x1,byte y1, byte reset) {
  	msg[0]=REMOTE_GLCD_UPDATEAREA;
  	msg[1]=x0;
  	msg[2]=y0;
  	msg[3]=x1;
  	msg[4]=y1;
  	msg[5]=reset;	
  	sendLCDMessage(6);
}

void GLCD_proxy::scroll(byte direction,byte y) {
  	msg[0]=REMOTE_GLCD_SCROLL;
  	msg[1]=direction;
  	msg[2]=y;
  	sendLCDMessage(3);
}

void GLCD_proxy::clear() {
    msg[0]=REMOTE_GLCD_CLEAR;
	  sendLCDMessage(1);
}

void GLCD_proxy::clearWhite() {
    msg[0]=REMOTE_GLCD_CLEARWHITE;
	  sendLCDMessage(1);
}

// this doesnt touch the buffer, just clears the display RAM - might be handy
void GLCD_proxy::clearDisplay() {
    msg[0]=REMOTE_GLCD_CLEARBUF;
	  sendLCDMessage(1);
}

void GLCD_proxy::sendLCDMessage(byte length) {
	  while (!rf12_canSend())
		    rf12_recvDone();
	  rf12_sendStart(remoteId, msg, length, 1);
}
