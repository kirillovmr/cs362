//
//  Lab 4 - More complicated input
//
//  Created by Viktor Kirillov on 2/1/20.
//  Copyright © 2020 Viktor Kirillov. All rights reserved.
//

#include <LiquidCrystal.h>
#include <string.h>
#include "Timer.hpp"

class LCDScroller {
private:
    const byte m_width, m_height;
    char *m_name;
    byte nameLen = 0;

    char values[6][20] = {"Starting up...", "Dark", "Partially dark", "Medium", "Partially light", "Full lit"};
    byte prevValue = 0;
    
    LiquidCrystal *m_lcd;
    
public:
    LCDScroller(byte width, byte height): m_width(width), m_height(height) {
        m_name = new char[width+1];
    }

    void setup(LiquidCrystal *lcd, const char *name) {
        m_lcd = lcd;
        m_lcd->begin(m_width, m_height);

        // Copying parameters into member variables
        strncpy(m_name, name, m_width);

        // Getting lengths of strings
        nameLen = strlen(m_name);

        // Printing name in the center of the 1st line
        m_lcd->setCursor((m_width-nameLen)/2, 0);
        m_lcd->print(m_name);

        // Printing value on the second line
        m_lcd->setCursor(0,1);
        m_lcd->print(values[prevValue]);
    }

    void update(int value) {
        // Dividing by 6 just because its a reasonable value in my room
        int newValue = value/6 + 1;

        // We have max 5 values to display  
        newValue = newValue > 5 ? 5 : newValue;
        if (newValue == prevValue) 
            return;
        
        Serial.print(value); Serial.print(" -> "); Serial.println(newValue);
    
        m_lcd->clear();

        // Draw name on the same position
        m_lcd->setCursor((m_width-nameLen)/2, 0);
        m_lcd->print(m_name);

        // Draw value
        m_lcd->setCursor(0,1);
        m_lcd->print(values[newValue]);

        prevValue = newValue;
    }
};

LCDScroller *controller;
Timer *timer;

int photoPin = A0;

void setup() {
    LiquidCrystal *lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);

    // Controls the whole LCD
    controller = new LCDScroller(16, 2);
    controller->setup(lcd, "Lab 4");

    // Controls the time defferences imitating delays
    timer = new Timer;
    timer->add( [controller](){ controller->update(analogRead(photoPin)); }, 400);

    pinMode(photoPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    timer->update();
}
