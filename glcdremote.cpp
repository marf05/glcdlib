/*
$Id$

glcdremote Remote LCD library!

*/

#include <avr/pgmspace.h>
#include <WProgram.h>
#include <util/delay.h>
#include <stdlib.h>

#include <RF12.h>

#include "glcdremote.h"
#include "glcdremotemsgs.h"

byte msg[66];

void glcdremote::drawbitmap(byte x, byte y, 
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

void glcdremote::drawstring(byte x, byte line, char *c) {
	msg[0]=REMOTELCDDRAWSTRING;
	msg[1]=x;
	msg[2]=line;
	byte p=4;
  while (*c) msg[p++]=*c++;
	msg[3]=p-4;
	sendlcdmessage(msg,p);
}

void  glcdremote::drawchar(byte x, byte line, char c) {
	msg[0]=REMOTELCDDRAWCHAR;
	msg[1]=x;
	msg[2]=line;
	msg[3]=c;
  sendlcdmessage(msg,4);
}

// pixel vertical aligned text functions

void glcdremote::drawstringx(byte x, byte y, char *c) {
	msg[0]=REMOTELCDDRAWSTRINGX;
	msg[1]=x;
	msg[2]=y;
	byte p=4;
  while (*c) msg[p++]=*c++;
	msg[3]=p-4;
	sendlcdmessage(msg,p);
}
void  glcdremote::drawcharx(byte x, byte y, char c) {
	msg[0]=REMOTELCDDRAWCHARX;
	msg[1]=x;
	msg[2]=y;
	msg[3]=c;
  sendlcdmessage(msg,4);
}

void glcdremote::drawline(byte x0, byte y0, byte x1, byte y1, 
                      byte color) {
	msg[0]=REMOTELCDDRAWLINE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=color;
	sendlcdmessage(msg,6);
}


// draw triangle
void glcdremote::drawtriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
	msg[0]=REMOTELCDDRAWTRIANGLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=x2;
	msg[6]=y2;
	msg[7]=color;	
	sendlcdmessage(msg,8);
}

void glcdremote::filltriangle(byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color) {
	msg[0]=REMOTELCDFILLTRIANGLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=x2;
	msg[6]=y2;
	msg[7]=color;	
	sendlcdmessage(msg,8);
}

// filled rectangle
void glcdremote::fillrect(byte x, byte y, byte w, byte h, 
                      byte color) {
	msg[0]=REMOTELCDFILLRECT;
	msg[1]=x;
	msg[2]=y;
	msg[3]=w;
	msg[4]=h;
 	msg[5]=color;	
	sendlcdmessage(msg,6);
}

// draw a rectangle
void glcdremote::drawrect(byte x, byte y, byte w, byte h, 
                      byte color) {
	msg[0]=REMOTELCDDRAWRECT;
	msg[1]=x;
	msg[2]=y;
	msg[3]=w;
	msg[4]=h;
	msg[5]=color;	
	sendlcdmessage(msg,6);
}

// draw a circle outline
void glcdremote::drawcircle(byte x0, byte y0, byte r, 
                        byte color) {
	msg[0]=REMOTELCDDRAWCIRCLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=r;
	msg[4]=color;	
	sendlcdmessage(msg,5);
}

void glcdremote::fillcircle(byte x0, byte y0, byte r, byte color) {

	msg[0]=REMOTELCDFILLCIRCLE;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=r;
	msg[4]=color;	
	sendlcdmessage(msg,5);
}

// the most basic function, set a single pixel
void glcdremote::setpixel(byte x, byte y, byte color) {
	msg[0]=REMOTELCDSETPIXEL;
	msg[1]=x;
	msg[2]=y;
	msg[3]=color;	
	sendlcdmessage(msg,4);
}

void glcdremote::st7565_init(void) {
	msg[0]=REMOTELCDSETUPDATEAREA;
	msg[1]=0;
	msg[2]=0;
	msg[3]=LCDWIDTH-1;
	msg[4]=LCDHEIGHT-1;
	msg[5]=1;
	sendlcdmessage(msg,6);
}

void glcdremote::st7565_command(byte c) {
	msg[0]=REMOTELCDCOMMAND;
	msg[1]=c;
	sendlcdmessage(msg,2);
}

void glcdremote::st7565_data(byte c) {
	msg[0]=REMOTELCDDATA;
	msg[1]=c;
	sendlcdmessage(msg,2);
}

void glcdremote::st7565_set_brightness(byte val) {
	msg[0]=REMOTELCDSETBRIGHTNESS;
	msg[1]=val;
	sendlcdmessage(msg,2);
}

void glcdremote::display(void) {
	msg[0]=REMOTELCDDISPLAY;
	sendlcdmessage(msg,1);	
}

void glcdremote::setupdatearea(byte x0,byte y0,byte x1,byte y1, byte allowreduction=0) {
	msg[0]=REMOTELCDSETUPDATEAREA;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=allowreduction;	
	sendlcdmessage(msg,6);
}

void glcdremote::updatedisplayarea(byte x0,byte y0,byte x1,byte y1, byte reset=0) {
	msg[0]=REMOTELCDUPDATEAREA;
	msg[1]=x0;
	msg[2]=y0;
	msg[3]=x1;
	msg[4]=y1;
	msg[5]=reset;	
	sendlcdmessage(msg,6);
}

void glcdremote::scrollup(byte y) {
	msg[0]=REMOTELCDSCROLLUP;
	msg[1]=y;
	sendlcdmessage(msg,2);
}

void glcdremote::scrolldown(byte y) {
	msg[0]=REMOTELCDSCROLLDOWN;
	msg[1]=y;
	sendlcdmessage(msg,2);
}

// clear everything
void glcdremote::clear(void) {
  msg[0]=REMOTELCDCLEAR;
	sendlcdmessage(msg,1);
}

// clear everything
void glcdremote::clear_white(void) {
  msg[0]=REMOTELCDCLEARWHITE;
	sendlcdmessage(msg,1);
}

// this doesnt touch the buffer, just clears the display RAM - might be handy
void glcdremote::clear_display(void) {
  msg[0]=REMOTELCDCLEARBUF;
	sendlcdmessage(msg,1);
}

void glcdremote::sendlcdmessage(byte *message, byte length) {
//	Serial.print("Message ID:");
//	Serial.println(message[0],DEC);
	rf12_recvDone();
	if (not(rf12_canSend())) {
//		Serial.println(" waiting");
		while (not(rf12_canSend()))
			rf12_recvDone();
	}
/*
	if (length<5) {
		for (byte i=length;i<6;i++) msg[i]=0;
		length=5;
	}
*/

	rf12_sendStart(RemoteLCDhostID, message, length,1);

	rf12_recvDone();
/*
  for (byte i=0;i<length;i++) {
    Serial.print(message[i],DEC);
    Serial.print(",");
  }
  Serial.println("\n------------");
*/
}
