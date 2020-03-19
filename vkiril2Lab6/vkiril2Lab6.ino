//
//  Viktor Kirillov - 677621632
//  Lab 6 - Communication
//  Description: Prompts user for current time and then displays accurate time on LCD
//  Assumptions: LCD is connected to pins 12, 11, 5, 4, 3, 2
//  References: Time library <https://github.com/PaulStoffregen/Time>
//

#include <LiquidCrystal.h>
#include <TimeLib.h>
#include "Timer.hpp"

class LCDScroller {
private:
    const byte m_width, m_height;
    LiquidCrystal *m_lcd;
    
public:
    LCDScroller(byte width, byte height): m_width(width), m_height(height) { }

    void setup(LiquidCrystal *lcd) {
        m_lcd = lcd;
        m_lcd->begin(m_width, m_height);

        m_lcd->setCursor(0, 0);
        m_lcd->print("Current time");
        m_lcd->setCursor(0,1);
        m_lcd->print("Setting up...");

        takeInput();
    }

    void takeInput() {
        long userInputs[6];
        char prompts[6][20] = { "Month", "Day", "Year", "Hours", "Minutes", "Seconds" };
      
        byte i = 0;
        while(i < 6) {
            Serial.print("Enter "); Serial.print(prompts[i]); Serial.println(" value: ");
            long tmp = Serial.parseInt();
    
            // Value range checking
            switch(i) {
                case 0: // Month
                    if (tmp < 1 || tmp > 12) {
                        Serial.println("Please give the value in the range [1..12].");
                        continue;
                    }
                    break;
                case 1: // Day
                    if (tmp < 1 || tmp > 31) {
                        Serial.println("Please give the value in the range [1..31].");
                        continue;
                    }
                    break;
                case 2: // Year
                    if (tmp < 1970 || tmp > 2100) {
                        Serial.println("Please give the value in the range [1970..2100].");
                        continue;
                    }
                    break;
                case 3: // Hours
                    if (tmp < 0 || tmp > 23) {
                        Serial.println("Please give the value in the range [0..23].");
                        continue;
                    }
                    break;
                case 4: // Minutes
                    if (tmp < 0 || tmp > 59) {
                        Serial.println("Please give the value in the range [0..59].");
                        continue;
                    }
                    break;
                case 5: // Seconds
                    if (tmp < 0 || tmp > 59) {
                        Serial.println("Please give the value in the range [0..59].");
                        continue;
                    }
                    break;
            }
            
            userInputs[i] = tmp;
            Serial.print(prompts[i]); Serial.print(" value was set to "); Serial.println(userInputs[i]);
            i++;
        }

        // Setting the time
        setTime(userInputs[3], userInputs[4], userInputs[5], userInputs[1], userInputs[0], userInputs[2]);
    }

    void update() {
        m_lcd->clear();

        // Draw Date
        m_lcd->setCursor(0, 0);
        m_lcd->print("Date: ");
        m_lcd->print(month()); m_lcd->print("/"); 
        m_lcd->print(day()); m_lcd->print("/"); 
        m_lcd->print(year());

        // Draw Time
        m_lcd->setCursor(0,1);
        m_lcd->print("Time: ");
        int h = hour(), m = minute(), s = second();
        m_lcd->print(h < 10 ? "0" : ""); m_lcd->print(h); m_lcd->print(":"); 
        m_lcd->print(m < 10 ? "0" : ""); m_lcd->print(m); m_lcd->print(":"); 
        m_lcd->print(s < 10 ? "0" : ""); m_lcd->print(s);
    }
};

LCDScroller *controller;
Timer *timer;

void setup() {
    // Setting prompt timeout for 30 seconds
    Serial.setTimeout(30000);
    Serial.begin(9600);
  
    LiquidCrystal *lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);

    // Controls the whole LCD
    controller = new LCDScroller(16, 2);
    controller->setup(lcd);

    // Controls the time defferences imitating delays
    timer = new Timer;

    // Timer that would display time on LCD
    timer->add( [controller](){ controller->update(); }, 1000 );

    // Adding timer for debugging purposes
    timer->add( [](){ 
        Serial.print(month()); Serial.print("/"); 
        Serial.print(day()); Serial.print("/"); 
        Serial.print(year()); Serial.print(" ");

        Serial.print(hour()); Serial.print(":"); 
        Serial.print(minute()); Serial.print(":"); 
        Serial.println(second());
    }, 1000 );
}

void loop() {
    timer->update();
}
