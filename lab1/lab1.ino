//
// Lab 1
// Get started with Arduino
// 
// Viktor Kirillov
//

#include <stdarg.h> // for arbitrary number of args

const byte MAX_PINS = 10;

class Light {
private:
    byte *pins, totalPins, currentPinIdx;   // To store led pins

    unsigned long m_interval = 1000;  // Interval at which to blink
    unsigned long previousMillis;   // Last time LED was updated
    
public:
    Light(int pin...): totalPins(0), currentPinIdx(0), previousMillis(0), m_interval(1000) {
        pins = new byte[10];

        // First parameter
        if (isPinValid(pin))
            pins[totalPins++] = (byte) pin;

        // Rest parameters
        va_list list;
        va_start(list, pin);
        while (int _pin = va_arg(list, int)) {
            if (isPinValid(_pin) && totalPins < MAX_PINS) {
                pins[totalPins++] = (byte) _pin;
            }
        }
        va_end(list);

        _setPinsMode();
    }

    void setInterval(unsigned long value) {
        m_interval = value;
    }

    void update() {
        unsigned long currentMillis = millis(); // Time passed after start

        if (currentMillis - previousMillis < m_interval)
            return;
        
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        
        // Set current pin to LOW
        digitalWrite(pins[currentPinIdx], LOW);
        currentPinIdx++;
        currentPinIdx = currentPinIdx % totalPins;
        
        // Set next pin to HIGH
        digitalWrite(pins[currentPinIdx], HIGH);
    }

private:
    bool isPinValid(int pin) {  // Checks for pin to be in valid range
        return (pin >= 0 && pin < 30);
    }
    
    void _setPinsMode() {   // Sets mode to OUTPUT
        for(byte i=0; i<totalPins; i++) {
            pinMode(pins[i], OUTPUT);
        }
    }
};

Light *light;

void setup() {
    light = new Light(13,8,7);
    light->setInterval(100);
}

void loop() {
    light->update();
}
