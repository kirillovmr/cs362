
#include "ESP8266wifi.h"

WifiModule *wifi;

long int time = millis();
bool t = false;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

  wifi = new WifiModule(6, 5, 115200, new NoNetworkState());

  delay(2000);
  Serial.println("upd");


  
  if (wifi->atTest() == false) {
    Serial.println("at test failed");
    return;
  }
  else {
    Serial.println("at ok");
  }

  if (wifi->restart() == false) {
    Serial.println("rst failed");
  }
  else {
    Serial.println("rst ok");
  }
  // check for ready after restart
  // 13:58:15.693 -> ready

  
  // check for connected wifi
  // 13:58:17.870 -> WIFI CONNECTED
  // 13:58:18.508 -> WIFI GOT IP

  // make sure to enable wifi mode

  time = millis();

  return;
  
  if (wifi->connectToWifi("Yo Yo Yo", "vovavakhniuk")) {
//    if (wifi->connectToServer("10.0.0.204", "3010")) {
//      Serial.println("Connected to server.");
//    }
  }
}

void loop() {
//  if (time + 8000 < millis() && t == false) {
//    Serial.println("conn to wifi");
//    t = true;
//
//    if (wifi->connectToWifi("Yo Yo Yo", "vovavakhniuk")) {
//      if (wifi->connectToServer("10.0.0.204", "3010")) {
//        Serial.println("Connected to server.");
//      }
//    }
//  }
  
//  wifi->feedbackLoop();
}
