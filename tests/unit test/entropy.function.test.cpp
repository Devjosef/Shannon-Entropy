#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include <cmath>
#include "../../data-collection.cpp"

int main() {
    std::cout << "=== Shannon Entropy (Probability) Tests ===\n\n";

    // Controlled probability distributions
    std::vector<double> uniform = {0.25, 0.25, 0.25, 0.25}; // Maximum entropy (4 outcomes)
    std::vector<double> predictable = {1.0, 0.0, 0.0, 0.0}; // Minimum entropy
    std::vector<double> mixed = {0.5, 0.25, 0.125, 0.125};  // Intermediate entropy

    double H_uniform = shannon_entropy_from_probabilities(uniform);
    double H_predictable = shannon_entropy_from_probabilities(predictable);
    double H_mixed = shannon_entropy_from_probabilities(mixed);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Uniform (0.25,0.25,0.25,0.25): " << H_uniform << " bits\n";
    std::cout << "Predictable (1,0,0,0):        " << H_predictable << " bits\n";
    std::cout << "Mixed (0.5,0.25,0.125,0.125): " << H_mixed << " bits\n\n";

    // Exact expected values (within small tolerance)
    const double EPS = 1e-9;
    assert(std::fabs(H_uniform - 2.0) < EPS);     // 4 equally likely outcomes => 2 bits
    assert(std::fabs(H_predictable - 0.0) < EPS); // Fully predictable => 0 bits
    assert(std::fabs(H_mixed - 1.75) < EPS);      // Precomputed value

    // Ordering
    assert(H_uniform > H_mixed);
    assert(H_mixed > H_predictable);

    std::cout << "✓ Tests passed: uniform highest, predictable lowest, mixed between (exact values verified).\n";
    return 0;
}