#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <random>
#include <chrono>
#include "../data-collection.cpp"

// Market scenario for visual inspection
struct MarketWindow {
    int window_id;
    std::vector<int> actions;
    double entropy;
    double volatility;
    std::string market_condition;
    std::string behavior_pattern;
};

// Generate realistic market data for visual inspection
class MarketDataGenerator {
private:
    std::mt19937 rng;
    
public:
    MarketDataGenerator(int seed = 42) : rng(seed) {}
    
    std::vector<int> generate_actions(int length, double predictability) {
        std::vector<int> actions;
        actions.reserve(length);
        
        // predictability: 0 = random, 1 = predictable
        double hold_bias = 0.5 + (predictability * 0.3);  // 0.5 to 0.8
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
    std::cout << "=== Visual Inspection Test: Entropy vs Volatility ===\n\n";
    
    MarketDataGenerator generator(42);
    
    // Generate comprehensive market data for visual inspection
    std::vector<MarketWindow> windows;
    
    // Scenario 1: Bull Market (20 windows)
    for (int i = 1; i <= 20; ++i) {
        auto actions = generator.generate_actions(15, 0.3); // Low predictability
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(1.0, entropy, 0.1);
        
        windows.push_back({
            i,
            actions,
            entropy,
            volatility,
            "Bull Market",
            entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"
        });
    }
    
    // Scenario 2: Bear Market (20 windows)
    for (int i = 21; i <= 40; ++i) {
        auto actions = generator.generate_actions(15, 0.6); // Medium predictability
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(2.0, entropy, 0.4);
        
        windows.push_back({
            i,
            actions,
            entropy,
            volatility,
            "Bear Market",
            entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"
        });
    }
    
    // Scenario 3: Market Crash (10 windows)
    for (int i = 41; i <= 50; ++i) {
        auto actions = generator.generate_actions(15, 0.8); // High predictability (panic)
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(3.5, entropy, 0.8);
        
        windows.push_back({
            i,
            actions,
            entropy,
            volatility,
            "Market Crash",
            entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"
        });
    }
    
    // Scenario 4: Recovery (10 windows)
    for (int i = 51; i <= 60; ++i) {
        auto actions = generator.generate_actions(15, 0.4); // Medium-low predictability
        double entropy = shannon_entropy(actions);
        double volatility = generator.generate_volatility(2.5, entropy, 0.3);
        
        windows.push_back({
            i,
            actions,
            entropy,
            volatility,
            "Recovery",
            entropy > 1.2 ? "Unpredictable" : entropy > 0.5 ? "Mixed" : "Predictable"
        });
    }
    
    // Output to console
    std::cout << "Generated " << windows.size() << " market windows for visual inspection\n\n";
    std::cout << std::fixed << std::setprecision(3);
    
    std::cout << "Sample Data (first 10 windows):\n";
    std::cout << "Window | Entropy | Volatility | Market Condition | Behavior\n";
    std::cout << "-------|---------|------------|------------------|----------\n";
    
    for (size_t i = 0; i < std::min(size_t(10), windows.size()); ++i) {
        const auto& w = windows[i];
        std::cout << std::setw(6) << w.window_id << " | "
                  << std::setw(7) << w.entropy << " | "
                  << std::setw(10) << w.volatility << " | "
                  << std::setw(16) << w.market_condition << " | "
                  << w.behavior_pattern << "\n";
    }
    
    // Output to CSV for plotting
    std::ofstream csv_file("tests/visual_inspection_data.csv");
    csv_file << "Window_ID,Entropy,Volatility,Market_Condition,Behavior_Pattern\n";
    
    for (const auto& w : windows) {
        csv_file << w.window_id << ","
                 << std::fixed << std::setprecision(6) << w.entropy << ","
                 << w.volatility << ","
                 << w.market_condition << ","
                 << w.behavior_pattern << "\n";
    }
    csv_file.close();
    
    // Calculate correlation for validation
    double sum_entropy = 0.0, sum_volatility = 0.0;
    for (const auto& w : windows) {
        sum_entropy += w.entropy;
        sum_volatility += w.volatility;
    }
    double mean_entropy = sum_entropy / windows.size();
    double mean_volatility = sum_volatility / windows.size();
    
    double numerator = 0.0, denom_entropy = 0.0, denom_volatility = 0.0;
    for (const auto& w : windows) {
        double e_diff = w.entropy - mean_entropy;
        double v_diff = w.volatility - mean_volatility;
        numerator += e_diff * v_diff;
        denom_entropy += e_diff * e_diff;
        denom_volatility += v_diff * v_diff;
    }
    
    double correlation = numerator / std::sqrt(denom_entropy * denom_volatility);
    
    std::cout << "\n=== Statistical Summary ===\n";
    std::cout << "Total windows: " << windows.size() << "\n";
    std::cout << "Mean entropy: " << mean_entropy << " bits\n";
    std::cout << "Mean volatility: " << mean_volatility << "\n";
    std::cout << "Correlation coefficient: " << correlation << "\n";
    
    std::cout << "\n=== Market Condition Analysis ===\n";
    std::map<std::string, std::vector<double>> condition_entropies;
    std::map<std::string, std::vector<double>> condition_volatilities;
    
    for (const auto& w : windows) {
        condition_entropies[w.market_condition].push_back(w.entropy);
        condition_volatilities[w.market_condition].push_back(w.volatility);
    }
    
    for (const auto& [condition, entropies] : condition_entropies) {
        double avg_entropy = 0.0, avg_volatility = 0.0;
        for (double e : entropies) avg_entropy += e;
        for (double v : condition_volatilities[condition]) avg_volatility += v;
        avg_entropy /= entropies.size();
        avg_volatility /= entropies.size();
        
        std::cout << condition << ":\n";
        std::cout << "  Windows: " << entropies.size() << "\n";
        std::cout << "  Avg Entropy: " << avg_entropy << " bits\n";
        std::cout << "  Avg Volatility: " << avg_volatility << "\n";
        std::cout << "  Pattern: " << (avg_entropy > 1.0 ? "High entropy" : "Low entropy") 
                  << " + " << (avg_volatility > 2.5 ? "High volatility" : "Low volatility") << "\n\n";
    }
    
    std::cout << "✓ Data exported to: tests/visual_inspection_data.csv\n";
    std::cout << "✓ Ready for plotting with Python/Excel\n";
    std::cout << "✓ Use visualize_entropy.py to create charts\n";
    
    return 0;
}
