
#include "common.h"
#include <utility/w5100.h>
#include <utility/socket.h>

void httpQuakeRequest() {
  String postVars = String("deviceid=");
  
  byte macaddress[6];
  getMACAddress(macaddress);
  postVars += macToString(macaddress);
  
  postVars += "&tsstart=";
  postVars += getUNIXTime();
  postVars += "&lat=" + getLatitudeAsString() + "&lon=" + getLongitudeAsString();
  httpRequest(DEFAULTHOST, 80, "/seismocloud/terremoto.php", postVars);
}

void httpAliveRequest() {
  String postVars = String("deviceid=");
  
  byte macaddress[6];
  getMACAddress(macaddress);
  postVars += macToString(macaddress);
  
  // TODO: parametrized version and model
  postVars += "&model=uno&version=" + getVersionAsString() + "&lat=" + getLatitudeAsString() + "&lon=" + getLongitudeAsString();
  httpRequest(DEFAULTHOST, 80, "/seismocloud/alive.php", postVars);
}

void httpRequest(char* host, unsigned short port, char* path, String postVars) {
  EthernetClient client;
  // if there's a successful connection:
  int cresult = client.connect(host, port);
  if (cresult) {
    if(postVars == NULL) {
      client.print(F("GET "));
    } else {
      client.print(F("POST "));
    }
    client.print(path);
    client.println(F(" HTTP/1.1"));

    client.print(F("Host: "));
    client.println(host);
    client.println(F("User-Agent: arduino-ethernet"));
    if(postVars != NULL) {
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(postVars.length());
    }
    client.println(F("Connection: close"));
    client.println();

    if(postVars != NULL) {
      client.print(postVars);
    }

    unsigned long connms = millis();

    while(!client.available() && millis() - connms < 10*1000);
    if(client.available()) {
      // Read reply
      bool headerPass = false;
      char buf[256+1];
      while(client.available()) {
        memset(buf, 0, 256+1);
        int r = readLine(&client, buf, 256);
        if(r < -1) break;
        if(headerPass) {
          // TODO: Read body
        } else if(r != 0) {
          // TODO: Read header
        } else {
          // Header separator
          headerPass = true;
        }
      }
    } else {
      Serial.println(F("Socket read error"));
    }
  } else {
    // if you couldn't make a connection:
    Serial.print(F("connection failed to: "));
    Serial.print(host);
    Serial.print(":");
    Serial.print(port);
    Serial.print(" ");
    Serial.println(cresult);

    //ShowSockStatus();
  }
  if(client.connected()) {
    client.stop();
  }
}

int readLine(EthernetClient *client, char* buf, int bufmax) {

  bool readError = false;
  int i = 0;
  
  for(; client->available() && i < bufmax && !readError; i++) {
    buf[i] = client->read();
    if(buf[i] == '\r') {
      i--;
      continue;
    }
    if(buf[i] < 0 || buf[i] == '\n') {
      readError = buf[i] < 0;
      buf[i] = 0;
      break;
    }
  }

  return (readError ? -1 : i);
}


void ShowSockStatus()
{
 Serial.println();
 for (int i = 0; i < MAX_SOCK_NUM; i++) {
   Serial.print(F("Socket#"));
   Serial.print(i);
   uint8_t s = W5100.readSnSR(i);
   Serial.print(F(":0x"));
   Serial.print(s,16);
   Serial.print(F(" "));
   Serial.print(W5100.readSnPORT(i));
   Serial.print(F(" D:"));
   uint8_t dip[4];
   W5100.readSnDIPR(i, dip);
   for (int j=0; j<4; j++) {
     Serial.print(dip[j],10);
     if (j<3) Serial.print(".");
   }
   Serial.print(F("("));
   Serial.print(W5100.readSnDPORT(i));
   Serial.println(F(")"));
 }
  Serial.println();
}

