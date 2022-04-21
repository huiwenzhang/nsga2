#pragma once

#include "zdt_problem.h"

class Zdt3Problem : public ZdtProblem {
public:
    Zdt3Problem();
    ~Zdt3Problem() {}
    double f1(const Individual& other) override;
    double f2(const Individual& other) override;

    void perfectParetoFront(std::vector<double>& f1, std::vector<double>& f2) override;
};