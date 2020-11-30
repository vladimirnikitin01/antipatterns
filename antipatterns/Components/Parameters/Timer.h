//
// Created by dmitry on 4/26/20.
//

#ifndef ANTIPATTERNS_TIMER_H
#define ANTIPATTERNS_TIMER_H


#include <SFML/System/Clock.hpp>

class Timer : public sf::Clock {
public:
    explicit Timer(float time_to_elapse);

    void Start();

    bool Elapsed();

private:
    float _time_to_elapse;
};


#endif //ANTIPATTERNS_TIMER_H
