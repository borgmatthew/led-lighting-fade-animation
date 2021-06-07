#ifndef LIBRARIES_FADEANIMATION_H
#define LIBRARIES_FADEANIMATION_H

#include <Animation.h>
#include <LedStrip.h>

class FadeAnimation : public Animation {
public:
    FadeAnimation(LedStrip *strip, unsigned int count, unsigned int speed, uint32_t colour);

    void turnOn();

    void turnOff();

    void loop();

    void resetAnimation();

    uint32_t getColour();
    void setColour(uint32_t colour);

    int getSpeed();
    void setSpeed(unsigned int speed);

private:
    enum AnimationState {
        TURNING_OFF = 0,
        TURNING_ON = 1,
        ON = 2,
        OFF = 3
    };

    AnimationState _state;
    unsigned int _speed;
    unsigned int _count;
    unsigned long _lastMillis;
    LedStrip *_strip;
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
    unsigned int _steps = 255;

    uint8_t _current_red = 0;
    uint8_t _current_green = 0;
    uint8_t _current_blue = 0;
    uint8_t _current_step = _steps;

    void handleTurningOn();

    void handleTurningOff();

    void splitColour(uint32_t colour);

    uint32_t composeColour(uint8_t red, uint8_t green, uint8_t blue);
};

#endif //LIBRARIES_FADEANIMATION_H
