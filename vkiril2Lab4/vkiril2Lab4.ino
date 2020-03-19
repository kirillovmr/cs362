//
//  Viktor Kirillov - 677621632
//  Lab 4 - More complicated input
//  Description: Displays the relative amount of the light in the room on LCD display.
//  Assumptions: Lowest and Largest values on the photocell are 200 and 800 respectively.
//               LCD is connected to ports 12, 11, 5, 4, 3, 2; Analog device is connected to pin A0.
//  No references.
//

#include <LiquidCrystal.h>
#include <string.h>
#include "Timer.hpp"

class LCDScroller {
private:
    const byte m_width, m_height;
    char *m_firstLine;
    byte firstLineLen = 0;

    char values[6][20] = {"Starting up...", "Dark", "Partially dark", "Medium", "Partially light", "Full lit"};
    int lowestValue = 250;
    int highestValue = 800;
    int prevValue = 0;
    
    LiquidCrystal *m_lcd;
    
public:
    LCDScroller(byte width, byte height): m_width(width), m_height(height) {
        m_firstLine = new char[width+1];
    }

    void setup(LiquidCrystal *lcd, const char *firstLine) {
        m_lcd = lcd;
        m_lcd->begin(m_width, m_height);

        // Copying parameters into member variables
        strncpy(m_firstLine, firstLine, m_width);

        // Getting lengths of strings
        firstLineLen = strlen(m_firstLine);

        // Printing firstLine in the center of the 1st line
        m_lcd->setCursor((m_width-firstLineLen)/2, 0);
        m_lcd->print(m_firstLine);

        // Printing value on the second line
        m_lcd->setCursor(0,1);
        m_lcd->print(values[prevValue]);
    }

    void update(int value) {
        int diapason = highestValue - lowestValue;
        int delta = diapason / 5;

        // Interpolating the value in the range
        int newValue = 1;
        for(int i=lowestValue; i<highestValue; i += delta) {
            if (i < value) {
                newValue += 1;
            }
        }

        // We have max 5 values to display  
        newValue = newValue > 5 ? 5 : newValue;
        if (newValue == prevValue) 
            return;
        
        Serial.print(value); Serial.print(" -> "); Serial.println(newValue);
    
        m_lcd->clear();

        // Draw firstLine on the same position
        m_lcd->setCursor((m_width-firstLineLen)/2, 0);
        m_lcd->print(m_firstLine);

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
//  Serial.println(analogRead(photoPin));
    timer->update();
}
