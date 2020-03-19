//
//  Timer.hpp
//  timer.update() should be called in Arduino update() function, and each
//    function added to the timer will be called after specified amount of time
//
//  Created by Viktor Kirillov on 1/30/20.
//  Copyright © 2020 Viktor Kirillov. All rights reserved.
//

#ifndef Timer_hpp
#define Timer_hpp

#include "Arduino.h"
#include <stdlib.h>

class Timer {
private:
    class FuncObject {
    public:
        void (*m_func)(void);
        unsigned long m_interval;
        unsigned long m_previous;
        
        FuncObject(void (*func)(void), unsigned long interval): m_previous(0) {
            m_func = func;
            m_interval = interval;
        }
    };

    FuncObject **objects;
    int size, capacity;

    unsigned long currentTime;
    
public:
    Timer(): size(0), capacity(2) {
        objects = (FuncObject**) malloc(sizeof(FuncObject)*capacity);
    }

    // Add new function to timer
    void add(void (*m_func)(void), unsigned long interval) {
        if (size == capacity)
            growArray();

        // Save new object
        objects[size++] = new FuncObject(m_func, interval);
    }

    // Should be runned in a loop
    void update() {
        // Get the current time
        currentTime = millis();

        // For each object in a timer
        for (int i=0; i<size; i++) {
            // If it's a time to run it
            if (currentTime - objects[i]->m_previous >= objects[i]->m_interval) {
                objects[i]->m_func();
                objects[i]->m_previous = currentTime;
            }
        }
    }
    
private:
    // Grows array of objects when filled
    void growArray() {
        capacity += 2;
        FuncObject **newObjects = (FuncObject**) malloc(sizeof(FuncObject)*capacity);
        for (int i=0; i<size; i++)
            newObjects[i] = objects[i];
        free(objects);
        objects = newObjects;
    }
};

#endif /* Timer_hpp */
