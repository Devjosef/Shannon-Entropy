#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <random>
#include <algorithm>
#include "../data-collection.cpp"

struct RobustnessCase {
    std::string name;
    std::vector<int> actions;
    double expected_entropy;
    std::string expected_behavior;
};

int main() {
    std::cout << "=== Entropy Function Robustness Test ===\n\n";
    
    std::mt19937 rng(42);
    
    std::vector<RobustnessCase> cases = {
        {"All Identical Actions (Entropy=0)", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0.0, "Completely predictable behavior"},
        {"All Buy Actions", {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 0.0, "Mass buying behavior"},
        {"All Sell Actions", {2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 0.0, "Mass selling behavior"},
        {"Completely Random Actions (High Entropy)", {0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1}, 1.585, "Maximum unpredictability (uniform distribution)"},
        {"Empty Dataset", {}, 0.0, "No actions to analyze"},
        {"Single Action", {1}, 0.0, "Single action is completely predictable"},
        {"Two Identical Actions", {0, 0}, 0.0, "Two identical actions are predictable"},
        {"Two Different Actions", {0, 1}, 1.0, "Two different actions with equal probability"},
        {"Three Different Actions (Uniform)", {0, 1, 2}, 1.585, "Three actions with equal probability"},
        {"Skewed Distribution", {0, 0, 0, 0, 0, 1, 1, 2}, 1.299, "Skewed towards hold with some buy/sell"},
        {"Large Dataset - All Identical", std::vector<int>(100, 1), 0.0, "Large dataset with identical actions"},
        {"Large Dataset - Random", [&rng]() {
            std::vector<int> actions(100);
            std::uniform_int_distribution<int> dist(0, 2);
            for (int& action : actions) {
                action = dist(rng);
            }
            return actions;
        }(), 1.585, "Large random dataset (should approach theoretical max)"}
    };
    
    std::cout << std::fixed << std::setprecision(6);
    int passed_tests = 0;
    int total_tests = cases.size();
    
    for (const auto& test_case : cases) {
        std::cout << "Test: " << test_case.name << "\n";
        
        if (test_case.actions.empty()) {
            std::cout << "  Actions: [EMPTY]\n";
            std::cout << "  Expected entropy: " << test_case.expected_entropy << " bits\n";
            std::cout << "  Expected behavior: " << test_case.expected_behavior << "\n";
            
            try {
                double entropy = shannon_entropy(test_case.actions);
                std::cout << "  Calculated entropy: " << entropy << " bits\n";
                
                if (std::fabs(entropy - test_case.expected_entropy) < 0.001) {
                    std::cout << "  ✓ PASSED: Empty dataset handled gracefully\n";
                    passed_tests++;
                } else {
                    std::cout << "  ✗ FAILED: Empty dataset returned unexpected value\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ✗ FAILED: Empty dataset caused exception: " << e.what() << "\n";
            }
        } else {
            std::cout << "  Actions: [";
            size_t display_limit = std::min(size_t(20), test_case.actions.size());
            for (size_t i = 0; i < display_limit; ++i) {
                std::cout << test_case.actions[i];
                if (i + 1 < display_limit) std::cout << ",";
            }
            if (test_case.actions.size() > 20) {
                std::cout << "... (total: " << test_case.actions.size() << ")";
            }
            std::cout << "]\n";
            
            std::cout << "  Expected entropy: " << test_case.expected_entropy << " bits\n";
            std::cout << "  Expected behavior: " << test_case.expected_behavior << "\n";
            
            try {
                double entropy = shannon_entropy(test_case.actions);
                std::cout << "  Calculated entropy: " << entropy << " bits\n";
                
                const double EPS = 0.01;
                if (std::fabs(entropy - test_case.expected_entropy) < EPS) {
                    std::cout << "  ✓ PASSED: Entropy calculation correct\n";
                    passed_tests++;
                } else {
                    std::cout << "  ✗ FAILED: Entropy calculation incorrect\n";
                    std::cout << "    Difference: " << std::fabs(entropy - test_case.expected_entropy) << "\n";
                }
                
                if (test_case.name.find("Identical") != std::string::npos) {
                    if (entropy == 0.0) {
                        std::cout << "  ✓ PASSED: Identical actions correctly yield zero entropy\n";
                    } else {
                        std::cout << "  ✗ FAILED: Identical actions should yield zero entropy\n";
                    }
                }
                
                if (test_case.name.find("Random") != std::string::npos) {
                    if (entropy > 1.5) {
                        std::cout << "  ✓ PASSED: Random actions yield high entropy\n";
                    } else {
                        std::cout << "  ✗ FAILED: Random actions should yield high entropy\n";
                    }
                }
                
            } catch (const std::exception& e) {
                std::cout << "  ✗ FAILED: Exception occurred: " << e.what() << "\n";
            }
        }
        std::cout << "\n";
    }
    
    std::cout << "=== Boundary Condition Tests ===\n";
    
    std::vector<int> large_numbers = {1000, 1000, 2000, 2000, 3000, 3000};
    try {
        double entropy = shannon_entropy(large_numbers);
        std::cout << "Large numbers test: " << entropy << " bits ✓\n";
        passed_tests++;
    } catch (const std::exception& e) {
        std::cout << "Large numbers test failed: " << e.what() << " ✗\n";
    }
    
    std::vector<int> negative_numbers = {-1, -1, -2, -2, -3, -3};
    try {
        double entropy = shannon_entropy(negative_numbers);
        std::cout << "Negative numbers test: " << entropy << " bits ✓\n";
        passed_tests++;
    } catch (const std::exception& e) {
        std::cout << "Negative numbers test failed: " << e.what() << " ✗\n";
    }
    
    std::vector<int> mixed_numbers = {0, -1, 1, -2, 2, -3, 3};
    try {
        double entropy = shannon_entropy(mixed_numbers);
        std::cout << "Mixed numbers test: " << entropy << " bits ✓\n";
        passed_tests++;
    } catch (const std::exception& e) {
        std::cout << "Mixed numbers test failed: " << e.what() << " ✗\n";
    }
    
    std::cout << "\n=== Robustness Test Summary ===\n";
    std::cout << "Tests passed: " << passed_tests << "/" << (total_tests + 3) << "\n";
    
    if (passed_tests == total_tests + 3) {
        std::cout << "✓ ALL ROBUSTNESS TESTS PASSED\n";
        std::cout << "✓ Entropy function handles all edge cases gracefully\n";
        std::cout << "✓ Function is robust and production-ready\n";
    } else {
        std::cout << "✗ SOME TESTS FAILED - Function needs improvement\n";
    }
    
    return 0;
}
