// Acts as an example for time windows & trader actions e.g. hourly snapshots.
int main(
std::vector<std::vector<int>> windows= {
    {0, 0, 1, 2, 2, 0, 1} // Represents mixed behaviours
    {0, 0, 0, 0, 0, 0, 0} // Everyone buys, predictable
    {0, 1, 2, 0, 1, 2, 0} // Maximum unpredictability
};

// Market simulation: volatility index per window
std::vector<double> market_vol = {2.5, 0.2, 3.0};

// Print mapping results
for (size_t i = 0; i <windows.size(); ++i) {
    double entropy = shannon_entropy (windows[i]);
    std::cout << "Window " << i+1
    << " | Entropy: " << entropy
    << " | Market Volatility: " << market_vol[i] << std:endl;
}
return 0;
)