/*
GLCD_proxy Remote LCD library!
$Id$
*/

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

void GLCD_proxy::drawbitmap(byte x, byte y, 
                        const byte *bitmap, byte w, byte h,
                        byte color) {
/* Need to work out the messaging for bitmap sending
	  for (byte j=0; j<h; j++) {
	    for (byte i=0; i<w; i++ ) {
	      if (pgm_read_byte(bitmap + i + (j/8)*w) & _BV(j%8)) {
	        setpixel(x+i, y+j, color);
	      }
	    }
	  }
*/
}

void GLCD_proxy::drawstring(byte x, byte line, char *c) {
	msg[0]=REMOTELCDDRAWSTRING;
	msg[1]=x;
	msg[2]=line;
	byte p=4;
    while (*c)
        msg[p++]=*c++;
	msg[3]=p-4;
	sendlcdmessage(p);
}

void  GLCD_proxy::drawchar(byte x, byte line, char c) {
	msg[0]=REMOTELCDDRAWCHAR;
	msg[1]=x;
	msg[2]=line;
	msg[3]=c;
    sendlcdmessage(4);
}

// pixel vertical aligned text functions

void GLCD_proxy::drawstringx(byte x, byte y, char *c) {
	msg[0]=REMOTELCDDRAWSTRINGX;
	msg[1]=x;
	msg[2]=y;
	byte p=4;
    while (*c)
        msg[p++]=*c++;
    msg[3]=p-4;
	sendlcdmessage(p);
}
void  GLCD_proxy::drawcharx(byte x, byte y, char c) {
	msg[0]=REMOTELCDDRAWCHARX;
	msg[1]=x;
	msg[2]=y;
	msg[3]=c;
    sendlcdmessage(4);
}

void GLCD_proxy::drawline(byte x0, byte y0, byte x1, byte y1, 
                      byte color) {
	msg[0]=REMOTELCDDRAWLINE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=color;
	sendlcdmessage(6);
}

void GLCD_proxy::drawtriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
	msg[0]=REMOTELCDDRAWTRIANGLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=x2;
	msg[6]=y2;
	msg[7]=color;	
	sendlcdmessage(8);
}

void GLCD_proxy::filltriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
	msg[0]=REMOTELCDFILLTRIANGLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=x2;
	msg[6]=y2;
	msg[7]=color;	
	sendlcdmessage(8);
}

void GLCD_proxy::fillrect(byte x, byte y, byte w, byte h, 
                      byte color) {
	msg[0]=REMOTELCDFILLRECT;
	msg[1]=x;
	msg[2]=y;
	msg[3]=w;
	msg[4]=h;
 	msg[5]=color;	
	sendlcdmessage(6);
}

void GLCD_proxy::drawrect(byte x, byte y, byte w, byte h, 
                      byte color) {
	msg[0]=REMOTELCDDRAWRECT;
	msg[1]=x;
	msg[2]=y;
	msg[3]=w;
	msg[4]=h;
	msg[5]=color;	
	sendlcdmessage(6);
}

void GLCD_proxy::drawcircle(byte x0, byte y0, byte r, 
                        byte color) {
	msg[0]=REMOTELCDDRAWCIRCLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=r;
	msg[4]=color;	
	sendlcdmessage(5);
}

void GLCD_proxy::fillcircle(byte x0, byte y0, byte r, byte color) {

	msg[0]=REMOTELCDFILLCIRCLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=r;
	msg[4]=color;	
	sendlcdmessage(5);
}

void GLCD_proxy::setpixel(byte x, byte y, byte color) {
	msg[0]=REMOTELCDSETPIXEL;
	msg[1]=x;
	msg[2]=y;
	msg[3]=color;	
	sendlcdmessage(4);
}

void GLCD_proxy::st7565_init() {
	msg[0]=REMOTELCDSETUPDATEAREA;
	msg[1]=0;
	msg[2]=0;
	msg[3]=LCDWIDTH-1;
	msg[4]=LCDHEIGHT-1;
	msg[5]=1;
	sendlcdmessage(6);
}

void GLCD_proxy::st7565_command(byte c) {
	msg[0]=REMOTELCDCOMMAND;
	msg[1]=c;
	sendlcdmessage(2);
}

void GLCD_proxy::st7565_data(byte c) {
	msg[0]=REMOTELCDDATA;
	msg[1]=c;
	sendlcdmessage(2);
}

void GLCD_proxy::st7565_set_brightness(byte val) {
	msg[0]=REMOTELCDSETBRIGHTNESS;
	msg[1]=val;
	sendlcdmessage(2);
}

void GLCD_proxy::display() {
	msg[0]=REMOTELCDDISPLAY;
	sendlcdmessage(1);	
}

void GLCD_proxy::setupdatearea(byte x0,byte y0,byte x1,byte y1, byte allowreduction) {
	msg[0]=REMOTELCDSETUPDATEAREA;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=allowreduction;	
	sendlcdmessage(6);
}

void GLCD_proxy::updatedisplayarea(byte x0,byte y0,byte x1,byte y1, byte reset) {
	msg[0]=REMOTELCDUPDATEAREA;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=reset;	
	sendlcdmessage(6);
}

void GLCD_proxy::scrollup(byte y) {
	msg[0]=REMOTELCDSCROLLUP;
	msg[1]=y;
	sendlcdmessage(2);
}

void GLCD_proxy::scrolldown(byte y) {
	msg[0]=REMOTELCDSCROLLDOWN;
	msg[1]=y;
	sendlcdmessage(2);
}

void GLCD_proxy::scrollleft(byte x) {
	msg[0]=REMOTELCDSCROLLLEFT;
	msg[1]=x;
	sendlcdmessage(2);
}

void GLCD_proxy::scrollright(byte x) {
	msg[0]=REMOTELCDSCROLLRIGHT;
	msg[1]=x;
	sendlcdmessage(2);
}

void GLCD_proxy::clear() {
    msg[0]=REMOTELCDCLEAR;
	sendlcdmessage(1);
}

void GLCD_proxy::clear_white() {
    msg[0]=REMOTELCDCLEARWHITE;
	sendlcdmessage(1);
}

// this doesnt touch the buffer, just clears the display RAM - might be handy
void GLCD_proxy::clear_display() {
    msg[0]=REMOTELCDCLEARBUF;
	sendlcdmessage(1);
}

void GLCD_proxy::sendlcdmessage(byte length) {
	while (!rf12_canSend())
		rf12_recvDone();
	rf12_sendStart(remoteId, msg, length, 1);
}
