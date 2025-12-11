#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>
#include "../data-collection.cpp"

struct MarketWindow {
    int window_id;
    std::vector<int> actions;
    double entropy;
    double volatility;
    std::string market_condition;
    std::string behavior_pattern;
};

class MarketDataGenerator {
private:
    std::mt19937 rng;
    
public:
    MarketDataGenerator(int seed = 42) : rng(seed) {}
    
    std::vector<int> generate_actions(int length, double predictability) {
        std::vector<int> actions; actions.reserve(length);
        double hold_bias = 0.5 + (predictability * 0.3);
        double buy_bias = (1.0 - hold_bias) * 0.6;
        double sell_bias = (1.0 - hold_bias) * 0.4;
        std::discrete_distribution<int> dist({hold_bias, buy_bias, sell_bias});
        for (int i = 0; i < length; ++i) {
            actions.push_back(dist(rng));
        }
        return actions;
    }
    
    double generate_volatility(double base_vol, double entropy, double market_stress) {
        return base_vol + (entropy * 1.2) + (market_stress * 2.5);
    }
};

int main() {
    std::cout << "=== Visual Inspection Test: 60 Market Windows ===\n\n";
    
    MarketDataGenerator generator(42);
    std::vector<MarketWindow> windows;
    
    // Bull Market (20 windows)
    for (int i = 1; i <= 20; ++i) {
        auto actions = generator.generate_actions(15, 0.3);
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(1.0, entropy, 0.1);
        windows.push_back({i, actions, entropy, volatility, "Bull Market", 
                          entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"});
    }
    
    // Bear Market (20 windows)
    for (int i = 21; i <= 40; ++i) {
        auto actions = generator.generate_actions(15, 0.6);
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(2.0, entropy, 0.4);
        windows.push_back({i, actions, entropy, volatility, "Bear Market", 
                          entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"});
    }
    
    // Market Crash (10 windows)
    for (int i = 41; i <= 50; ++i) {
        auto actions = generator.generate_actions(15, 0.8);
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(3.5, entropy, 0.8);
        windows.push_back({i, actions, entropy, volatility, "Market Crash", 
                          entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"});
    }
    
    // Recovery (10 windows)
    for (int i = 51; i <= 60; ++i) {
        auto actions = generator.generate_actions(15, 0.4);
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(2.5, entropy, 0.3);
        windows.push_back({i, actions, entropy, volatility, "Recovery", 
                          entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"});
    }
    
    // Export CSV for Python visualizer
    std::ofstream csv("tests/visual_inspection_data.csv");
    csv << "Window_ID,Entropy,Volatility,Market_Condition,Behavior_Pattern\n";
    for (const auto& w : windows) {
        csv << w.window_id << "," << std::fixed << std::setprecision(6) << w.entropy << ","
            << w.volatility << "," << w.market_condition << "," << w.behavior_pattern << "\n";
    }
    csv.close();
    
    std::cout << "✓ Generated " << windows.size() << " windows → tests/visual_inspection_data.csv\n";
    std::cout << "✓ Run: python visualize_entropy.py\n";
    
    return 0;
}
