std::vector<double> unifrom = {0.25, 0.25, 0.25, 0.25}; // Maximum entropy
std:vector<double> predictable = {1.0, 0.0, 0.0, 0.0}; // Minimum entropy
std::vector<double> mixed = {0.5, 0.25, 0.125, 0.125}; // Intermediate entropy

std::cout << "Uniform entropy: " << shannon_entropy(uniform) <<std::endl;
std::cout << "Predictable entropy: " << shannon_entropy(predictable)
<< std::endl;
std::cout << "Mixed entropy: " << shannon_entropy(mixed)  <<std::endl;