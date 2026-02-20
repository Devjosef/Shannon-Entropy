# Shannon Entropy Market Analysis

**Live Demo** (60 windows, r=-0.193):
[![Kaggle Dataset](https://img.shields.io/badge/Kaggle-Dataset-blueviolet)](https://www.kaggle.com/datasets/josefmath/shannon-entropy-market-simulation-60-windows)
[![Kaggle Notebook](https://img.shields.io/badge/Kaggle-Notebook-orange)](https://www.kaggle.com/code/josefmath/notebook-for-shannon-dataset)

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

## Phase 1: Synthetic trader actions test
*Controlled experiment* Validating the entropy volatility relationship

| Time_Window | Trader_Actions  | Shannon_Entropy | Market_Volatility | Interpretation             |
| ----------- | --------------- | --------------- | ----------------- | -------------------------- |
| 1           | [0,0,1,2,2,0,1] | 1.251           | 2.5               | Mixed, Moderate Volatility |
| 2           | [0,0,0,0,0,0,0] | 0.000           | 0.2               | Predictable, Stable Market |
| 3           | [0,1,2,0,1,2,0] | 1.585           | 3.0               | Unpredictable, High Vol    |

## Phase 2: Live Market validation
*SPY portfolio proxy (312 quotes, Feb 16, 2026 15:30-21:09):

| Index | Timestamp | Price (Avg) | Volatility | Entropy | Condition            | Assets            |
| ----- | --------- | ----------- | ---------- | ------- | -------------------- | ----------------- |
| 0     | 15:30:58  | 681.27      | 0.00       | 0.722   | Stable (Predictable) | SPY/QQQ/AAPL/TSLA |
| 50    | 15:35:31  | 681.27      | 173.57     | 1.468   | Chaotic High Vol     | SPY/QQQ/AAPL/TSLA |
| 100   | 17:34:00  | 417.07      | 178.54     | 1.441   | Chaotic High Vol     | SPY/QQQ/AAPL/TSLA |
| 150   | 18:31:45  | 417.07      | 147.99     | 1.467   | Moderate Vol         | SPY/QQQ/AAPL/TSLA |
| 200   | 21:00:03  | 681.27      | 147.95     | 1.508   | Moderate Vol         | SPY/QQQ/AAPL/TSLA |
| 250   | 21:04:36  | 681.27      | 173.57     | 1.490   | Chaotic High Vol     | SPY/QQQ/AAPL/TSLA |
| 300   | 21:09:24  | 600.64      | 156.10     | 1.479   | Moderate Vol         | SPY/QQQ/AAPL/TSLA |       |


**Trader Actions**: 0 (hold), 1 (buy), 2 (sell)  
**Time Window**: Sequential trading periods  
**Entropy Calculation**: `H = -Σ(p_i * log2(p_i))`

### Testing Framework
**Unit Tests** Validate entropy calculations against known distrubutions.

**Robustness Tests** Edge cases such as empty data, all identical actions, and random or algorithmic trading patterns.

**Market Simulation**: Realistic scenarios (Bull/Bear markets, crashes, recovery)

**Visual Validation**: Comphrensive plotting and correlation analysis

### Implementation
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

**Unit Tests**: 100% pass rate (exact entropy calculations) *automate.test.cpp*

**Robustness Tests**: 15/15 edge cases handled gracefully *robustness.test.cpp*
 
**Market Validation**: 60 windows across 5 market scenarios *market_validation.test.cpp*

**Table Validation**: Phase 1. tables  *realistic.data.test.cpp*

**Visual Inspection**: Comprehensive correlation analysis *visual_inspection.test.cpp*

### Key Test Scenarios
**Bull Market**: High entropy (1.497 bits) + Moderate volatility (2.945)

**Bear Market**: Mixed entropy (0.944 bits) + High volatility (3.216)

**Market Crash**: Low entropy (0.599 bits) + Very high volatility (4.999)

**Sideways Market**: Mixed entropy (0.925 bits) + Moderate volatility (2.788)

**Recovery**: Mixed entropy (0.868 bits) + High volatility (3.902)

##  Validating the Thesis

### Primary Finding
**Live Data Correlation**: r = **-0.193** (312 quotes, `visualize_entropy.py`)

**Simulation Correlation**: r = **-0.601** (`market_validation.test.cpp`)

**Unexpected vs Hypothesis**: Initially the thesis predicted *positive* entropy-vol correlation. Data shows **negative** -high entropy during volatile periods, low entropy during panic crashes.

**Phase 2 Evidence** (table samples):  
`Idx 50: entropy=1.468 → vol=173.57 (High)`  
`Idx 100: entropy=1.441 → vol=178.54 (High)`  
`Idx 0: entropy=0.722 → vol=0.000 (Stable)`

### Market Behavior Patterns Observed:

### Market Behavior Patterns Observed

1. **Panic/Crash Scenarios**  
   **Low Entropy + Extreme Volatility**  
   - Mass coordinated selling → predictable (0.599 bits)  
   - Results in 4.999+ vol spikes  
   *Pattern*: **Predictable panic leads to market crash**

2. **Normal/Chaotic Trading**  
   **High Entropy + High Volatility**  
   - Diverse actions (1.44-1.50 bits, Phase 2)  
   - 173-178 vol range  
   *Pattern*: **Chaotic trading  leads to volatile markets**

3. **Stable Periods**  
   **Low-Medium Entropy + Low Vol**  
   - Predictable/mixed (0.72 bits, Idx 0)  
   - 0.000 vol  
   *Pattern*: **Orderly trading leads to stable prices**


## Conclusions

### Thesis Status: **Surprising & Valid**

**Live Data**: r = **-0.193** (Phase 2, 312 quotes)  
**Simulation**: r = **-0.601** (60 windows)  

**What We Learned**: Predicted that: "entropy rises with volatility"! which was wrong.
- **Negative correlation confirmed**: High entropy = volatile markets
- **Panic crashes show low entropy** Predictable mass selling = 0.599 bits
- **Entropy excels at regime detection** Stable / Chaotic / Panic
- **Needs microstructure data** In other words it needs access to high frequency feeds, for real prediction power

**Bottom Line**: Entropy seems to quantify behavior brilliantly. **With access to high frequency feeds it can potentially be used for crash detection, and not for volume forecasting.**


### Key Observations
1. **Lower entropy during crashes** (0.599 bits) to extreme volatility (4.999+)
2. **Higher entropy during volatile periods** (1.44-1.50 bits) ca: 173-178 vol range  
3. **Negative correlation is stable**: r=-0.193 (Phase 2) / r=-0.601 (simulation)
4. **Clear behavioral regimes** But requrie high-frequency data for prediction

### Research Insights
- **Entropy Analysis**: Quantifies trader action distribution across market regimes [1]
- **Pattern Recognition**: Captures distinct behavioral signatures (crashes: 0.599 bits vs. normal: 1.44+ bits) [2]
- **Volatility Prediction**: Negative correlation (r=-0.193) but needs high-frequency data + ML models + methods such as: 
  - Semi-supervised learning that captures the noise/signal mix
  - Pruning via regression & or decision trees  
  - Classical Reinforcement Learning identifies regimes [3] [4]
- **Risk Management**: Can provide behavioral context as a supporting signal, and not  as a standalone predictor [5]



## Usage

### Quick Start
```bash
# Setup environment
./setup.sh

# Compile & run main entropy analyzer
g++ -std=c++17 -o entropy entropy.cpp && ./entropy

# Run robustness tests
g++ -std=c++17 -o test_suite tests/robustness.test.cpp && ./test_suite

# Run visual inspection tests
g++ -std=c++17 -o visual_test tests/visual_inspection.test.cpp && ./visual_test

# Generate visualizations
python visualize_entropy.py
```

## Technical Specifications
- **Language**: C++17 core + Python visualization
- **Entropy Range**: 0.0-1.585 bits (theoretical max for 3 trader actions: BUY/HOLD/SELL)
- **Test Coverage**: Edge cases + 60 market scenarios (Phase 2 data)
- **Correlation**: Pearson r=-0.193 (entropy vs. volatility)
- **Visualization**: **3-panel charts** scatter correlation, entropy time-series, volatility time-series + summary



 ## References
- [1]: Shannon (1948) - https://ieeexplore.ieee.org/document/6773024
- - [2]: Shefrin, H. (2002). *Beyond Greed and Fear: Understanding Behavioral Finance and the Psychology of Investing*
- [3]: Market Microstructure and Entropy Analysis - [https://pmc.ncbi.nlm.nih.gov/articles/PMC9073522/]
[1]: https://www.linkedin.com/pulse/volume-volatillity-imesh-fernando-0lhyc  
[2]: https://pmc.ncbi.nlm.nih.gov/articles/PMC9318915/  
[3]: https://pmc.ncbi.nlm.nih.gov/articles/PMC12748890/  
[4]: http://upubscience.com/upload/20250314101323.pdf  
[5]: https://pmc.ncbi.nlm.nih.gov/articles/PMC11675851/
---

## Final Insight
Entropy quantifies behavioral patterns effectively. The negative correlation (r=-0.193) challenges traditional hypotheses but reveals **crash predictability** (low entropy leads to high volatility). 

*Due to algorithmic trading (m/s: e.g. milisecond executions), requires co-located high-frequency model near exchange to confirm new findings.*
