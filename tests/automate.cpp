#include <cassert>

assert(fabs(shannon_entropy(uniform) - 2.0) < 0.01); // 2 bits for uniform
// distribution of 4 elements
assert(fabs(shannon_entropy(predictable) - 0.0) < 0.01); // 0 bits for predictable
// fully predictable
assert(shannon_entropy(mixed) > 0.0); // Should be between 0 and 2 bits