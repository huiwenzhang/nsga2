#include "zdt_problem.h"

#include <limits>


ZdtProblem::ZdtProblem() {
    this->max_objectives.resize(2);
    this->min_objectives.resize(2);
    for (int i = 0; i < 2; i++) {
        this->max_objectives[i] = std::numeric_limits<double>::lowest();
        this->min_objectives[i] = std::numeric_limits<double>::max();
    }
}

void ZdtProblem::calcObjectives(Individual& s) {
    s.objectives.clear();
    s.objectives.push_back(f1(s));
    s.objectives.push_back(f2(s));

    for (int i = 0; i < s.objectives.size(); i++) {
        if (this->min_objectives[i] > s.objectives[i]) {
            this->min_objectives[i] = s.objectives[i];
        }
        if (this->max_objectives[i] < s.objectives[i]) {
            this->max_objectives[i] = s.objectives[i];
        }
    }
}

void ZdtProblem::generateIndividual(Individual& s) {
    s.features.clear();
    for (int i = 0; i < n; i++) { s.features.push_back(randomFloat01Close()); }
    calcObjectives(s);
}

std::vector<double> ZdtProblem::lineSpace(double start, double stop, double step) {
    std::vector<double> linespaced;
    int num = std::round((stop - start) / step);

    if (num > 1) {
        for (int i = 0; i < num; ++i) { linespaced.push_back(start + step * i); }
        linespaced.push_back(stop);
    }
    return linespaced;
}