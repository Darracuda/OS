//
// Created by Diana Kolpakova on 14.12.2021.
//

#include <chrono>
#include <iostream>
#include "TimeMeter.h"

void TimeMeter::start() {
    if (m_state != INITIAL) throw std::out_of_range("state != initial");

    m_startTimePoint = std::chrono::system_clock::now();
    m_state = STARTED;
}

void TimeMeter::stop() {
    if (m_state != STARTED) throw std::out_of_range("state != started");

    m_stopTimePoint = std::chrono::system_clock::now();
    m_state = STOPPED;
}

void TimeMeter::print() {
    if (m_state != STOPPED) throw std::out_of_range("state != stopped");

    std::chrono::duration<double> elapsed_seconds = m_stopTimePoint-m_startTimePoint;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s" << std::endl;
}

TimeMeter::TimeMeter() {
    m_state = INITIAL;
}


