//
// Created by Diana Kolpakova on 14.12.2021.
//

#ifndef LAB3_DISTRIBUTOR_H
#define LAB3_DISTRIBUTOR_H


class Distributor {
private:

    int m_workers;
    int m_workItems;

public:
    Distributor(int workers, int workItems);
    std::vector<int> distribute();
};


#endif //LAB3_DISTRIBUTOR_H
