#include <iostream>
#include <vector>
#include <map>
#include <cmath>

double shannon_entropy(std::vector<int> actions) {
    std::map<int, int> counts;
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

int main() {
    std::vector<std::vector<int>> windows = {
        {0, 0, 1, 2, 2, 0, 1}, // Mixed behavior
        {0, 0, 0, 0, 0, 0, 0}, // All hold (predictable)
        {0, 1, 2, 0, 1, 2, 0} // Maximum entropy
    };

    std::vector<double> market_vol = {2.5, 0.2, 3.0};

    for (size_t i = 0; i < windows.size(); ++i) {
        double entropy = shannon_entropy(windows[i]);
        std::cout << "Window " << i+1
                    << " - Entropy: " << entropy
                    << ", Market Volatility: " << market_vol[i] << std::endl;
    }
    
    return 0;
};
