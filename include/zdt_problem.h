#pragma once

#include <math.h>

#include "individual.h"
#include "random_generator.h"

class ZdtProblem {
public:
    ZdtProblem();
    ~ZdtProblem() {}

    virtual double f1(const Individual& x) = 0;
    virtual double f2(const Individual& x) = 0;
    virtual void perfectParetoFront(std::vector<double>& f1, std::vector<double>& f2) = 0;

    void calcObjectives(Individual& s);
    void generateIndividual(Individual& s);

    std::vector<double> min_objectives;
    std::vector<double> max_objectives;

protected:
    std::vector<double> lineSpace(double start, double stop, double step);
    const int n = 30;
};

using ZdtProblemPtr = std::shared_ptr<ZdtProblem>;