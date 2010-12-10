/*
$Id$

GLCD_proxy Remote LCD library!

*/

#include <WProgram.h>

#define BLACK					0
#define WHITE					1

#define LCDWIDTH				128
#define LCDHEIGHT				64

class GLCD_proxy {
    byte remoteId;
	void sendlcdmessage(byte length);  
	
public:
    GLCD_proxy(byte nodeId) : remoteId (nodeId) {}

    void begin();
    void backlight(byte level);
    void clear_display();
    void clear_white();
    void clear();
    void display();

    void setpixel   (byte x, byte y, byte color);
    void fillcircle (byte x0, byte y0, byte r, byte color);
    void drawcircle (byte x0, byte y0, byte r, byte color);
    void drawrect   (byte x, byte y, byte w, byte h, byte color);
    void fillrect   (byte x, byte y, byte w, byte h, byte color);
    void drawline   (byte x0, byte y0, byte x1, byte y1, byte color);
    void drawchar   (byte x, byte line, char c );
    void drawstring (byte x, byte line, char *c);
    void drawstring_p (byte x, byte line, char *c);
    void drawtriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void filltriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void updatedisplayarea(byte x0,byte y0,byte x1,byte y1,byte reset =0);
    void setupdatearea(byte x0,byte y0,byte x1,byte y1, byte allowreduction =0);

    void drawbitmap (byte x, byte y, const byte *bitmap, byte w, byte h, byte color);
    void drawcharx(byte x, byte y, char c);
    void drawstringx(byte x, byte line, char *c);
    void drawstringx_p(byte x, byte line, char *c);
    void scrollup(byte y);
    void scrolldown(byte y);
    void scrollleft(byte x);
    void scrollright(byte x);
};