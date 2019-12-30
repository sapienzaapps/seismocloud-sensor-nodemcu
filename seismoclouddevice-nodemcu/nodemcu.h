
#ifndef __NODEMCU_H
#define __NODEMCU_H

#include "common.h"

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define CFGSSID "SeismoCloud"

class NodeMCU {
  public:
    static void begin();
};

#endif
