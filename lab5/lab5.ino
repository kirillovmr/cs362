


#include "Timer.hpp"

class LightSensor {
private:
    byte leds[4];
    int sensorPin;

    int maxPhotoValue = 100;
    int sensorValue;
    byte mode = 0;
    int bias[5];

public:
    LightSensor(byte l1, byte l2, byte l3, byte l4, int sensor): sensorPin(sensor) {
        leds[0] = l1;
        leds[1] = l2;
        leds[2] = l3;
        leds[3] = l4;
        
        pinMode(sensorPin, INPUT);
        pinMode(l1, OUTPUT);
        pinMode(l2, OUTPUT);
        pinMode(l3, OUTPUT);
        pinMode(l4, OUTPUT);

        // Saving the initial sensor value
        bias[0] = analogRead(sensorPin);
        Serial.print("0 leds = "); Serial.println(bias[0]);
        delay(100);

        // Collect biases
        for (byte i=0; i<4; i++) {
            digitalWrite(leds[i], HIGH);
            delay(100);
            bias[i+1] = analogRead(sensorPin);
            Serial.print(i+1); Serial.print(" leds = "); Serial.println(sensorValue);
        }

        // Turning all the lights off
        for (byte i=0; i<4; i++)
            digitalWrite(leds[i], LOW);
    }

    void update() {
        sensorValue = analogRead(sensorPin);
        Serial.print("Photo = "); Serial.println(sensorValue);
        
//        if (sensorValue > maxPhotoValue)
//            maxPhotoValue = sensorValue;

        // Unbiasing the value
        int unbiasedValue = sensorValue;
        if (mode > 0) {
            unbiasedValue = sensorValue - (bias[mode] - bias[0]);
        }
        
        int revertedValue = maxPhotoValue - sensorValue;
        mode = 0;
        for (byte i=0; i<=maxPhotoValue; i+= maxPhotoValue/4) {
            digitalWrite(leds[mode++], i < revertedValue ? HIGH : LOW);
        }
        
        int sensorValueAfter = analogRead(sensorPin);
        Serial.print("mode = "); Serial.println(mode);
    }
};

class MusicPlayer {
private:
    byte buzzerPin;
    int sensorPin;
    bool playing = false; // Should the "music" play or not
    bool m_tone = true; // Should the tone be high or low at the next update
    
    unsigned long *interval;
    unsigned long x1Interval = 500;

public:
    MusicPlayer(byte buzzer, int sensor): buzzerPin(buzzer), sensorPin(sensor) {
        pinMode(buzzerPin, OUTPUT);
        pinMode(sensorPin, INPUT);
    }

    // Saves the pointer to the timer interval so that we can modify it
    void setIntervalPointer(unsigned long *ptr) {
        interval = ptr;
    }

    // Plays sound or keeps silence
    void playMusic() {
        if (interval == nullptr)
            return;

        if (playing == false)
            return;

        // Stop sound
        if (m_tone == false) {
            noTone(buzzerPin);
            m_tone = !m_tone;
            return;
        }

        // Make sound
        tone(buzzerPin, 500);
        x1Interval = 1000;
        m_tone = !m_tone;
    }

    // Reads value from potentiometer
    void readSensor() {
        if (interval == nullptr)
            return;
            
        int sensorValue = analogRead(sensorPin);
        sensorValue = sensorValue > 1000 ? 1000 : sensorValue;

//        Serial.print("Potentiometer = "); Serial.println(sensorValue);
        
        unsigned long newInterval;

        if (sensorValue < 1000) {
            // Slow down the interval depending on sensor value
            newInterval = (sensorValue/1000.0) * x1Interval;
            playing = true;
        }
        // If potentiometer is on max malue
        else {
            noTone(buzzerPin);
            playing = false;
            return;
        }

        // Just in case, limiting the interval
        if (newInterval > x1Interval)
            newInterval = x1Interval;
        
        *interval = newInterval;
//        Serial.print("Interval = "); Serial.println(newInterval);
    }
};

Timer *timer;
LightSensor *lightSensor;
MusicPlayer *musicPlayer;

void setup() {
    Serial.begin(9600);
    
    timer = new Timer;
    lightSensor = new LightSensor(7,6,5,4,A2);
    musicPlayer = new MusicPlayer(9,A0);

    // Frequency of updating the light sendor
    timer->add( [lightSensor](){ lightSensor->update(); }, 1000 );

    // Frequency of updating the potentiometer
    timer->add( [musicPlayer](){ musicPlayer->readSensor(); }, 300 );

    // Creating timer for music playing
    unsigned long *intervalPtr = timer->add( [musicPlayer](){ musicPlayer->playMusic(); }, 0 );
    musicPlayer->setIntervalPointer(intervalPtr);
}

void loop() {
    timer->update();
}
