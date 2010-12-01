#define Node_ID 2
#define Node_group 212

#include <glcdremotemsgs.h>
#include <ST7565.h>
#include <Ports.h>
#include <RF12.h>

ST7565 glcd(14, 4, 17, 7);

byte serialmsg[100];
byte serialpos=0;

void setup () {
  Serial.begin(57600);
  Serial.println("\n[gLCD Host]");
  glcd.st7565_init();
  glcd.st7565_command(CMD_DISPLAY_ON);
  glcd.st7565_command(CMD_SET_ALLPTS_NORMAL);
  glcd.st7565_set_brightness(0x015);
  glcd.clear();
  glcd.display(); 
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
      for (byte i=0;i<rf12_len;i++) {
        Serial.print(rf12_data[i],DEC);
        Serial.print(" ");
      }
      Serial.println("");
      processmessage(rf12_data);
      Serial.println("------------");
    }
  }
  if (Serial.available()>0) {
    byte incoming=Serial.read();
    if (incoming==13) {
      serialmsg[serialpos]=0;
      if (serialpos>0) {
        processmessage(serialmsg);
        serialpos=0;
      }
    }
    else {
      serialmsg[serialpos++]=incoming;
    }
  }
}

void processmessage(volatile uint8_t *data) {
  switch (data[0])
  {
    case REMOTELCDINIT: glcd.st7565_init();
      Serial.println("init");
      break;
    case REMOTELCDCOMMAND: glcd.st7565_command(data[1]);
      Serial.println("Command");
      break;
    case REMOTELCDDATA: glcd.st7565_data(data[1]);
      Serial.println("data");
      break;
    case REMOTELCDSETBRIGHTNESS: glcd.st7565_set_brightness(data[1]);
      Serial.println("brightness");
      break;
    case REMOTELCDCLEAR: glcd.clear();
      Serial.println("clear");
      break;
    case REMOTELCDCLEARWHITE: glcd.clear_white();
      Serial.println("clear white");
      break;
    case REMOTELCDSETPIXEL: glcd.setpixel(data[1],data[2],data[3]);
      Serial.println("setpixel");
      break;
    case REMOTELCDDRAWLINE: glcd.drawline(data[1],data[2],data[3],data[4],data[5]);
      Serial.println("drawline");
      break;
    case REMOTELCDDRAWRECT: glcd.drawrect(data[1],data[2],data[3],data[4],data[5]);
      Serial.println("drawrect");
      break;
    case REMOTELCDFILLRECT: glcd.fillrect(data[1],data[2],data[3],data[4],data[5]);
      Serial.println("fillrect");
      break;
    case REMOTELCDDRAWCIRCLE: glcd.drawcircle(data[1],data[2],data[3],data[4]);
      Serial.println("drawcircle");
      break;
    case REMOTELCDFILLCIRCLE: glcd.fillcircle(data[1],data[2],data[3],data[4]);
      Serial.println("fillcircle");
      break;
    case REMOTELCDDRAWTRIANGLE: glcd.drawtriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      Serial.println("drawtriangle");
      break;
    case REMOTELCDFILLTRIANGLE: glcd.filltriangle(data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
      Serial.println("filltriangle");
      break;
    case REMOTELCDDRAWCHAR: glcd.drawchar(data[1],data[2],data[3]);
      Serial.println("drawchar");
      break;
    case REMOTELCDDRAWSTRING:
      if (data[3]<65) {
        char text[66];
        for (int i=0;i<data[3];i++) {
          text[i]=data[i+4];
        }
        text[data[3]]=0;
        glcd.drawstring(data[1],data[2],text);
        Serial.println("drawstring");
      }
      break;
    case REMOTELCDUPDATEAREA: glcd.updatedisplayarea(data[1],data[2],data[3],data[4],data[5]);
      Serial.println("updatedisplayarea");
      break;
    case REMOTELCDSETUPDATEAREA: glcd.setupdatearea(data[1],data[2],data[3],data[4],data[5]);
      Serial.println("setupdatearea");
      break;
//        case REMOTELCDDRAWBMP: ;
    case REMOTELCDDISPLAY: glcd.display();
      Serial.println("display");
      break;
    case REMOTELCDCLEARBUF: glcd.clear_display();
      Serial.println("clear_display");
      break;
  }
}
