#include "nsga2.h"

#include <assert.h>

#include <fstream>
#include <iostream>

NSGA2::NSGA2(ZdtProblemPtr& problem) : ptr_problem_(problem) { printConfig(); }

NSGA2::NSGA2(ZdtProblemPtr& problem, int num_generations, int num_individual)
        : ptr_problem_(problem) {
    config_.num_generations = num_generations;
    config_.num_individuals = num_individual;

    printConfig();
}

void NSGA2::printConfig() const {
    // minfo("Run NSGA2, configs #generations %d, #individuals %d, #mutation gens %d, #tour particips "
    //       "%d, mutation prob %.3f",
    //       config_.num_generations,
    //       config_.num_individuals,
    //       config_.num_mutation_gens,
    //       config_.num_tour_particips,
    //       config_.mutation_prob);
}

PopulationPtr NSGA2::createInitialPopulation() {
    PopulationPtr pop = std::make_shared<Population>();
    for (int i = 0; i < config_.num_individuals; i++) {
        Individual s;
        ptr_problem_->generateIndividual(s);
        pop->append(s);
    }
    return pop;
}

void NSGA2::crossOver(const Individual& s1,
                      const Individual& s2,
                      Individual& child1,
                      Individual& child2) {
    ptr_problem_->generateIndividual(child1);
    ptr_problem_->generateIndividual(child2);
    int num_genes = child1.features.size();
    int cross_ids = randomIntCloseAOpenB(0, num_genes);
    for (int i = 0; i < num_genes; i++) {
        if (i == cross_ids) {
            child1.features[i] = s2.features[i];
            child2.features[i] = s1.features[i];
        } else {
            child1.features[i] = s1.features[i];
            child2.features[i] = s2.features[i];
        }
    }
}

void NSGA2::mutate(Individual& child) {
    std::vector<int> ids;
    for (int i = 0; i < child.features.size(); i++) { ids.emplace_back(i); }
    std::vector<int> genes_to_mutate;
    std::random_shuffle(ids.begin(), ids.end());
    std::copy_n(ids.begin(), config_.num_mutation_gens, std::back_inserter(genes_to_mutate));
    // std::sample(
    //         ids.begin(), ids.end(), std::back_inserter(genes_to_mutate),
    //         config_.num_mutation_gens);
    for (auto gene : genes_to_mutate) {
        child.features.at(gene) = child.features.at(gene) - config_.mutation_prob / 2.0 +
                                  randomFloat01Close() * config_.mutation_prob;
        if (child.features.at(gene) < 0) {
            child.features.at(gene) = 0.0;
        } else if (child.features.at(gene) > 1) {
            child.features.at(gene) = 1.0;
        }
    }
}

bool NSGA2::crowdOperator(const Individual* s1, const Individual* s2) {
    return s1->rank < s2->rank ||
           ((s1->rank == s2->rank) && (s1->crowd_distance > s2->crowd_distance));
}

void NSGA2::tournament(const PopulationPtr& pop, Individual& best) const {
    std::vector<Individual> participants;
    std::vector<int> ids;
    for (int i = 0; i < pop->size(); i++) { ids.emplace_back(i); }
    std::random_shuffle(ids.begin(), ids.end());
    for (int j = 0; j < config_.num_tour_particips; j++) {
        participants.emplace_back(pop->population.at(ids[j]));
    }

    best = participants.at(0);
    for (auto participant : participants) {
        if (crowdOperator(&participant, &best)) { best = participant; }
    }
}

PopulationPtr NSGA2::createChildren(const PopulationPtr& parent) {
    PopulationPtr children = std::make_shared<Population>();
    children->population.clear();
    while (children->size() < parent->size()) {
        Individual parent1;
        tournament(parent, parent1);
        Individual parent2 = parent1;
        while (parent1.features == parent2.features) { tournament(parent, parent2); }

        Individual child1, child2;
        crossOver(parent1, parent2, child1, child2);
        mutate(child1);
        mutate(child2);
        ptr_problem_->calcObjectives(child1);
        ptr_problem_->calcObjectives(child2);
        children->append(child1);
        children->append(child2);
    }
    return children;
}

// bool NSGA2::sortObjective(const Individual& s1, const Individual& s2, int m) const {
//     assert(m < s1.objectives.size());
//     return s1.objectives.at(m) > s2.objectives.at(m);
// }

void NSGA2::calcCrowdingDistance(std::vector<Individual*>& front) {
    if (front.size() < 1) return;

    for (auto& individual : front) { individual->crowd_distance = 0.0; }

    for (int m = 0; m < front.at(0)->objectives.size(); m++) {
        std::sort(front.begin(), front.end(), [m](Individual* a, Individual* b) {
            return a->objectives[m] > b->objectives[m];
        });
        front.front()->crowd_distance = ptr_problem_->max_objectives.at(m);
        front.back()->crowd_distance = ptr_problem_->min_objectives.at(m);

        for (int i = 1; i < front.size() - 1; i++) {
            front.at(i)->crowd_distance +=
                    (front.at(i + 1)->objectives[m] - front.at(i - 1)->objectives[m]) /
                    (ptr_problem_->max_objectives[m] - ptr_problem_->min_objectives[m]);
        }
    }
}

void NSGA2::fastNondominatedSort(PopulationPtr& population) {
    population->fronts.clear();
    population->fronts.resize(1);

    for (int i = 0; i < population->size(); i++) {
        Individual* p = &population->population.at(i);
        p->dominate_cnt = 0;
        p->dominated_solutions.clear();

        for (int j = 0; j < population->size(); j++) {
            if (i == j) { continue; }
            Individual* q = &population->population.at(j);
            if (p->dominates(*q)) {
                p->dominated_solutions.push_back(q);
            } else if (q->dominates(*p)) {
                p->dominate_cnt += 1;
            }
        }

        if (p->dominate_cnt == 0) {
            population->fronts.at(0).push_back(p);
            p->rank = 0;
        }
    }

    int i = 0;
    while (population->fronts.at(i).size() > 0) {
        std::vector<Individual*> front;
        for (int j = 0; j < population->fronts.at(i).size(); j++) {
            Individual* p = population->fronts.at(i)[j];
            for (int k = 0; k < p->dominated_solutions.size(); k++) {
                Individual* q = p->dominated_solutions.at(k);
                q->dominate_cnt -= 1;
                if (q->dominate_cnt == 0) {
                    q->rank = i + 1;
                    front.push_back(q);
                }
            }
        }
        i++;
        population->fronts.push_back(front);
    }
}

void NSGA2::run() {
    PopulationPtr parent = createInitialPopulation();
    fastNondominatedSort(parent);
    for (auto& front : parent->fronts) { calcCrowdingDistance(front); }

    PopulationPtr children, old_parent;

    children = createChildren(parent);
    for (int i = 0; i < config_.num_generations; i++) {
        parent->extend(children->population);
        fastNondominatedSort(parent);

        // minfo("Parent size %d", ptr_parent_->size());

        // 构建下一代种群
        PopulationPtr new_population = std::make_shared<Population>();
        int front_num = 0;
        while (new_population->size() + parent->fronts.at(front_num).size() <=
               config_.num_individuals) {
            calcCrowdingDistance(parent->fronts.at(front_num));
            for (Individual* p : parent->fronts.at(front_num)) { new_population->append(*p); }
            front_num++;
        }

        std::sort(parent->fronts.at(front_num).begin(),
                  parent->fronts.at(front_num).end(),
                  crowdOperator);
        int diff = config_.num_individuals - new_population->size();
        for (int j = 0; j < diff; j++) { new_population->append(*parent->fronts.at(front_num)[j]); }

        old_parent = parent;
        parent = new_population;
        children = createChildren(parent);

        // save data
        if (i % 10 == 0) {
            std::cout << "NSGA iteration #" << i << std::endl;
            std::string filename = "front/" + std::to_string(i) + ".txt";
            saveParetoFront(filename, old_parent);
        }
    }
}

void NSGA2::saveParetoFront(const std::string& filename, const PopulationPtr& data) const {
    std::ofstream out(filename, std::ofstream::trunc);
    out << "f1"
        << " "
        << "f2"
        << "\n";

    std::vector<Individual*> front = data->fronts.at(0);

    for (auto pnt : front) {
        for (int i = 0; i < pnt->objectives.size(); i++) {
            if (i != pnt->objectives.size() - 1) {
                out << pnt->objectives.at(i) << " ";
            } else {
                out << pnt->objectives.at(i) << "\n";
            }
        }
    }
    out.close();
}