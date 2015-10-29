
int readLine(char* buf, int bufmax);
void httpRequest(char* host, unsigned short port, char* path, String postVars);
void httpAliveRequest();
void httpQuakeRequest();

void httpQuakeRequest() {
  String postVars = String("deviceid=");
  postVars += macToString(mac);
  postVars += "&tsstart=";
  postVars += getUNIXTime();
  postVars += "&lat=" + String(latitude) + "&lon=" + String(longitude);
  httpRequest("www.sapienzaapps.it", 80, "/seismocloud/terremoto.php", postVars);
}

void httpAliveRequest() {
  String postVars = String("deviceid=");
  postVars += macToString(mac);
  // TODO: parametrized version and model
  postVars += "&model=uno&version=1.00&lat=" + String(latitude) + "&lon=" + String(longitude);
  httpRequest("www.sapienzaapps.it", 80, "/seismocloud/alive.php", postVars);
}

void httpRequest(char* host, unsigned short port, char* path, String postVars) {
  // if there's a successful connection:
  if (client.connect(host, port)) {
    Serial.println(F("connecting..."));
    // send the HTTP PUT request:
    String req;
    if(postVars == NULL) {
      req = "GET " + String(path) + " HTTP/1.1";
    } else {
      req = "POST " + String(path) + " HTTP/1.1";
    }
    
    client.println(req);

    String hostHeader = String("Host: ");
    hostHeader += host;
    client.println(hostHeader);
    client.println(F("User-Agent: arduino-ethernet"));
    client.println(F("Connection: close"));
    client.println();

    unsigned long connms = millis();

    while(!client.available() && millis() - connms < 10*1000);
    if(client.available()) {
      // Read reply
      bool headerPass = false;
      char buf[256+1];
      while(client.available()) {
        memset(buf, 0, 256+1);
        int r = readLine(buf, 256);
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
    Serial.println(F("connection failed"));
  }
  client.stop();
}

int readLine(char* buf, int bufmax) {

  bool readError = false;
  int i = 0;
  
  for(; client.available() && i < bufmax && !readError; i++) {
    buf[i] = client.read();
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

