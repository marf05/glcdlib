/*
$Id$

GLCD_proxy Remote LCD library!

*/

#include <WProgram.h>

#define BLACK					0
#define WHITE					1

#define LCDWIDTH				128
#define LCDHEIGHT				64
#define LCDUNUSEDSTARTBYTES 4

#define CMD_DISPLAY_OFF			0xAE		//  1
#define CMD_DISPLAY_ON			0xAF		//  1

#define CMD_SET_DISP_START_LINE	0x40		//  2
#define CMD_SET_PAGE			0xB0		//  3

#define CMD_SET_COLUMN_UPPER	0x10		//  4
#define CMD_SET_COLUMN_LOWER	0x00		//  4

#define CMD_SET_ADC_NORMAL		0xA0		//  8
#define CMD_SET_ADC_REVERSE		0xA1		//  8

#define CMD_SET_DISP_NORMAL		0xA6		//  9
#define CMD_SET_DISP_REVERSE	0xA7		//  9

#define CMD_SET_ALLPTS_NORMAL	0xA4		// 10
#define CMD_SET_ALLPTS_ON		0xA5		// 10
#define CMD_SET_BIAS_9			0xA2		// 11
#define CMD_SET_BIAS_7			0xA3		// 11

#define CMD_RMW					0xE0		// 12
#define CMD_RMW_CLEAR			0xEE		// 13
#define CMD_INTERNAL_RESET		0xE2		// 14
#define CMD_SET_COM_NORMAL		0xC0		// 15
#define CMD_SET_COM_REVERSE		0xC8		// 15
#define CMD_SET_POWER_CONTROL	0x2F		// 16
#define CMD_SET_RESISTOR_RATIO  0x24		// 17
#define CMD_SET_VOLUME_FIRST	0x81		// 18
#define CMD_SET_VOLUME_SECOND	0x00		// 18
#define CMD_SET_STATIC_OFF		0xAC		// 19
#define CMD_SET_STATIC_ON		0xAD		// 19
#define CMD_SET_STATIC_REG		0x00		// 19
//#define CMD_SET_BOOSTER_FIRST	0xF8		// 
//#define CMD_SET_BOOSTER_234		0x00		// 20
//#define CMD_SET_BOOSTER_5		0x01		// 20
//#define CMD_SET_BOOSTER_6		0x03		// 20
#define CMD_NOP					0xE3		// 22
#define CMD_TEST				0xF0		// 23

class GLCD_proxy {
public:
    GLCD_proxy(byte nodeId) : remoteId (nodeId) {}

    void begin();
    void st7565_command(byte c);
    void st7565_data(byte c);
    void st7565_set_brightness(byte val);
    void clear_display(void);
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
    void drawtriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void filltriangle (byte x0, byte y0, byte x1, byte y1, byte x2, byte y2, byte color);
    void updatedisplayarea(byte x0,byte y0,byte x1,byte y1,byte reset =0);
    void setupdatearea(byte x0,byte y0,byte x1,byte y1, byte allowreduction =0);

    void drawbitmap (byte x, byte y, const byte *bitmap, byte w, byte h, byte color);
    void drawcharx(byte x, byte y, char c);
    void drawstringx(byte x, byte line, char *c);
    void scrollup(byte y);
    void scrolldown(byte y);
    void scrollleft(byte x);
    void scrollright(byte x);

private:
	void sendlcdmessage(byte length);  

    byte remoteId;
};