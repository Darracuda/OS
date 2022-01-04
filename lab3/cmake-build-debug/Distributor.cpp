//
// Created by Diana Kolpakova on 14.12.2021.
//

#include <cmath>
#include <vector>
#include "Distributor.h"

Distributor::Distributor(int workers, int workItems) {
    m_workers = workers;
    m_workItems = workItems;
}

std::vector<int> Distributor::distribute() {
    std::vector<int> workLoad;

    for (int i = 0; i < m_workers; i++)
        workLoad.push_back(0);

    int averageWorkItems = m_workItems / m_workers;
    int workItemsLeft = m_workItems % m_workers;

    for (int i = 0; i < m_workers; i++) {
        workLoad[i] = averageWorkItems;
    }

    for (int i = 0; i < fmin(m_workers, workItemsLeft); i++) {
        workLoad[i] = averageWorkItems + 1;
    }
    return workLoad;
}
