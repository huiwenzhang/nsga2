#include "nsga2.h"
#include "zdt3_problem.h"
#include "zdt1_problem.h"
#include <fstream>

int main(int argc, char** argv) {

    std::shared_ptr<ZdtProblem> problem = std::make_shared<Zdt1Problem>();

    std::vector<double> f1, f2;
    problem->perfectParetoFront(f1, f2);

    // save
    std::ofstream out("front/perfect_front.txt", std::ofstream::trunc);
    out << "f1" << " " << "f2" << "\n";
    for (int i = 0; i < f1.size(); i++) {
        out << f1.at(i) << " " << f2.at(i) << "\n";
    }
    out.close();

    NSGA2 solver(problem);
    solver.run();

    return 0;
}