#include "FadeAnimation.h"

FadeAnimation::FadeAnimation(LedStrip *strip, unsigned int count, unsigned int speed, uint32_t colour) {
    _state = FadeAnimation::OFF;
    _count = count;
    _speed = speed;
    _lastMillis = millis();
    _strip = strip;
    splitColour(colour);
}

void FadeAnimation::splitColour(uint32_t colour) {
    _red = (uint8_t) ((colour >> 16) & 0xFF);
    _green = (uint8_t) ((colour >> 8) & 0xFF);
    _blue = (uint8_t) (colour & 0xFF);
}

uint32_t FadeAnimation::composeColour(uint8_t red, uint8_t green, uint8_t blue) {
    return red << 16 | green << 8 | blue;
}


void FadeAnimation::turnOn() {
    if (_state == FadeAnimation::OFF || _state == FadeAnimation::TURNING_OFF) {
        _state = FadeAnimation::TURNING_ON;
    }
};

void FadeAnimation::turnOff() {
    if (_state == FadeAnimation::ON || _state == FadeAnimation::TURNING_ON) {
        _state = FadeAnimation::TURNING_OFF;
    }
};

void FadeAnimation::loop() {
    if (_state == FadeAnimation::TURNING_ON) {
        handleTurningOn();
    } else if (_state == FadeAnimation::TURNING_OFF) {
        handleTurningOff();
    }
};

void FadeAnimation::handleTurningOn() {
    unsigned long currentMillis = millis();
    if (currentMillis > (_lastMillis + _speed)) {

        if (_current_red < _red) {
            _current_red += (_red - _current_red) / _current_step;
        }

        if (_current_green < _green) {
            _current_green += (_green - _current_green) / _current_step;
        }

        if (_current_blue < _blue) {
            _current_blue += (_blue - _current_blue) / _current_step;
        }

        _current_step -= 1;

        if (_current_step == 1) {
            _current_red = _red;
            _current_green = _green;
            _current_blue = _blue;
            _state = FadeAnimation::ON;
        }

        uint32_t colour = composeColour(_current_red, _current_green, _current_blue);

        for (unsigned int offset = 0; offset < _count; offset++) {
            _strip -> setPixelColour(offset, colour);
        }

        _strip -> update();

        _lastMillis = currentMillis;
    }
}

void FadeAnimation::handleTurningOff() {
    unsigned long currentMillis = millis();
    if (currentMillis > (_lastMillis + _speed)) {

        if (_current_red > 0) {
            _current_red -= (_current_red / (_steps - _current_step));
        }

        if (_current_green > 0) {
            _current_green -= (_current_green / (_steps - _current_step));
        }

        if (_current_blue > 0) {
            _current_blue -= (_current_blue / (_steps - _current_step));
        }

        _current_step += 1;

        if (_current_step == _steps) {
            _current_red = 0;
            _current_green = 0;
            _current_blue = 0;
            _state = FadeAnimation::OFF;
        }

        uint32_t colour = composeColour(_current_red, _current_green, _current_blue);

        for (unsigned int offset = 0; offset < _count; offset++) {
            _strip -> setPixelColour(offset, colour);
        }

        _strip -> update();


        _lastMillis = currentMillis;
    }
}

void FadeAnimation::resetAnimation() {
    _strip -> clear();
    _current_green = 0;
    _current_red = 0;
    _current_blue = 0;
    _current_step = _steps;
    _state = FadeAnimation::OFF;
}

void FadeAnimation::setSpeed(unsigned int speed) {
    _speed = speed;
}

int FadeAnimation::getSpeed() {
    return _speed;
}

void FadeAnimation::setColour(uint32_t colour) {
    splitColour(colour);
}

uint32_t FadeAnimation::getColour() {
    return composeColour(_red, _green, _blue);
}