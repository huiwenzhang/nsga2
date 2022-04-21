#pragma once

#include "individual.h"

class Population {
public:
    Population() = default;
    ~Population() {}

    inline int size() { return population.size(); }
    inline void append(const Individual& other) { population.push_back(other); }
    inline void extend(const std::vector<Individual>& others) {
        population.insert(population.end(), others.begin(), others.end());
    }

public:
    std::vector<Individual> population;
    std::vector<std::vector<Individual*>> fronts;
};

using PopulationPtr = std::shared_ptr<Population>;