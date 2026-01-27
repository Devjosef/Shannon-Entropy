import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np


def visualize_entropy_volatility(csv_file="tests/visual_inspection_data.csv"):
    try:
        df = pd.read_csv(csv_file)
        print("Data loaded successfully:")
        print(df.head())
        
        fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 14))
        fig.suptitle('Shannon Entropy vs Market Volatility Analysis', fontsize=18, fontweight='bold')

        sns.regplot(data=df, x='Entropy', y='Volatility', ax=ax1,
                    scatter_kws={'s':60, 'alpha':0.65, 'edgecolors':'black',
                                 'linewidths':0.8},
                    line_kws={'color':'black', 'linestyle':'--', 'lw':3})
        ax1.set_xlabel('Shannon Entropy (bits)', fontsize=14, fontweight='bold')
        ax1.set_ylabel('Market Volatility', fontsize=14, fontweight='bold')
        ax1.legend(bbox_to_anchor=(1.05, 1), loc='upper left', fontsize=11)
        ax1.grid(True, alpha=0.3)
        ax1.set_title('Negative Correlation: ρ ={:.3f}'.format(df['Entropy'].corr(df['Volatility'])), fontsize=14, fontweight='bold', pad=20)

        ax2.plot(df['Window_ID'], df['Entropy'], 'b-', linewidth=2, alpha=0.8)
        ax2.set_title('Entropy Over Time', fontsize=14, fontweight='bold')
        ax2.set_xlabel('Window ID', fontsize=12)
        ax2.grid(True, alpha=0.3)

        ax3.plot(df['Window_ID'], df['Volatility'], 'r-', linewidth=2, alpha=0.8)
        ax3.set_title('Volatility Over Time', fontsize=14, fontweight='bold')
        ax3.set_xlabel('Window ID', fontsize=12)
        ax3.grid(True, alpha=0.3)

        ax4.axis('off')
        summary_text = "MARKET CONDITION SUMMARY\n"
        summary_text += "="*40 + "\n"
        for condition in df['Market_Condition'].unique():
            subset = df[df['Market_Condition'] == condition]
            summary_text += f"{condition:<15}: H={subset['Entropy'].mean():6.3f}, V={subset['Volatility'].mean():6.2f}\n"
        
        summary_text += "="*40 + "\n"
        summary_text += f"OVERALL CORRELATION: {df['Entropy'].corr(df['Volatility']):.3f}"

        ax4.text(0.05, 0.95, summary_text, fontsize=12, fontfamily='monospace',
            verticalalignment='top', transform=ax4.transAxes, bbox=dict(boxstyle="round,pad=0.3", facecolor="lightgray", alpha=0.8))
            
        plt.tight_layout()
        plt.savefig('entropy_analysis.png', dpi=300, bbox_inches='tight', facecolor='white')
        plt.show()

        corr = df['Entropy'].corr(df['Volatility'])
        print(f"\n✓ Correlation: {corr:.3f} ({'negative' if corr < 0 else 'positive'})")
        print("✓ Graph saved as 'entropy_analysis.png'")
        
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    visualize_entropy_volatility()
