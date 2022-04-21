#include "zdt3_problem.h"

#include <limits>


Zdt3Problem::Zdt3Problem() : ZdtProblem() {
    this->max_objectives.resize(2);
    this->min_objectives.resize(2);
    for (int i = 0; i < 2; i++) {
        this->max_objectives[i] = std::numeric_limits<double>::lowest();
        this->min_objectives[i] = std::numeric_limits<double>::max();
    }
}

double Zdt3Problem::f1(const Individual& x) { return x.features.at(0); }

double Zdt3Problem::f2(const Individual& x) {
    double sigma = 0;
    for (int i = 1; i < x.features.size(); i++) { sigma += x.features[i]; }
    double g = 1 + sigma * 9 / (n - 1);
    double f = f1(x);
    double h = 1 - sqrt(f / g) - (f / g) * sin(10 * M_PI * f);
    return g * h;
}

void Zdt3Problem::perfectParetoFront(std::vector<double>& f1, std::vector<double>& f2) {
    double step = 0.01;
    std::vector<double> domain0 = lineSpace(0, 0.0830015349, step);
    std::vector<double> domain1 = lineSpace(0.1822287280, 0.2577623634, step);
    std::vector<double> domain2 = lineSpace(0.4093136748, 0.4538821041, step);
    std::vector<double> domain3 = lineSpace(0.6183967944, 0.6525117038, step);
    std::vector<double> domain4 = lineSpace(0.8233317983, 0.8518328654, step);

    f1.insert(f1.end(), domain0.begin(), domain0.end());
    f1.insert(f1.end(), domain1.begin(), domain1.end());
    f1.insert(f1.end(), domain2.begin(), domain2.end());
    f1.insert(f1.end(), domain3.begin(), domain3.end());
    f1.insert(f1.end(), domain4.begin(), domain4.end());

    for (auto x : f1) {
        double yi = 1 - sqrt(x) - x * sin(10 * M_PI * x);
        f2.push_back(yi);
    }
}
