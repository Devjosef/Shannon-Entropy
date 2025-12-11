# Shannon Entropy Market Analysis

## Purpose & Goal

**Purpose**: Apply Shannon entropy from information theory to analyze trader behavior patterns and their correlation with market volatility.

**Goal**: Develop a mathematical framework for predicting market volatility through behavioral complexity analysis.

# Theory & Approach

### Shannon Entropy in market analysis
Shannon entropy quantifies how unpredictable trader actions are.

**Low Entropy (0–0.5 bits)**: Predictable behavior (mass buying or selling)  
**Medium Entropy (0.5–1.2 bits)**: Mixed behavior patterns  
**High Entropy (1.2+ bits)**: Unpredictable, chaotic behavior

### Core Hypothesis
**Thesis**: Trader behavior entropy correlates with market volatility and may provide early warning signals of market stress.

## Methodology

### Data Collection
**Trader Actions**: 0 (hold), 1 (buy), 2 (sell)  
**Time Window**: Sequential trading periods  
**Entropy Calculation**: `H = -Σ(p_i * log2(p_i))`

### Testing Framework
**Unit Tests** Validate entropy calculations against known distrubutions.

**Robustness Tests** Edge cases such as empty data, all identical actions, and random or algorithmic trading patterns.

**Market Simulation**: Realistic scenarios (Bull/Bear markets, crashes, recovery)

**Visual Validation**: Comphrensive plotting and correlation analysis

### Implementation
# NOTE: Measures how "surprised" by trader decisions. All buys = 0 bits (predictable). Equal mix = max bits (chaotic)
```cpp
double shannon_entropy(std::vector<int> actions) {
    std::map<int, int> counts;
    int total = actions.size();
    for (int action : actions) {
        counts[action]++;
    }
    double entropy = 0.0;
    for (auto& pair : counts) {
        double p = (double)pair.second / total;
        if (p > 0) entropy -= p * std::log2(p);
    }
    return entropy;
}
```

## Testing & Validation

### Results

**Unit Tests**: 100% pass rate (exact entropy calculations)

**Robustness Tests**: 15/15 edge cases handled gracefully
 
**Market Validation**: 60 windows across 5 market scenarios

**Visual Inspection**: Comprehensive correlation analysis

### Key Test Scenarios
**Bull Market**: High entropy (1.497 bits) + Moderate volatility (2.945)

**Bear Market**: Mixed entropy (0.944 bits) + High volatility (3.216)

**Market Crash**: Low entropy (0.599 bits) + Very high volatility (4.999)

**Sideways Market**: Mixed entropy (0.925 bits) + Moderate volatility (2.788)

**Recovery**: Mixed entropy (0.868 bits) + High volatility (3.902)

##  Validating the Thesis

### Primary Finding: The Entropy-Volatility Relationship

**Correlation Coefficient: -0.601 (Moderate negative)**

This reveals an **unexpected pattern** that differs from the initial hypothesis:

### Market Behavior Patterns Observed:

1. **Market Crashes** -> **Low Entropy + High Volatility**
   - Mass panic creates predictable behavior (low entropy)
   - But results in extreme volatility spikes
   - **Pattern**: Predictable panic → High volatility

2. **Normal Trading** -> **High Entropy + Moderate Volatility**
   - Diverse trader actions (high entropy)
   - Results in moderate volatility
   - **Pattern**: High entropy behavior → Moderate volatility

3. **Market Stress** -> **Mixed Patterns**
   - Varying entropy levels
   - Consistently high volatility
   - **Pattern**: Mixed behavior -> High volatility

## Conclusions

### Thesis Status: Not Supported by Data

The relationship between entropy and volatility differs from the initial hypothesis:

- **Negative correlation (-0.601)** indicates entropy and volatility move in opposite directions
- **Entropy does differentiate market conditions** but not in the predicted direction
- **Traditional volatility measures may capture dynamics** that entropy-based analysis alone cannot fully explain
- **Behavioral complexity alone is insufficient** for volatility prediction without additional factors

### Key Observations:
1. **Low entropy during crashes** does occur with high volatility
2. **High entropy during normal periods** occurs with moderate volatility
3. **The correlation is negative and stable** across larger sample sizes
4. **Behavioral patterns exist** but predictive power requires further investigation

### Research Insights:
- **Entropy Analysis**: Successfully quantifies trader action distribution
- **Pattern Recognition**: Captures behavioral differences across market conditions
- **Volatility Prediction**: Requires additional models beyond entropy correlation
- **Risk Management**: Entropy provides supporting context but not standalone signals

## Usage

### Quick Start
```bash
# Setup environment
./setup.sh

g++ -std=c++17 -o entropy entropy.cpp && ./entropy

g++ -std=c++17 -o test_suite tests/robustness_test.cpp && ./test_suite

g++ -std=c++17 -o visual_test tests/visual_inspection_test.cpp && ./visual_test

python visualize_entropy.py

```
### Files Structure
```
Shannon/
├── data-collection.cpp          # Core entropy function
├── tests/                       # Comprehensive test suite
│   ├── robustness_test.cpp     # Edge case testing
│   ├── visual_inspection_test.cpp # Market simulation
│   └── unit test/              # Unit tests
├── visualize_entropy.py        # Python visualization
├── requirements.txt            # Python dependencies
└── setup.sh                   # Environment setup
```

## Technical Specifications
- **Language**: C++17 with Python visualization
- **Entropy Range**: 0.0 to 1.585 bits (theoretical max for 3 actions)
- **Test Coverage**: 100% edge cases, 60 market scenarios
- **Correlation Analysis**: Pearson coefficient
- **Visualization**: 4-panel comprehensive charts


 ## References
- Shannon, C.E. (1948). "A Mathematical Theory of Communication"
- Information Theory in Behavioral Finance
- Market Microstructure and Entropy Analysis

---

Final Insight: Entropy quantifies behavior patterns effectively. Negative correlation challenges the hypothesis but reveals crash predictability (low entropy -> high vol). Needs real market data for practical use.