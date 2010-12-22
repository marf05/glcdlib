/*
$Id$

gLCDhost Display driver host application.

*/

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
  if (rf12_recvDone()) {
    if ((rf12_len>=1) && (rf12_crc==0)) {
      for (byte i=0;i<rf12_len;i++)
        doubleBuf[i]=rf12_data[i];
      rf12_recvDone();
      processmessage(doubleBuf);
    }
  }
}

void processmessage(byte *data) {
  switch (data[0])
  {
    case REMOTELCDCLEAR: glcd.clear();
      break;
    case REMOTELCDCLEARWHITE: glcd.clearWhite();
      break;
    case REMOTELCDSETPIXEL: glcd.setPixel(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWLINE: glcd.drawLine(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDDRAWRECT: glcd.drawRect(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDFILLRECT: glcd.fillRect(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDDRAWCIRCLE: glcd.drawCircle(data[1],data[2],data[3],data[4]);
      break;
    case REMOTELCDFILLCIRCLE: glcd.fillCircle(data[1],data[2],data[3],data[4]);
      break;
    case REMOTELCDDRAWTRIANGLE: glcd.drawTriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      break;
    case REMOTELCDFILLTRIANGLE: glcd.fillTriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      break;
    case REMOTELCDDRAWCHAR: glcd.drawChar(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWSTRING:
      if (data[3]<65) {
        char text[66];
        for (int i=0;i<data[3];i++) {
          text[i]=data[i+4];
        }
        text[data[3]]=0;
        glcd.drawString(data[1],data[2],text);
      }
      break;
    case REMOTELCDDRAWCHARX: glcd.drawCharX(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWSTRINGX:
      if (data[3]<65) {
        char text[66];
        for (int i=0;i<data[3];i++) {
          text[i]=data[i+4];
        }
        text[data[3]]=0;
        glcd.drawStringX(data[1],data[2],text);
      }
      break;
    case REMOTELCDUPDATEAREA: glcd.updateDisplayArea(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDSETUPDATEAREA: glcd.setUpdateArea(data[1],data[2],data[3],data[4],data[5]);
      break;
//        case REMOTELCDDRAWBMP: ;
    case REMOTELCDDISPLAY: glcd.display();
      break;
    case REMOTELCDCLEARBUF: glcd.clearDisplay();
      break;
    case REMOTELCDSCROLLUP: glcd.scrollUp(data[1]);
      break;
    case REMOTELCDSCROLLDOWN: glcd.scrollDown(data[1]);
      break;
    case REMOTELCDSCROLLLEFT: glcd.scrollLeft(data[1]);
      break;
    case REMOTELCDSCROLLRIGHT: glcd.scrollRight(data[1]);
      break;
    case REMOTELCDBACKLIGHT: glcd.backLight(data[1]);
      break;
  }
}
