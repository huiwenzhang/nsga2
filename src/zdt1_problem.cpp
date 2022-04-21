#include "zdt1_problem.h"

#include <limits>


Zdt1Problem::Zdt1Problem() : ZdtProblem() {
    this->max_objectives.resize(2);
    this->min_objectives.resize(2);
    for (int i = 0; i < 2; i++) {
        this->max_objectives[i] = std::numeric_limits<double>::lowest();
        this->min_objectives[i] = std::numeric_limits<double>::max();
    }
}

double Zdt1Problem::f1(const Individual& x) { return x.features.at(0); }

double Zdt1Problem::f2(const Individual& x) {
    double sigma = 0;
    for (int i = 1; i < x.features.size(); i++) { sigma += x.features[i]; }
    double g = 1 + sigma * 9 / (n - 1);
    double f = f1(x);
    double h = 1 - sqrt(f / g);
    return g * h;
}

void Zdt1Problem::perfectParetoFront(std::vector<double>& f1, std::vector<double>& f2) {
    f1 = lineSpace(0, 1, 0.01);
    for (auto x : f1) {
        double yi = 1 - sqrt(x);
        f2.push_back(yi);
    }
}
