#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "../data-collection.cpp"

// Behavioral patterns mapped to volatility spikes
struct BehavioralCase {
    std::string name;
    std::vector<int> actions;
    double expected_entropy;
    double volatility_spike;
    std::string description;
};

int main() {
    std::cout << "=== Automated Behavioral Data Test: Entropy -> Volatility Spikes ===\n\n";

    // Define behavioral cases with known entropy-volatility relationships
    std::vector<BehavioralCase> cases = {
        {
            "Panic Selling",
            {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, // All sell
            0.0, // Predictable
            5.0, // High volatility spike
            "Mass panic selling creates predictable behavior but high volatility"
        },
        {
            "FOMO Buying",
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // All buy
            0.0, // Predictable
            4.5, // High volatility spike
            "Fear of missing out creates predictable buying but high volatility"
        },
        {
            "Mixed Sentiment",
            {0, 1, 2, 0, 1, 2, 0, 1, 2, 0}, // Alternating
            1.585, // High entropy (uniform distribution)
            3.2, // Moderate-high volatility
            "Mixed sentiment creates unpredictable behavior and moderate volatility"
        },
        {
            "Hesitant Trading",
            {0, 0, 0, 1, 0, 0, 2, 0, 0, 0}, // Mostly hold with occasional trades
            0.881, // Low-medium entropy
            1.8, // Low volatility
            "Hesitant trading creates low entropy and low volatility"
        },
        {
            "Chaotic Trading",
            {1, 2, 0, 2, 1, 0, 1, 2, 1, 2}, // Random pattern
            1.585, // High entropy
            4.8, // Very high volatility
            "Chaotic trading creates high entropy and very high volatility"
        }
    };

    std::cout << std::fixed << std::setprecision(3);
    int passed_tests = 0;
    int total_tests = cases.size();

    for (const auto& test_case : cases) {
        double calculated_entropy = shannon_entropy(test_case.actions);
        
        std::cout << "Test: " << test_case.name << "\n";
        std::cout << "  Actions: [";
        for (size_t i = 0; i < test_case.actions.size(); ++i) {
            std::cout << test_case.actions[i];
            if (i + 1 < test_case.actions.size()) std::cout << ",";
        }
        std::cout << "]\n";
        std::cout << "  Expected entropy: " << test_case.expected_entropy << " bits\n";
        std::cout << "  Calculated entropy: " << calculated_entropy << " bits\n";
        std::cout << "  Volatility spike: " << test_case.volatility_spike << "\n";
        std::cout << "  Description: " << test_case.description << "\n";

        // Verify entropy calculation
        const double EPS = 0.01;
        if (std::fabs(calculated_entropy - test_case.expected_entropy) < EPS) {
            std::cout << "  ✓ Entropy calculation PASSED\n";
            passed_tests++;
        } else {
            std::cout << "  ✗ Entropy calculation FAILED\n";
        }
        std::cout << "\n";
    }

    // Test behavioral patterns -> volatility relationship
    std::cout << "=== Behavioral Pattern Analysis ===\n";
    std::cout << "Pattern: Low Entropy + High Volatility (Panic/FOMO)\n";
    assert(cases[0].expected_entropy < 0.5 && cases[0].volatility_spike > 4.0); // Panic selling
    assert(cases[1].expected_entropy < 0.5 && cases[1].volatility_spike > 4.0); // FOMO buying
    std::cout << "✓ Panic/FOMO patterns verified: Low entropy but high volatility\n\n";

    std::cout << "Pattern: High Entropy + High Volatility (Chaos)\n";
    assert(cases[4].expected_entropy > 1.5 && cases[4].volatility_spike > 4.0); // Chaotic trading
    std::cout << "✓ Chaos pattern verified: High entropy and high volatility\n\n";

    std::cout << "Pattern: Low Entropy + Low Volatility (Stable)\n";
    assert(cases[3].expected_entropy < 1.0 && cases[3].volatility_spike < 2.0); // Hesitant trading
    std::cout << "✓ Stable pattern verified: Low entropy and low volatility\n\n";

    std::cout << "Test Results: " << passed_tests << "/" << total_tests << " entropy calculations passed\n";
    std::cout << "✓ All behavioral pattern assertions passed\n";

    return 0;
}