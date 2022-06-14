#pragma once

#include <algorithm>

#include "population.h"
#include "zdt_problem.h"

class NSGA2 {
public:
    NSGA2(ZdtProblemPtr& problem, int num_generations, int num_individual);
    NSGA2(ZdtProblemPtr& problem);
    ~NSGA2() {}

    struct NsgaConfig {
        double mutation_prob = 0.2;
        int num_mutation_gens = 3;
        int num_tour_particips = 2;
        int num_generations = 150;
        int num_individuals = 50;
    };

    void inline setConfig(const NsgaConfig& config) { config_ = config; }

    void printConfig() const;

    void fastNondominatedSort(PopulationPtr& population);

    void calcCrowdingDistance(std::vector<Individual*>& front);

    PopulationPtr createInitialPopulation();

    PopulationPtr createChildren(const PopulationPtr& parent);

    void tournament(const PopulationPtr& pop, Individual& best) const;

    void run();

private:
    void crossOver(const Individual& individual1,
                   const Individual& individual2,
                   Individual& child1,
                   Individual& child2);

    void mutate(Individual& child);

    static bool crowdOperator(const Individual* s1, const Individual* s2);

    void saveParetoFront(const std::string& filename, const PopulationPtr& data) const;


private:
    NsgaConfig config_;
    ZdtProblemPtr ptr_problem_;
};