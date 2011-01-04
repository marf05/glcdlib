/*
$Id$

GLCD_proxy Remote LCD library!

*/

#include <WProgram.h>

#define BLACK           0
#define WHITE           1

#define LCDWIDTH        128
#define LCDHEIGHT       64

#define SCROLLUP        1
#define SCROLLDOWN      2
#define SCROLLLEFT      3
#define SCROLLRIGHT     4


class GLCD_proxy {
    byte remoteId;
    void sendLCDMessage(byte length);  
    
public:
    GLCD_proxy(byte nodeId) : remoteId (nodeId) {}

    void begin();
    void backLight(byte level);
    void clearDisplay();
    void clearWhite();
    void clear();
    void display();

    void setPixel   (byte x, byte y, byte color);
    void fillCircle (byte x0, byte y0, byte r, byte color);
    void drawCircle (byte x0, byte y0, byte r, byte color);
    void drawRect   (byte x, byte y, byte w, byte h, byte color);
    void fillRect   (byte x, byte y, byte w, byte h, byte color);
    void drawLine   (byte x0, byte y0, byte x1, byte y1, byte color);
    void drawChar   (byte x, byte y, char c );
    void drawString (byte x, byte y, char *c);
    void drawString_p (byte x, byte y, char *c);
    void drawTriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void fillTriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void updateDisplayArea(byte x0,byte y0,byte x1,byte y1,byte reset =0);
    void setUpdateArea(byte x0,byte y0,byte x1,byte y1, byte allowReduction =0);

    void drawBitmap (byte x, byte y, const byte *bitmap, byte w, byte h, byte color);
    void scroll(byte direction, byte pixels);
};