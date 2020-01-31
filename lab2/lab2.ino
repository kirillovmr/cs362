//
// Lab 2
// Input and Output
// 
// Viktor Kirillov
//

#include <stdarg.h> // for arbitrary number of args

const byte MAX_LEDS = 10;

class BinaryLed {
private:
    byte *leds;
    byte totalLeds;
    byte btn1, btn2;
    byte counter;
    int maxValue;

    bool pressed;

public:
    BinaryLed(byte b1, byte b2, byte led...): 
        totalLeds(0), btn1(b1), btn2(b2), counter(0), maxValue(1), pressed(false)
    {
        Serial.begin(9600);
        
        leds = new byte[MAX_LEDS];

        // Filling leds array
        if (isPinValid(led))
            leds[totalLeds++] = (byte) led;
        
        va_list list;
        va_start(list, led);
        while (int _pin = va_arg(list, int)) {
            if (isPinValid(_pin) && totalLeds < MAX_LEDS) {
                leds[totalLeds++] = (byte) _pin;
            }
        }
        va_end(list);

        // Setting the pin mode
        for(byte i=0; i<totalLeds; i++) {
            pinMode(leds[i], OUTPUT);
            Serial.print("Setting pin "); Serial.print(leds[i]); Serial.println(" as LED");
            maxValue *= 2;
        }
        pinMode(b1, INPUT);
        pinMode(b2, INPUT);
    }

    void update() {
        if (digitalRead(btn1) == HIGH) {
            if (!pressed) {
                updateCounter(false);
                pressed = true;
            }
        }
        else if (digitalRead(btn2) == HIGH) {
            if (!pressed) {
                updateCounter(true);
                pressed = true;
            }
        }
        else {
            pressed = false;
        }
    }

private:
    bool isPinValid(int pin) {  // Checks for pin to be in valid range
        return (pin > 0 && pin < 30);
    }
    
    void updateCounter(bool increase) {
        counter += increase ? 1 : -1;
        counter %= maxValue;
        Serial.print("New counter value: "); Serial.println(counter);

        // Turning off all LEDs
        for(byte i=0; i<totalLeds; i++)
            digitalWrite(leds[i], LOW);

        int res = counter;
        byte i=totalLeds-1;
        while(res > 0) {
            if (res % 2 == 1 ) {
                digitalWrite(leds[i], HIGH);
            }
            res /= 2;
            i--;
        }
    }
};

BinaryLed *binaryLed;

void setup() {
    // First 2 inputs are button pins, then arbitrary led pins from left to right
    binaryLed = new BinaryLed(2, 12, 5, 4, 7, 10);
}

void loop() {
    binaryLed->update();
}
