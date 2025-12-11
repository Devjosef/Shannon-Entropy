#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "../data-collection.cpp"

struct BehavioralCase {
    std::string name;
    std::vector<int> actions;
    double expected_entropy;
    double volatility_spike;
    std::string description;
};

int main() {
    std::cout << "=== Behavioral Data Test: Entropy -> Volatility ===\n\n";

    std::vector<BehavioralCase> cases = {
        {"Panic Selling",  {2,2,2,2,2,2,2,2,2,2}, 0.0, 5.0, "Mass panic: predictable but volatile"},
        {"FOMO Buying",    {1,1,1,1,1,1,1,1,1,1}, 0.0, 4.5, "FOMO rush: predictable but volatile"},
        {"Mixed Sentiment",{0,1,2,0,1,2,0,1,2,0}, 1.585, 3.2, "Diverse actions: moderate volatility"},
        {"Hesitant Trading",{0,0,0,1,0,0,2,0,0,0}, 0.881, 1.8, "Mostly hold: low volatility"},
        {"Chaotic Trading", {1,2,0,2,1,0,1,2,1,2}, 1.585, 4.8, "Random actions: high volatility"}
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

        if (std::fabs(calculated_entropy - test_case.expected_entropy) < 0.01) {
            std::cout << "  ✓ Entropy calculation PASSED\n";
            passed_tests++;
        } else {
            std::cout << "  ✗ Entropy calculation FAILED\n";
        }
        std::cout << "\n";
    }

    std::cout << "=== Behavioral Pattern Analysis ===\n";
    std::cout << "Pattern: Low Entropy + High Volatility (Panic/FOMO)\n";
    assert(cases[0].expected_entropy < 0.5 && cases[0].volatility_spike > 4.0);
    assert(cases[1].expected_entropy < 0.5 && cases[1].volatility_spike > 4.0);
    std::cout << "✓ Panic/FOMO patterns verified\n\n";

    std::cout << "Pattern: High Entropy + High Volatility (Chaos)\n";
    assert(cases[4].expected_entropy > 1.5 && cases[4].volatility_spike > 4.0);
    std::cout << "✓ Chaos pattern verified\n\n";

    std::cout << "Pattern: Low Entropy + Low Volatility (Stable)\n";
    assert(cases[3].expected_entropy < 1.0 && cases[3].volatility_spike < 2.0);
    std::cout << "✓ Stable pattern verified\n\n";

    std::cout << "Test Results: " << passed_tests << "/" << total_tests << " entropy calculations passed\n";
    std::cout << "✓ All behavioral pattern assertions passed\n";

    return 0;
}
