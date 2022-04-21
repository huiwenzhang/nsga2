#pragma once

#include "zdt_problem.h"

class Zdt1Problem : public ZdtProblem {
public:
    Zdt1Problem();
    ~Zdt1Problem() {}
    double f1(const Individual& other) override;
    double f2(const Individual& other) override;

    void perfectParetoFront(std::vector<double>& f1, std::vector<double>& f2) override;
};