//
// Created by dmitry on 4/26/20.
//

#include "Timer.h"

Timer::Timer(float time_to_elapse) : _time_to_elapse(time_to_elapse) {

}

void Timer::Start() {
    this->restart();
}

bool Timer::Elapsed() {
    return this->getElapsedTime().asSeconds() >= _time_to_elapse;
}
