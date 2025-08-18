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


// Entropy for controlled probability distributions (probabilities may be unnormalized)
double shannon_entropy_from_probabilities(const std::vector<double>& probabilities) {
    double sum = 0.0;
    for (double p : probabilities) {
        if (p < 0.0) {
            continue;
        }
        sum += p;
    }
    if (sum <= 0.0) {
        return 0.0;
    }

    double entropy = 0.0;
    for (double p : probabilities) {
        if (p <= 0.0) {
            continue;
        }
        double normalized = p / sum;
        if (normalized > 0.0) {
            entropy -= normalized * std::log2(normalized);
        }
    }
    return entropy;
}


