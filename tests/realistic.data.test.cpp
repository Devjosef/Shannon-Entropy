#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include "../data-collection.cpp"

// Helper to stringify actions
static std::string actions_to_string(const std::vector<int>& actions) {
    std::string s = "[";
    for (size_t i = 0; i < actions.size(); ++i) {
        s += std::to_string(actions[i]);
        if (i + 1 < actions.size()) s += ",";
    }
    s += "]";
    return s;
}

int main() {
    // Simulated sequential trading actions across 3 time windows
    // 0 = hold, 1 = buy, 2 = sell
    std::vector<std::vector<int>> windows = {
        {0, 0, 1, 2, 2, 0, 1},             // Mixed (counts ~ 3,2,2)
        {0, 0, 0, 0, 0, 0, 0},             // Predictable (all same)
        {0, 1, 2, 0, 1, 2, 0, 1, 2}        // More uniform (3,3,3) -> higher entropy
    };

    // Simulated market volatility for each window (e.g., realized volatility index)
    std::vector<double> market_vol = {2.5, 0.2, 3.0};

    std::cout << "=== Realistic Data Test: Entropy vs Market Volatility ===\n\n";
    std::cout << std::fixed << std::setprecision(3);

    std::vector<double> entropies;
    entropies.reserve(windows.size());

    for (size_t i = 0; i < windows.size(); ++i) {
        double H = shannon_entropy(windows[i]);
        entropies.push_back(H);

        // Interpretation bucket
        std::string interpretation = (H < 0.5) ? "Predictable, Calm" : (H < 1.5) ? "Mixed, Moderate" : "Unpredictable, Spiking";

        std::cout << "Window " << (i + 1) << ": actions=" << actions_to_string(windows[i])
                  << " | entropy=" << H << " bits"
                  << " | volatility=" << market_vol[i]
                  << " | " << interpretation << "\n";
    }

    std::cout << "\nExpected ordering checks (higher entropy -> higher volatility):\n";
    // With our chosen data: window2 should be lowest, window3 highest, window1 in the middle
    assert(entropies[1] < entropies[0] && entropies[0] < entropies[2]);
    assert(market_vol[1] < market_vol[0] && market_vol[0] < market_vol[2]);
    std::cout << "✓ Ordering verified: W2 < W1 < W3 for both entropy and volatility.\n";

    // Simple positive rank correlation check via pairwise comparisons
    int concordant = 0, total_pairs = 0;
    for (size_t i = 0; i < entropies.size(); ++i) {
        for (size_t j = i + 1; j < entropies.size(); ++j) {
            // Treat ties in entropy as neutral (do not count)
            if (std::fabs(entropies[i] - entropies[j]) < 1e-12) {
                continue;
            }
            const bool e_order = entropies[i] < entropies[j];
            const bool v_order = market_vol[i] < market_vol[j];
            if (e_order == v_order) {
                concordant += 1;
            } else {
                // If you want to allow some noise, relax this; for now require perfect rank match
                std::cerr << "Discordant pair at (" << i << "," << j << ")\n";
                assert(false);
            }
            total_pairs += 1;
        }
    }
    std::cout << "Concordant pairs: " << concordant << "/" << total_pairs << " (higher is better)\n";
    assert(concordant == total_pairs); // all non-tied pairwise orders match
    std::cout << "✓ All pairwise ordering between entropy and volatility match (perfect positive rank correlation).\n";

    return 0;
}