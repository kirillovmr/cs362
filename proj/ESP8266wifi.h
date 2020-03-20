/*
    
 */

#ifndef ESP8266wifi_h
#define ESP8266wifi_h

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <string.h>


/* ===================
 Abstract WiFi state
==================== */
class WifiModule;
class WifiState {
protected:
    WifiModule *_context;
    
public:
    void setContext(WifiModule *context);
    virtual bool connectToWifi(String ssid, String pass) = 0;
    virtual bool connectToServer(String ip, String port) = 0;
};


/* =================
 WiFi Module class
================== */
class WifiModule {
private:
    WifiState *_state;
    SoftwareSerial *_espSerialLib;
    
public:
    WifiModule(byte rx, byte tx, uint32_t baud, WifiState *state);
    ~WifiModule();
    
    void transitionTo(WifiState *state);
    void sendSerialCmd(String cmd);
    void feedbackLoop();
    
    bool atTest();
    bool connectToWifi(String ssid, String pass);
    bool connectToServer(String ip, String port);
    
    bool isFound(String word, int tries, int timeout);
    
    bool sendLeft();
    bool sendRight();
    bool sendUp();
    bool sendDown();
    
    bool readFromServer(String &data);
    
//    bool sendToServer(String data, int len);
    
    bool restart();
};


/* ===========================
 Connecting to Network State
============================ */
class NoNetworkState: public WifiState {
private:
    bool _wifiModeOn;
public:
    NoNetworkState();
    bool connectToWifi(String ssid, String pass) override;
    bool connectToServer(String ip, String port) override {}
    bool setWifiMode();
};

/* ==========================
 Connecting to Server State
=========================== */
class ConnectedToNetworkState: public WifiState {
private:
public:
    ConnectedToNetworkState();
    bool connectToWifi(String ssid, String pass) override {}
    bool connectToServer(String ip, String port) override;
};

/* ===============
 Connected State
================ */
class ConnectedToServerState: public WifiState {
private:
public:
    ConnectedToServerState();
    bool connectToWifi(String ssid, String pass) override {}
    bool connectToServer(String ip, String port) override {}
};

#endif
