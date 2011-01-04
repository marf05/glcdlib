// JeePU Display driver host application.
// $Id$

#define Node_ID 2
#define Node_group 212

#include <GLCD_proxymsgs.h>
#include <GLCD_ST7565.h>
#include <Ports.h>
#include <RF12.h>

GLCD_ST7565 glcd;

byte doubleBuf[66];

void setup () {
    glcd.begin();
    glcd.backLight(255);
    glcd.drawString(30,1,"JeePU Host");
    glcd.drawString(16,3,"Group:");
    char t[4];
    itoa(Node_group,t,10);
    glcd.drawString(52,3,t);
  
    glcd.drawString(84,3,"ID:");
    itoa(Node_ID,t,10);
    glcd.drawString(102,3,t);
    glcd.drawLine(30,16,89,16,1);
    glcd.display(); 
    rf12_initialize(Node_ID, RF12_868MHZ, Node_group);
    delay(1000);
    rf12_recvDone();  
    delay(1000);
}

void loop () {
    if (rf12_recvDone() && rf12_crc == 0 && rf12_len > 0) {
        memcpy(doubleBuf, (void*) rf12_data, rf12_len);
        rf12_recvDone();
        processmessage(doubleBuf);
    }
}

void processmessage(byte *d) {
    switch (d[0]) {
        case REMOTELCDCLEAR: 
            glcd.clear(); break;
        case REMOTELCDCLEARWHITE:
            glcd.clearWhite(); break;
        case REMOTELCDSETPIXEL: 
            glcd.setPixel(d[1],d[2],d[3]); break;
        case REMOTELCDDRAWLINE: 
            glcd.drawLine(d[1],d[2],d[3],d[4],d[5]); break;
        case REMOTELCDDRAWRECT: 
            glcd.drawRect(d[1],d[2],d[3],d[4],d[5]); break;
        case REMOTELCDFILLRECT: 
            glcd.fillRect(d[1],d[2],d[3],d[4],d[5]); break;
        case REMOTELCDDRAWCIRCLE: 
            glcd.drawCircle(d[1],d[2],d[3],d[4]); break;
        case REMOTELCDFILLCIRCLE: 
            glcd.fillCircle(d[1],d[2],d[3],d[4]); break;
        case REMOTELCDDRAWTRIANGLE:
            glcd.drawTriangle(d[1],d[2],d[3],d[4],d[5],d[6],d[7]); break;
        case REMOTELCDFILLTRIANGLE:
            glcd.fillTriangle(d[1],d[2],d[3],d[4],d[5],d[6],d[7]); break;
        case REMOTELCDDRAWCHAR: 
            glcd.drawChar(d[1],d[2],d[3]); break;
        case REMOTELCDDRAWSTRING:
            if (d[3]<65) {
                char text[66];
                for (int i = 0; i < d[3]; ++i)
                    text[i] = d[i+4];
                text[d[3]] = 0;
                glcd.drawString(d[1],d[2],text);
            }
            break;
        case REMOTELCDUPDATEAREA:
            glcd.updateDisplayArea(d[1],d[2],d[3],d[4],d[5]); break;
        case REMOTELCDSETUPDATEAREA:
            glcd.setUpdateArea(d[1],d[2],d[3],d[4],d[5]); break;
        // case REMOTELCDDRAWBMP:
        case REMOTELCDDISPLAY: 
            glcd.display(); break;
        case REMOTELCDCLEARBUF: 
            glcd.clearDisplay(); break;
        case REMOTELCDSCROLL: 
            glcd.scroll(d[1],d[2]); break;
        case REMOTELCDBACKLIGHT: 
            glcd.backLight(d[1]); break;
    }
}
