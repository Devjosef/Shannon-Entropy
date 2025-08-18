// # Goal to calculate shannon entropy for a vector of actions
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

double  shannon_entropy(std::vector<int> actions) {
    std::map<int, int>counts;
    int total = actions.size();
    for (int action : actions) {
        counts[action]++;
    }
    double entropy = 0.0;
    for (auto& pair : counts) {
        double p = (double)pair.second / total;
        if (p > 0) entropy -= p * std::log2(p);
    }
    return entropy;
}


