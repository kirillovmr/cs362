/*
    
 */

#include "ESP8266wifi.h"
#include <Arduino.h>
#include <string.h>


/* =================
 WiFi Module class
================== */
WifiModule::WifiModule(byte rx, byte tx, uint32_t baud, WifiState *state): _state(nullptr){
    this->_espSerialLib = new SoftwareSerial(rx, tx);
    this->_espSerialLib->begin(baud);
    
    this->transitionTo(state);
}
WifiModule::~WifiModule() {
    delete _espSerialLib;
    delete _state;
}

void WifiModule::transitionTo(WifiState *state){
    if (this->_state != nullptr)
        delete this->_state;
    
    this->_state = state;
    this->_state->setContext(this);
}

void WifiModule::sendSerialCmd(String cmd){
    String newCmd = "AT+";
    newCmd += cmd;
    
    Serial.println("Sending command " + newCmd);
    this->_espSerialLib->println(newCmd);
}

void WifiModule::feedbackLoop() {
    //impliment serial view debugging
    while(this->_espSerialLib->available() > 0){
        char a = _espSerialLib->read();
        if(a == '\0')
            continue;
        if(a != '\r' && a != '\n' && (a < 32))
            continue;
        Serial.print(a);
    }

    //impliment serial view debugging
    while(Serial.available() > 0){
        char a = Serial.read();
        Serial.write(a);
        _espSerialLib->write(a);
    }
}

bool WifiModule::atTest() {
    this->_espSerialLib->println("AT");
    return this->isFound("OK", 5, 10);
}

bool WifiModule::connectToWifi(String ssid, String pass) {
    return this->_state->connectToWifi(ssid, pass);
}
bool WifiModule::connectToServer(String ip, String port) {
    return this->_state->connectToServer(ip, port);
}

bool WifiModule::isFound(String word, int tries, int timeout){
    for (byte i = 0; i < tries; i++) {
        delay(timeout);
        if (this->_espSerialLib->find( (char*)(word.c_str())) ){
            return true;
        }
    }
    return false;
}

//bool WifiModule::sendToServer(String data, int len) {
//    String cmd = "CIPSEND=1";
//    this->sendSerialCmd(cmd);
//    // TODO: check for ok
//
//    delay(10);
//
//    this->_espSerialLib->println("8");
//    // TODO: check for ok?
//
//    return true;
//}

bool WifiModule::sendLeft(){
    String cmd = "CIPSEND=1";
    this->sendSerialCmd(cmd);
    delay(10);
    this->_espSerialLib->println("L");
}
bool WifiModule::sendRight(){
    String cmd = "CIPSEND=1";
    this->sendSerialCmd(cmd);
    delay(10);
    this->_espSerialLib->println("R");
}
bool WifiModule::sendUp(){
    String cmd = "CIPSEND=1";
    this->sendSerialCmd(cmd);
    delay(10);
    this->_espSerialLib->println("U");
}
bool WifiModule::sendDown(){
    String cmd = "CIPSEND=1";
    this->sendSerialCmd(cmd);
    delay(10);
    this->_espSerialLib->println("D");
}
bool WifiModule::readFromServer(String &data){
    // 22:27:44.823 -> +IPD,8:hi there
    // 22:27:47.826 -> +IPD,6:Update
    
    if (this->_espSerialLib->find("+IPD,")) {
        delay(20);
        
        int bytes = this->_espSerialLib->read() - 48;
        Serial.print("Received "); Serial.print(bytes); Serial.println(" bytes");
        this->_espSerialLib->read(); // colon
        
        data = "";
        for(byte i=0; i<bytes; i++)
            data += (char)(this->_espSerialLib->read());
        
        return true;
    }
    
    return false;
}

bool WifiModule::restart(){
    this->_espSerialLib->println("AT+RST");
    return this->isFound("ready", 5, 20);
}

/* ===================
 Abstract WiFi state
==================== */
void WifiState::setContext(WifiModule *context){
    this->_context = context;
}

/* ===========================
 Connecting to Network State
============================ */
NoNetworkState::NoNetworkState(){
    this->_wifiModeOn = false;
    Serial.println("WiFi has entered NoNetworkState State");
}

bool NoNetworkState::connectToWifi(String ssid, String pass){
    Serial.println("connectToWifi: checking for being connected to wifi..");
    if (this->_context->isFound("WIFI CONNECTED", 5, 25)){
        Serial.println("connectToWifi: wifi is already connected");
        
        Serial.println("connectToWifi: switching to wifi mode");
        if (this->_wifiModeOn == false) { this->setWifiMode(); }
        
        this->_context->transitionTo(new ConnectedToNetworkState());
        return true;
    }
    
    Serial.println("connectToWifi: switching to wifi mode");
    if (this->_wifiModeOn == false) { this->setWifiMode(); }
    
    Serial.println("connectToWifi: connecting to wifi");
    String cmd = "CWJAP=\"";
    cmd += ssid;
    cmd += "\",\"";
    cmd += pass;
    cmd += "\"";
    this->_context->sendSerialCmd(cmd);
    
    if (this->_context->isFound("OK", 5, 20) == false) {
        Serial.println("connectToWifi: was not able to connect to wifi");
        return false;
    }
    
    this->_context->transitionTo(new ConnectedToNetworkState());
    return true;
}

bool NoNetworkState::setWifiMode() {
    String cmd = "CWMODE=3";
    this->_context->sendSerialCmd(cmd);
    
//    if (this->_context->isFound("OK", 3, 10) == false){
//        // It may return ERROR if we already in wifi mode
//        Serial.println("setWifiMode: was not able to set wifi mode but its ok");
//        return false;
//    }
    
    Serial.println("setWifiMode: successfully switched to wifi mode");
    this->_wifiModeOn = true;
    return true;
}

/* ==========================
 Connecting to Server State
=========================== */
ConnectedToNetworkState::ConnectedToNetworkState(){
    Serial.println("WiFi has entered ConnectedToNetworkState State");
}

bool ConnectedToNetworkState::connectToServer(String ip, String port){
    // AT+CIPSTART="TCP","10.0.0.204",3010
    Serial.println("connectToServer: connecting to " + ip + ":" + port);
    String cmd = "CIPSTART=\"TCP\",\"";
    cmd += ip;
    cmd += "\",";
    cmd += port;
    cmd += "";
    this->_context->sendSerialCmd(cmd);
    
    if (this->_context->isFound("OK", 5, 20) == false) {
        Serial.println("connectToServer: was not able to connect to " + ip + ":" + port);
        return false;
    }
    
    Serial.println("connectToServer: successfully conencted to " + ip + ":" + port);
    this->_context->transitionTo(new ConnectedToServerState());
    return true;
}

/* ==============================
 Connected to Game Server State
=============================== */
ConnectedToServerState::ConnectedToServerState(){
    Serial.println("WiFi has entered ConnectedToServerState State");
}
