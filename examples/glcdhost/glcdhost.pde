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

byte doublebuf[66];

void setup () {
  glcd.begin();
  glcd.backlight(255);
  glcd.drawstring(30,1,"JeePU Host");
  glcd.drawstring(16,3,"Group:");
  char t[4];
  itoa(Node_group,t,10);
  glcd.drawstring(52,3,t);
  
  glcd.drawstring(84,3,"ID:");
  itoa(Node_ID,t,10);
  glcd.drawstring(102,3,t);
  glcd.drawline(30,16,89,16,1);
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
        doublebuf[i]=rf12_data[i];
      rf12_recvDone();
      processmessage(doublebuf);
    }
  }
}

void processmessage(byte *data) {
  switch (data[0])
  {
    case REMOTELCDCLEAR: glcd.clear();
      break;
    case REMOTELCDCLEARWHITE: glcd.clear_white();
      break;
    case REMOTELCDSETPIXEL: glcd.setpixel(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWLINE: glcd.drawline(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDDRAWRECT: glcd.drawrect(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDFILLRECT: glcd.fillrect(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDDRAWCIRCLE: glcd.drawcircle(data[1],data[2],data[3],data[4]);
      break;
    case REMOTELCDFILLCIRCLE: glcd.fillcircle(data[1],data[2],data[3],data[4]);
      break;
    case REMOTELCDDRAWTRIANGLE: glcd.drawtriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      break;
    case REMOTELCDFILLTRIANGLE: glcd.filltriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      break;
    case REMOTELCDDRAWCHAR: glcd.drawchar(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWSTRING:
      if (data[3]<65) {
        char text[66];
        for (int i=0;i<data[3];i++) {
          text[i]=data[i+4];
        }
        text[data[3]]=0;
        glcd.drawstring(data[1],data[2],text);
      }
      break;
    case REMOTELCDDRAWCHARX: glcd.drawcharx(data[1],data[2],data[3]);
      break;
    case REMOTELCDDRAWSTRINGX:
      if (data[3]<65) {
        char text[66];
        for (int i=0;i<data[3];i++) {
          text[i]=data[i+4];
        }
        text[data[3]]=0;
        glcd.drawstringx(data[1],data[2],text);
      }
      break;
    case REMOTELCDUPDATEAREA: glcd.updatedisplayarea(data[1],data[2],data[3],data[4],data[5]);
      break;
    case REMOTELCDSETUPDATEAREA: glcd.setupdatearea(data[1],data[2],data[3],data[4],data[5]);
      break;
//        case REMOTELCDDRAWBMP: ;
    case REMOTELCDDISPLAY: glcd.display();
      break;
    case REMOTELCDCLEARBUF: glcd.clear_display();
      break;
    case REMOTELCDSCROLLUP: glcd.scrollup(data[1]);
      break;
    case REMOTELCDSCROLLDOWN: glcd.scrolldown(data[1]);
      break;
    case REMOTELCDSCROLLLEFT: glcd.scrollleft(data[1]);
      break;
    case REMOTELCDSCROLLRIGHT: glcd.scrollright(data[1]);
      break;
    case REMOTELCDBACKLIGHT: glcd.backlight(data[1]);
      break;
  }
}
