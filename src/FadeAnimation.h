#ifndef LIBRARIES_FADEANIMATION_H
#define LIBRARIES_FADEANIMATION_H

#include <Animation.h>
#include <LedStrip.h>
#include <ColourProvider.h>

class FadeAnimation : public Animation {
public:
    FadeAnimation(LedStrip *strip, unsigned int count, unsigned int speed);

    void turnOn(ColourProvider* colourProvider);

    void turnOff(ColourProvider* colourProvider);

    void loop(ColourProvider* colourProvider);

    void resetAnimation();

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
    uint8_t _current_brightness;

    void handleTurningOn(ColourProvider* colourProvider);

    void handleTurningOff(ColourProvider* colourProvider);

    uint32_t adjustColour(uint32_t colour);

    uint32_t composeColour(uint8_t red, uint8_t green, uint8_t blue);
};

#endif //LIBRARIES_FADEANIMATION_H
