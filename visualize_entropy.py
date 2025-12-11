import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

def visualize_entropy_volatility(csv_file="tests/visual_inspection_data.csv"):
    try:
        df = pd.read_csv(csv_file)
        print("Data loaded successfully:")
        print(df.head())
        
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle('Shannon Entropy vs Market Volatility Analysis', fontsize=16, fontweight='bold')
        
        colors = {'Bull Market': 'green', 'Bear Market': 'orange', 'Market Crash': 'red', 'Recovery': 'blue'}
        
        # Fix column names to match C++ output
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            ax1.scatter(subset['Entropy'], subset['Volatility'], 
                       s=80, alpha=0.7, c=colors.get(condition, 'gray'), 
                       label=condition, edgecolors='black')
        
        ax1.set_xlabel('Shannon Entropy (bits)')
        ax1.set_ylabel('Market Volatility')
        ax1.legend()
        
        z = np.polyfit(df['Entropy'], df['Volatility'], 1)
        p = np.poly1d(z)
        ax1.plot(df['Entropy'], p(df['Entropy']), "k--", alpha=0.8, linewidth=2)
        
        # Time series (rest unchanged)
        ax2.plot(df['Window_ID'], df['Entropy'], 'b-', linewidth=2)
        ax2.set_title('Entropy Over Time')
        
        ax3.plot(df['Window_ID'], df['Volatility'], 'r-', linewidth=2)
        ax3.set_title('Volatility Over Time')
        
        # Summary
        ax4.axis('off')
        summary_text = ""
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            summary_text += f"{condition}: Entropy={subset['Entropy'].mean():.3f}, Vol={subset['Volatility'].mean():.2f}\n"
        ax4.text(0.1, 0.9, summary_text, fontsize=10, fontfamily='monospace')
        
        plt.tight_layout()
        plt.savefig('entropy_analysis.png', dpi=300)
        plt.show()
        
        corr = df['Entropy'].corr(df['Volatility'])
        print(f"Correlation: {corr:.3f} ({'negative' if corr < 0 else 'positive'})")
        
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    visualize_entropy_volatility()
