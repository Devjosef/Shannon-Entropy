#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <map>
#include <cmath>
#include <random>

struct MarketWindow {
    int window_id;
    std::string timestamp;
    double h, l, o;
    double entropy;
    double volatility;
    std::string regime;
};

double shannon_entropy(const std::vector<double>& prices) {
    if (prices.empty()) return 0.0;
    std::map<double, int> freq;
    for (double p : prices) freq[p]++;
    
    double entropy = 0.0;
    double n = prices.size();
    for (const auto& p : freq) {
        double prob = p.second / n;
        entropy -= prob * log2(prob);
    }
    return entropy;
}

class MarketDataGenerator {
    std::mt19937 rng;
public:
    MarketDataGenerator(int seed = 42) : rng(seed) {}
    double generate_volatility(double base_vol, double entropy, double stress) {
        return base_vol + (entropy * 1.2) + (stress * 2.5);
    }
};

int main() {
    MarketDataGenerator gen(42);
    std::vector<MarketWindow> windows;
    
    struct Regime {
        double h, l, o;
    };
    
    Regime regimes[4] = {
        {677.52, 681.7, 681.27},
        {596.42, 600.44, 600.64}, 
        {255.45, 262.02, 261.73},
        {410.88, 414.315, 417.07}
    };
    
    std::string timestamps[] = {
        "15:30:58", "15:31:13", "15:31:29", "15:31:44", "15:32:29", "15:32:44", "15:33:00", "15:33:15",
        "17:30:12", "17:30:28", "17:30:43", "17:30:58", "17:31:44", "17:31:59", "17:32:14", "17:32:29",
        "18:29:29", "18:29:44", "18:29:59", "18:30:14", "18:31:00", "18:31:15", "18:31:30", "18:31:45",
        "21:00:03", "21:00:18", "21:00:33", "21:00:49", "21:01:34", "21:01:49", "21:02:04", "21:02:20"
    };
    
    int regime_idx = 0;
    for (int i = 1; i <= 60; ++i) {
        std::string ts = "2026-02-16 " + std::string(timestamps[(i-1) % 32]);
        Regime r = regimes[regime_idx % 4];
        
        std::vector<double> prices = {r.h, r.l, r.o};
        double entropy = shannon_entropy(prices);
        double stress = (regime_idx % 4 == 2) ? 0.8 : (regime_idx % 4 == 1) ? 0.4 : 0.1;
        double volatility = gen.generate_volatility(1.0, entropy, stress);
        
        windows.push_back({i, ts, r.h, r.l, r.o, entropy, volatility, "Regime" + std::to_string(regime_idx % 4 + 1)});
        regime_idx++;
    }
    
    std::ofstream csv("market_data.csv");
    csv << "c,d,dp,h,l,o,pc,t,symbol,timestamp\n";
    std::string symbols[] = {"SPY", "QQQ", "AAPL", "TSLA"};
    
    for (const auto& w : windows) {
        for (const auto& sym : symbols) {
            csv << w.timestamp << ",?,?," << std::fixed << std::setprecision(2) 
                << w.h << "," << w.l << "," << w.o << ",,0," << sym << ",\n";
        }
    }
    csv.close();
    
    std::ofstream analysis("visual_inspection_data.csv");
    analysis << "Window_ID,Timestamp,Entropy,Volatility,Regime\n";
    for (const auto& w : windows) {
        analysis << w.window_id << "," << w.timestamp << "," << std::fixed << std::setprecision(6) 
                 << w.entropy << "," << w.volatility << "," << w.regime << "\n";
    }
    analysis.close();
    
    return 0;
}
