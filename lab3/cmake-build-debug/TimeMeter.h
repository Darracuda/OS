//
// Created by Diana Kolpakova on 14.12.2021.
//

#ifndef LAB3_TIMEMETER_H
#define LAB3_TIMEMETER_H


class TimeMeter {
private:
    std::chrono::time_point<std::chrono::system_clock> m_startTimePoint;
    std::chrono::time_point<std::chrono::system_clock> m_stopTimePoint;
    enum state {
        INITIAL,
        STARTED,
        STOPPED
    } m_state;
public:
    TimeMeter();

    void start();
    void stop();
    void print();
};


#endif //LAB3_TIMEMETER_H
