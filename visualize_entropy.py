import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os
import warnings
warnings.filterwarnings("ignore", category=RuntimeWarning)

def compute_rolling_entropy(prices, window=50, bins=4):
    def entropy_window(x):
        if len(x) < 2:
            return 0
        levels = pd.cut(x, bins=bins, labels=False)
        probs = pd.Series(levels).value_counts(normalize=True)
        return -sum(p * np.log2(p + 1e-10) for p in probs.values)
    return prices.rolling(window=window, min_periods=10).apply(entropy_window,raw=False)

def validate_and_visualize_stock_data(csv_file="spy_live_data_final.csv"):
    expected_file = "spy_live_data_final.csv"
    
    if csv_file != expected_file:
        print(f"ERROR: Only '{expected_file}' is accepted. Provided: '{csv_file}'")
        return
        
    if not os.path.exists(csv_file):
        print(f"ERROR: File '{csv_file}' not found at current location.")
        return
    
    try:
        df = pd.read_csv(csv_file, on_bad_lines='skip')
        print("Data loaded successfully:")
        print(df.head())
        print(f"Shape: {df.shape}")
        
        df.columns = ['timestamp', 'c', 'd', 'dp', 'h', 'l', 'o', 'pc', 't', 'symbol']
        
        numeric_cols = ['c', 'd', 'dp', 'h', 'l', 'o', 'pc', 't']
        for col in numeric_cols:
            df[col] = pd.to_numeric(df[col], errors='coerce')
        
        df['Price'] = df['h'].ffill()
        df['Window_ID'] = range(len(df))
        df['Volatility'] = df['Price'].rolling(window=10).std().fillna(0)
        
        df['Entropy'] = compute_rolling_entropy(df['Price'], window=100)
        df['Entropy'] = df['Entropy'].bfill().fillna(0)

        
        df['Market_Condition'] = np.where(df['Volatility'] > df['Volatility'].median(), 
                                        'High Volatility', 'Stable')
        
        print("\n Processed data preview:")
        print(df[['timestamp', 'symbol', 'Price', 'Volatility', 'Entropy', 'Market_Condition']][::50].head(10))
        print(f"Volatility range: {df['Volatility'].min():.3f} to {df['Volatility'].max():.3f}")
        
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 14))
        fig.suptitle('Live Data Analysis', fontsize=18, fontweight='bold')

        sns.scatterplot(data=df, x='Entropy', y='Volatility', ax=ax1, 
                       s=60, alpha=0.65, edgecolors='black', linewidth=0.8)
        corr = df['Entropy'].corr(df['Volatility'])
        ax1.set_title(f'Correlation: ρ={corr:.3f}', fontsize=14, fontweight='bold')
        ax1.grid(True, alpha=0.3)

        ax2.plot(df['Window_ID'], df['Entropy'], 'b-', linewidth=2, alpha=0.8)
        ax2.set_title('Entropy Over Time', fontsize=14, fontweight='bold')
        ax2.set_xlabel('Window ID')
        ax2.grid(True, alpha=0.3)

        ax3.plot(df['Window_ID'], df['Volatility'], 'r-', linewidth=2, alpha=0.8)
        ax3.set_title('Volatility Over Time', fontsize=14, fontweight='bold')
        ax3.set_xlabel('Window ID')
        ax3.grid(True, alpha=0.3)

        ax4.axis('off')
        summary = f"LIVE DATA SUMMARY\n{'='*30}\n"
        summary += f"Records: {len(df):,}\n"
        summary += f"Avg Price: ${df['Price'].mean():.2f}\n"
        summary += f"Avg Volatility: {df['Volatility'].mean():.3f}\n"
        summary += f"Entropy: {df['Entropy'].mean():.3f} bits\n"
        summary += f"Correlation: {corr:.3f}\n"
        summary += f"High Vol Periods: {len(df[df['Market_Condition']=='High Volatility'])}"
        
        ax4.text(0.05, 0.95, summary, fontsize=12, fontfamily='monospace',
                verticalalignment='top', transform=ax4.transAxes,
                bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray", alpha=0.8))
        
        plt.tight_layout()
        plt.savefig('spy_analysis.png', dpi=300, bbox_inches='tight', facecolor='white')
        plt.show()
        
        print(f"\nAnalysis complete! Correlation: {corr:.3f}")
        print("Chart saved as 'spy_analysis.png'")
        
    except Exception as e:
        print(f"Error processing SPY data: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    validate_and_visualize_stock_data()
