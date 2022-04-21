#pragma once

#include <memory>
#include <vector>

class Individual {
public:
    Individual() = default;
    ~Individual() {}

    inline void setObjectives(const std::vector<double>& obj) { this->objectives = obj; }

    inline bool dominates(const Individual& other) {
        bool better_than = true;
        bool no_worse_than = true;
        for (size_t i = 0; i < this->objectives.size(); i++) {
            no_worse_than = no_worse_than && (this->objectives[i] <= other.objectives[i]);
            better_than = better_than || (this->objectives[i] < other.objectives[i]);
        }
        return no_worse_than && better_than;
    }

public:
    int rank;                                      // Pareto等级
    double crowd_distance;                         // 拥挤度
    std::vector<double> features;                  // 自变量
    std::vector<double> objectives;                // 目标函数的值
    std::vector<Individual*> dominated_solutions;  // 支配的个体集合
    int dominate_cnt;
};
