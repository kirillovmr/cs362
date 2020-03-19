//
//  Viktor Kirillov - 677621632
//  Lab 3 - Scrolling output
//  Description - Displays name on the first line and a scrolling text on the second
//
//      [    name    ]      ->     [    name    ]     -> ... ->     [    name    ]
//      [super long j]oke   ->    s[uper long jo]ke   -> ... -> ..jo[ke   super l]ong..
//                                                                                \
//        \           \             \            \                    /     \       \___  nextOccurrenceIndex
//        first         no        posIndex    still no            spaces    second          which  temporary
//      occurance    truncation              truncation         after 1st.  occurance        truncates joke
//
//
//  LCD is connected to ports 12, 11, 5, 4, 3, 2
//  No references
//

#include <LiquidCrystal.h>
#include <string.h>
#include "Timer.hpp"

class LCDScroller {
private:
    const byte m_width, m_height;
    char *m_name, *m_joke;
    byte nameLen = 0, jokeLen = 0;
    
    LiquidCrystal *m_lcd;

    byte numSpacesAfterJoke = 5;
    byte posIndex = 0;  // from where the joke should be printed next time
    byte nextOccurrenceIndex; // where to put '\0' for the next occurence of joke
    
public:
    LCDScroller(byte width, byte height): m_width(width), m_height(height) {
        m_name = new char[width+1];
        m_joke = new char[width*2+1];
    }

    void setup(LiquidCrystal *lcd, const char *name, const char *joke) {
        m_lcd = lcd;
        m_lcd->begin(m_width, m_height);

        // Copying parameters into member variables
        strncpy(m_name, name, m_width);
        strncpy(m_joke, joke, m_width*2);

        // Getting lengths of strings
        nameLen = strlen(m_name);
        jokeLen = strlen(m_joke);

        // Printing name in the center of the 1st line
        m_lcd->setCursor((m_width-nameLen)/2, 0);
        m_lcd->print(m_name);

        // Printing joke on the second line
        m_lcd->setCursor(0,1);
        m_lcd->print(m_joke);
        
        nextOccurrenceIndex = jokeLen + numSpacesAfterJoke;
    }

    void shiftLeft() {
        m_lcd->clear();

        // Draw name on the same position
        m_lcd->setCursor((m_width-nameLen)/2, 0);
        m_lcd->print(m_name);

        // Draw joke starting from posIndex
        m_lcd->setCursor(0,1);
        m_lcd->print(posIndex > jokeLen ? m_joke+jokeLen : m_joke+posIndex);

        // Draw second occurence of joke if needed
        if (nextOccurrenceIndex < m_width) {
            // Save character which would be replaced by '\0'
            byte tmpIdx = m_width - nextOccurrenceIndex;
            char temp = m_joke[tmpIdx];

            // Truncating the output
            m_joke[tmpIdx] = '\0';
            
            m_lcd->setCursor(nextOccurrenceIndex, 1);
            m_lcd->print(m_joke);

            // Return the temp character back
            m_joke[tmpIdx] = temp;
        }

        // Define the next position counter
        posIndex += 1;
        nextOccurrenceIndex -= 1;
        if (posIndex >= jokeLen + numSpacesAfterJoke) {
            posIndex = 0;
            nextOccurrenceIndex = jokeLen + numSpacesAfterJoke;
        }
    }
 
};

LCDScroller *controller;
Timer *timer;

void setup() {
    LiquidCrystal *lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);

    // Controls the whole LCD
    controller = new LCDScroller(16, 2);
    controller->setup(lcd, "Viktor", "This is a programmers joke.");

    // Controls the time defferences imitating delays
    timer = new Timer;
    timer->add( [controller](){ controller->shiftLeft(); }, 300);
}

void loop() {
    timer->update();
}
