std::vector<std::vector<int>> windows = {
    {0, 0, 1, 2, 2, 0, 1},
    {0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 0, 1, 2, 0},
};

for (const auto& window : windows) {
    std::cout << "Window entropy: " << shannon_entropy(window) << std::endl;
}