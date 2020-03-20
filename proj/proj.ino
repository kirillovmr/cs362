
#include "ESP8266wifi.h"
#include "Timer.hpp"

#define joyX A5
#define joyY A4

WifiModule *wifi;
Timer *timer;

long int time = millis();
bool t = false;

int xMap, yMap, xValue, yValue;

void setup() {
  Serial.begin(115200);
  Serial.println("start");

//  return;

  wifi = new WifiModule(6, 5, 115200, new NoNetworkState());

  delay(500);
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

  delay(20);

  time = millis();

//  return;
  
  if (wifi->connectToWifi("Yo Yo Yo", "vovavakhniuk")) {
    delay(2000);
    if (wifi->connectToServer("10.0.0.204", "3010")) {
      
    }
  }

    timer = new Timer;
    timer->add([wifi](){
      String data = "";
      if (wifi->readFromServer(data)) {
        String output = "Received: ";
        Serial.println(output + data);
      }
    }, 100);

    // 22:27:44.823 -> +IPD,8:hi there
    // 22:27:47.826 -> +IPD,6:Update 
}

void loop() { 
  timer->update();
//  wifi->feedbackLoop();


  xValue = analogRead(joyX);
  yValue = analogRead(joyY);
  xMap = map(xValue, 0,1023, 2, 0);
  yMap = map(yValue, 0,1023, 3, 0);
  String x = "X=";
  String y = "Y=";
//  Serial.println(x + xMap);
//  Serial.println(y + yMap);
//  Serial.println();

  if (xMap > 1)
    wifi->sendRight();
  else if (xMap < 1)
    wifi->sendLeft();
  if (yMap > 2)
    wifi->sendUp();
  else if (yMap < 1)
    wifi->sendDown();
  
  delay(100);
}
