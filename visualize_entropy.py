"""
Shannon Entropy Market Analysis Visualizer
Reads CSV data from C++ output and creates charts showing the entropy volatility correlation
"""

import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore
import seaborn as sns # type: ignore
import numpy as np # type: ignore

def visualize_entropy_volatility(csv_file="tests/visual_inspection_data.csv"):
    """Create visualizations of entropy-volatility relationship"""
    
    try:
        # Read CSV data
        df = pd.read_csv(csv_file)
        print("Data loaded successfully:")
        print(df)
        print("\n" + "="*50)
        
        # Create figure with multiple subplots
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
        fig.suptitle('Shannon Entropy vs Market Volatility Analysis', fontsize=16, fontweight='bold')
        
        # 1. Scatter plot: Entropy vs Volatility (colored by market condition)
        colors = {'Bull Market': 'green', 'Bear Market': 'orange', 'Market Crash': 'red', 'Recovery': 'blue'}
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            ax1.scatter(subset['Entropy'], subset['Volatility'], 
                       s=80, alpha=0.7, c=colors.get(condition, 'gray'), 
                       label=condition, edgecolors='black', linewidth=0.5)
        
        ax1.set_xlabel('Shannon Entropy (bits)')
        ax1.set_ylabel('Market Volatility')
        ax1.set_title('Entropy vs Volatility by Market Condition')
        ax1.grid(True, alpha=0.3)
        ax1.legend()
        
        # Add trend line
        z = np.polyfit(df['Entropy'], df['Volatility'], 1)
        p = np.poly1d(z)
        ax1.plot(df['Entropy'], p(df['Entropy']), 
                "k--", alpha=0.8, linewidth=2, label=f'Trend: y={z[0]:.2f}x+{z[1]:.2f}')
        ax1.legend()
        
        # 2. Time series: Entropy over windows
        ax2.plot(df['Window_ID'], df['Entropy'], 'b-', linewidth=2, alpha=0.8)
        ax2.scatter(df['Window_ID'], df['Entropy'], s=30, c='blue', alpha=0.6)
        ax2.set_xlabel('Window ID')
        ax2.set_ylabel('Shannon Entropy (bits)')
        ax2.set_title('Entropy Over Time')
        ax2.grid(True, alpha=0.3)
        
        # Add market condition regions
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            if not subset.empty:
                ax2.axvspan(subset['Window_ID'].min(), subset['Window_ID'].max(), 
                           alpha=0.2, color=colors.get(condition, 'gray'), label=condition)
        
        # 3. Time series: Volatility over windows
        ax3.plot(df['Window_ID'], df['Volatility'], 'r-', linewidth=2, alpha=0.8)
        ax3.scatter(df['Window_ID'], df['Volatility'], s=30, c='red', alpha=0.6)
        ax3.set_xlabel('Window ID')
        ax3.set_ylabel('Market Volatility')
        ax3.set_title('Volatility Over Time')
        ax3.grid(True, alpha=0.3)
        
        # Add market condition regions
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            if not subset.empty:
                ax3.axvspan(subset['Window_ID'].min(), subset['Window_ID'].max(), 
                           alpha=0.2, color=colors.get(condition, 'gray'))
        
        # 4. Market condition summary
        ax4.axis('off')
        summary_text = "Market Condition Analysis:\n\n"
        
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            avg_entropy = subset['Entropy'].mean()
            avg_volatility = subset['Volatility'].mean()
            count = len(subset)
            
            summary_text += f"{condition} ({count} windows):\n"
            summary_text += f"• Avg Entropy: {avg_entropy:.3f} bits\n"
            summary_text += f"• Avg Volatility: {avg_volatility:.2f}\n"
            summary_text += f"• Pattern: {subset['Behavior_Pattern'].mode().iloc[0]}\n\n"
        
        ax4.text(0.1, 0.95, summary_text, transform=ax4.transAxes, 
                fontsize=10, verticalalignment='top', fontfamily='monospace')
        
        plt.tight_layout()
        plt.savefig('entropy_volatility_visual_inspection.png', dpi=300, bbox_inches='tight')
        plt.show()
        
        # Print correlation analysis
        correlation = df['Entropy'].corr(df['Volatility'])
        print(f"\nCorrelation Analysis:")
        print(f"Entropy-Volatility Correlation: {correlation:.3f}")
        print(f"Interpretation: {'Strong positive' if correlation > 0.7 else 'Moderate positive' if correlation > 0.3 else 'Weak'} correlation")
        
    except FileNotFoundError:
        print(f"Error: {csv_file} not found. Run the C++ program first to generate the CSV file.")
    except Exception as e:
        print(f"Error visualizing data: {e}")

if __name__ == "__main__":
    import sys
    
    # Use command line argument if provided, otherwise use default
    csv_file = sys.argv[1] if len(sys.argv) > 1 else "market_analysis.csv"
    visualize_entropy_volatility(csv_file)