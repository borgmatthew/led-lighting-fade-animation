#include "FadeAnimation.h"
#include <math.h>

FadeAnimation::FadeAnimation(LedStrip *strip, unsigned int count, unsigned int speed) {
    _state = FadeAnimation::OFF;
    _count = count;
    _speed = speed;
    _lastMillis = millis();
    _strip = strip;
    _current_brightness = 0;
}

uint32_t FadeAnimation::adjustColour(uint32_t colour) {
    float percentage = (float)_current_brightness / 255.0f;
    uint8_t red = (uint8_t) fmin(255, ceil(((colour >> 16) & 0xFF) * percentage));
    uint8_t green = (uint8_t) fmin(255, ceil(((colour >> 8) & 0xFF) * percentage));
    uint8_t blue = (uint8_t) fmin(255, ceil((colour & 0xFF) * percentage));

    return composeColour(red, green, blue);
}

uint32_t FadeAnimation::composeColour(uint8_t red, uint8_t green, uint8_t blue) {
    return red << 16 | green << 8 | blue;
}


void FadeAnimation::turnOn(ColourProvider* colourProvider) {
    if (_state == FadeAnimation::OFF || _state == FadeAnimation::TURNING_OFF) {
        _state = FadeAnimation::TURNING_ON;
    }
};

void FadeAnimation::turnOff(ColourProvider* colourProvider) {
    if (_state == FadeAnimation::ON || _state == FadeAnimation::TURNING_ON) {
        _state = FadeAnimation::TURNING_OFF;
    }
};

void FadeAnimation::loop(ColourProvider* colourProvider) {
    if (_state == FadeAnimation::TURNING_ON) {
        handleTurningOn(colourProvider);
    } else if (_state == FadeAnimation::TURNING_OFF) {
        handleTurningOff(colourProvider);
    } else if (_state == FadeAnimation::ON) {
        handleOn(colourProvider);
    }
};

void FadeAnimation::handleTurningOn(ColourProvider* colourProvider) {
    unsigned long currentMillis = millis();
    if (currentMillis > (_lastMillis + _speed)) {

        _current_brightness++;

        if (_current_brightness >= 255) {
            _current_brightness = 255;
            _state = FadeAnimation::ON;
        }

        for (unsigned int offset = 0; offset < _count; offset++) {
            uint32_t colour = colourProvider -> getColour(offset);
            _strip -> setPixelColour(offset, adjustColour(colour));
        }

        _strip -> update();

        _lastMillis = currentMillis;
    }
};

void FadeAnimation::handleTurningOff(ColourProvider* colourProvider) {
    unsigned long currentMillis = millis();
    if (currentMillis > (_lastMillis + _speed)) {

        _current_brightness--;

        if (_current_brightness <= 0) {
            _current_brightness = 0;
            _state = FadeAnimation::OFF;
        }

        for (unsigned int offset = 0; offset < _count; offset++) {
            uint32_t colour = colourProvider -> getColour(offset);
            _strip -> setPixelColour(offset, adjustColour(colour));
        }

        _strip -> update();

        _lastMillis = currentMillis;
    }
}

void FadeAnimation::handleOn(ColourProvider *colourProvider) {
    for(int i = 0; i < _count; i++) {
        uint32_t colour = colourProvider -> getColour(i);
        _strip -> setPixelColour(i, colour);
    }
    _strip -> update();
}

void FadeAnimation::resetAnimation() {
    _strip -> clear();
    _current_brightness = 0;
    _state = FadeAnimation::OFF;
}

void FadeAnimation::setSpeed(unsigned int speed) {
    _speed = speed;
}

int FadeAnimation::getSpeed() {
    return _speed;
}