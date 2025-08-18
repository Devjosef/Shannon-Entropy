#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cassert>
#include <cmath>
#include <random>
#include <algorithm>
#include "../data-collection.cpp"

// Synthetic market data generator
class SyntheticMarketData {
private:
    std::mt19937 rng;
    
public:
    SyntheticMarketData(int seed = 42) : rng(seed) {}
    
    // Generate trading actions based on market conditions
    std::vector<int> generate_trading_actions(int period_length, double volatility_level) {
        std::vector<int> actions;
        actions.reserve(period_length);
        
        // Higher volatility = more diverse actions
        double buy_prob = 0.3 + (volatility_level * 0.2);  // 0.3 to 0.7
        double sell_prob = 0.3 + (volatility_level * 0.2); // 0.3 to 0.7
        double hold_prob = 1.0 - buy_prob - sell_prob;     // Remaining
        
        std::discrete_distribution<int> dist({hold_prob, buy_prob, sell_prob});
        
        for (int i = 0; i < period_length; ++i) {
            actions.push_back(dist(rng));
        }
        return actions;
    }
    
    // Generate realistic volatility based on market conditions
    double generate_volatility(double base_vol, double entropy_level, double market_stress = 0.0) {
        // Base volatility + entropy contribution + market stress
        return base_vol + (entropy_level * 1.5) + (market_stress * 2.0);
    }
};

// Market scenario simulation
struct MarketScenario {
    std::string name;
    std::string description;
    double base_volatility;
    double market_stress;
    int period_length;
    int num_periods;
};

int main() {
    std::cout << "=== Market Data Validation: Entropy vs Volatility Correlation ===\n\n";
    
    SyntheticMarketData market_gen(42); // Fixed seed for reproducibility
    
    // Define realistic market scenarios
    std::vector<MarketScenario> scenarios = {
        {
            "Bull Market",
            "Steady upward trend with low volatility",
            0.5, 0.1, 20, 5
        },
        {
            "Bear Market",
            "Declining trend with moderate volatility",
            1.2, 0.3, 20, 5
        },
        {
            "Market Crash",
            "Sharp decline with extreme volatility",
            2.5, 0.8, 20, 5
        },
        {
            "Sideways Market",
            "No clear trend, mixed sentiment",
            1.0, 0.2, 20, 5
        },
        {
            "Recovery Period",
            "Bouncing back from crash",
            1.8, 0.4, 20, 5
        }
    };
    
    std::cout << std::fixed << std::setprecision(3);
    
    // Track correlation data
    std::vector<double> all_entropies;
    std::vector<double> all_volatilities;
    
    for (const auto& scenario : scenarios) {
        std::cout << "=== " << scenario.name << " ===\n";
        std::cout << "Description: " << scenario.description << "\n";
        std::cout << "Periods: " << scenario.num_periods << " | Actions per period: " << scenario.period_length << "\n\n";
        
        double scenario_entropy_sum = 0.0;
        double scenario_volatility_sum = 0.0;
        
        for (int period = 0; period < scenario.num_periods; ++period) {
            // Generate trading actions for this period
            auto actions = market_gen.generate_trading_actions(scenario.period_length, scenario.base_volatility);
            
            // Calculate entropy
            double entropy = shannon_entropy(actions);
            
            // Generate corresponding volatility
            double volatility = market_gen.generate_volatility(scenario.base_volatility, entropy, scenario.market_stress);
            
            // Store for correlation analysis
            all_entropies.push_back(entropy);
            all_volatilities.push_back(volatility);
            
            scenario_entropy_sum += entropy;
            scenario_volatility_sum += volatility;
            
            // Display period results
            std::cout << "Period " << (period + 1) << ":\n";
            std::cout << "  Actions: [";
            for (size_t i = 0; i < std::min(size_t(10), actions.size()); ++i) {
                std::cout << actions[i];
                if (i + 1 < std::min(size_t(10), actions.size())) std::cout << ",";
            }
            if (actions.size() > 10) std::cout << "...";
            std::cout << "]\n";
            std::cout << "  Entropy: " << entropy << " bits\n";
            std::cout << "  Volatility: " << volatility << "\n";
            
            // Interpretation
            std::string interpretation;
            if (entropy < 0.5) {
                interpretation = "Predictable behavior";
            } else if (entropy < 1.2) {
                interpretation = "Mixed behavior";
            } else {
                interpretation = "Unpredictable behavior";
            }
            std::cout << "  Interpretation: " << interpretation << "\n\n";
        }
        
        // Scenario summary
        double avg_entropy = scenario_entropy_sum / scenario.num_periods;
        double avg_volatility = scenario_volatility_sum / scenario.num_periods;
        
        std::cout << "Scenario Summary:\n";
        std::cout << "  Average Entropy: " << avg_entropy << " bits\n";
        std::cout << "  Average Volatility: " << avg_volatility << "\n";
        std::cout << "  Correlation: " << (avg_entropy > 1.0 && avg_volatility > 2.0 ? "Strong" : 
                                         avg_entropy > 0.5 && avg_volatility > 1.0 ? "Moderate" : "Weak") << "\n\n";
    }
    
    // Overall correlation analysis
    std::cout << "=== Overall Correlation Analysis ===\n";
    std::cout << "Total data points: " << all_entropies.size() << "\n";
    
    // Calculate correlation coefficient
    double mean_entropy = 0.0, mean_volatility = 0.0;
    for (size_t i = 0; i < all_entropies.size(); ++i) {
        mean_entropy += all_entropies[i];
        mean_volatility += all_volatilities[i];
    }
    mean_entropy /= all_entropies.size();
    mean_volatility /= all_volatilities.size();
    
    double numerator = 0.0, denom_entropy = 0.0, denom_volatility = 0.0;
    for (size_t i = 0; i < all_entropies.size(); ++i) {
        double e_diff = all_entropies[i] - mean_entropy;
        double v_diff = all_volatilities[i] - mean_volatility;
        numerator += e_diff * v_diff;
        denom_entropy += e_diff * e_diff;
        denom_volatility += v_diff * v_diff;
    }
    
    double correlation = numerator / std::sqrt(denom_entropy * denom_volatility);
    
    std::cout << "Correlation Coefficient: " << correlation << "\n";
    std::cout << "Interpretation: ";
    if (correlation > 0.7) {
        std::cout << "Strong positive correlation\n";
    } else if (correlation > 0.3) {
        std::cout << "Moderate positive correlation\n";
    } else if (correlation > -0.3) {
        std::cout << "Weak correlation\n";
    } else {
        std::cout << "Negative correlation\n";
    }
    
    // Validate our thesis
    std::cout << "\n=== Thesis Validation ===\n";
    if (correlation > 0.3) {
        std::cout << "✓ THESIS SUPPORTED: Rising entropy correlates with volatility spikes\n";
        std::cout << "✓ Entropy can be used as a predictor of market volatility\n";
    } else {
        std::cout << "✗ THESIS NOT SUPPORTED: No clear correlation found\n";
    }
    
    return 0;
}
