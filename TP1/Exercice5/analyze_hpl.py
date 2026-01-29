#!/usr/bin/env python3
"""
HPL Benchmark Analysis - Exercise 5
Generates plots from measured HPL results.
"""

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# =============================================================================
# Configuration
# =============================================================================
# Empirical peak based on maximum measured performance
EMPIRICAL_PEAK_GFLOPS = 70.0

print(f"Test System: Apple M3 Pro (single core, single thread)")
print(f"Empirical Peak: {EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s")
print()

# =============================================================================
# Load Measured Data
# =============================================================================
df = pd.read_csv('hpl_results.csv')
df['Efficiency'] = (df['GFLOPs'] / EMPIRICAL_PEAK_GFLOPS) * 100

print("MEASURED HPL RESULTS")
print("=" * 60)
print(df.to_string(index=False))
print()

# =============================================================================
# Summary Statistics
# =============================================================================
print("SUMMARY BY PROBLEM SIZE")
print("=" * 60)
for N in sorted(df['N'].unique()):
    subset = df[df['N'] == N]
    best_row = subset.loc[subset['GFLOPs'].idxmax()]
    print(f"N={N:5d}: Best NB={int(best_row['NB']):3d}, "
          f"GFLOP/s={best_row['GFLOPs']:.2f}, "
          f"Efficiency={best_row['GFLOPs']/EMPIRICAL_PEAK_GFLOPS*100:.1f}%")

# =============================================================================
# Figure 1: Performance vs Block Size (All N values)
# =============================================================================
fig, axes = plt.subplots(2, 2, figsize=(12, 10))
fig.suptitle(f'Measured HPL Performance: Effect of Block Size (NB)\n'
             f'Test System: Apple M3 Pro, Single Core, Peak={EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s',
             fontsize=12, fontweight='bold')

colors = {'1000': '#1f77b4', '5000': '#ff7f0e', '10000': '#2ca02c', '20000': '#d62728'}
n_values = sorted(df['N'].unique())

for idx, N in enumerate(n_values):
    ax = axes[idx // 2, idx % 2]
    subset = df[df['N'] == N].sort_values('NB')
    
    ax.plot(subset['NB'], subset['GFLOPs'], 'o-', 
            color=colors[str(N)], linewidth=2, markersize=8,
            label=f'Measured (N={N})')
    
    # Mark optimal point
    best_idx = subset['GFLOPs'].idxmax()
    best_nb = subset.loc[best_idx, 'NB']
    best_gflops = subset.loc[best_idx, 'GFLOPs']
    ax.plot(best_nb, best_gflops, '*', color='gold', markersize=15, 
            markeredgecolor='black', markeredgewidth=1.5,
            label=f'Optimal: NB={int(best_nb)}, {best_gflops:.1f} GFLOP/s')
    
    ax.axhline(y=EMPIRICAL_PEAK_GFLOPS, color='gray', linestyle='--', linewidth=1.5,
               label=f'Empirical Peak ({EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s)')
    
    ax.set_xlabel('Block Size (NB)', fontsize=10)
    ax.set_ylabel('Performance (GFLOP/s)', fontsize=10)
    ax.set_title(f'N = {N}', fontsize=11, fontweight='bold')
    ax.set_xscale('log', base=2)
    ax.set_xticks([1, 2, 4, 8, 16, 32, 64, 128, 256])
    ax.set_xticklabels(['1', '2', '4', '8', '16', '32', '64', '128', '256'])
    ax.grid(True, alpha=0.3)
    ax.legend(loc='best', fontsize=8)
    ax.set_ylim(0, max(EMPIRICAL_PEAK_GFLOPS * 1.1, subset['GFLOPs'].max() * 1.1))

plt.tight_layout()
plt.savefig('hpl_nb_analysis.png', dpi=150, bbox_inches='tight')
print("\nSaved: hpl_nb_analysis.png")

# =============================================================================
# Figure 2: Efficiency vs Problem Size
# =============================================================================
fig, ax = plt.subplots(figsize=(10, 6))

best_per_n = df.loc[df.groupby('N')['GFLOPs'].idxmax()]
best_per_n = best_per_n.sort_values('N')

bars = ax.bar(range(len(best_per_n)), best_per_n['Efficiency'], 
              color=['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728'],
              edgecolor='black', linewidth=1.2)

for i, (idx, row) in enumerate(best_per_n.iterrows()):
    ax.text(i, row['Efficiency'] + 2, f"{row['Efficiency']:.1f}%\n(NB={int(row['NB'])})",
            ha='center', va='bottom', fontsize=10, fontweight='bold')

ax.set_xticks(range(len(best_per_n)))
ax.set_xticklabels([f'N={int(n)}' for n in best_per_n['N']], fontsize=11)
ax.set_ylabel('Efficiency (% of Empirical Peak)', fontsize=11)
ax.set_xlabel('Problem Size', fontsize=11)
ax.set_title(f'Measured HPL Efficiency vs Problem Size\n'
             f'Test System: Apple M3 Pro, Peak={EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s',
             fontsize=12, fontweight='bold')
ax.set_ylim(0, 100)
ax.axhline(y=100, color='red', linestyle='--', linewidth=1.5, label='100% Efficiency')
ax.grid(True, axis='y', alpha=0.3)

plt.tight_layout()
plt.savefig('hpl_efficiency.png', dpi=150, bbox_inches='tight')
print("Saved: hpl_efficiency.png")

# =============================================================================
# Figure 3: Combined Performance Overview
# =============================================================================
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

for N in n_values:
    subset = df[df['N'] == N].sort_values('NB')
    ax1.plot(subset['NB'], subset['GFLOPs'], 'o-', label=f'N={N}', linewidth=2, markersize=6)

ax1.axhline(y=EMPIRICAL_PEAK_GFLOPS, color='red', linestyle='--', linewidth=2,
            label=f'Empirical Peak ({EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s)')
ax1.set_xlabel('Block Size (NB)', fontsize=11)
ax1.set_ylabel('Performance (GFLOP/s)', fontsize=11)
ax1.set_title('Measured Performance vs Block Size', fontsize=12, fontweight='bold')
ax1.set_xscale('log', base=2)
ax1.legend(loc='best')
ax1.grid(True, alpha=0.3)

ax2.plot(best_per_n['N'], best_per_n['GFLOPs'], 'o-', 
         color='blue', linewidth=2.5, markersize=10, label='Measured (best NB)')
ax2.axhline(y=EMPIRICAL_PEAK_GFLOPS, color='red', linestyle='--', linewidth=2,
            label=f'Empirical Peak ({EMPIRICAL_PEAK_GFLOPS:.0f} GFLOP/s)')
ax2.set_xlabel('Problem Size (N)', fontsize=11)
ax2.set_ylabel('Performance (GFLOP/s)', fontsize=11)
ax2.set_title('Performance Scaling with Problem Size', fontsize=12, fontweight='bold')
ax2.set_xscale('log')
ax2.legend(loc='lower right')
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('hpl_combined.png', dpi=150, bbox_inches='tight')
print("Saved: hpl_combined.png")

print("\nAnalysis complete!")
