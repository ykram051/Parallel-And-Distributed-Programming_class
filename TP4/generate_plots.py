"""
Generate plots for TP4 Exercise 4: Synchronization and Barrier Cost
Creates plots for: CPU Time, Speedup, Efficiency, and MFLOP/s
"""

import matplotlib.pyplot as plt
import numpy as np

# Data from Exercise 4 execution results
threads = [1, 2, 4, 8, 16]

# CPU Time (seconds)
v1_time = [0.644, 0.373, 0.221, 0.162, 0.194]
v3_time = [0.568, 0.264, 0.224, 0.177, 0.171]

# Sequential time for speedup calculation
seq_time = 0.204

# Calculate speedup
v1_speedup = [seq_time / t for t in v1_time]
v3_speedup = [seq_time / t for t in v3_time]
ideal_speedup = threads

# Calculate efficiency
v1_efficiency = [s / t for s, t in zip(v1_speedup, threads)]
v3_efficiency = [s / t for s, t in zip(v3_speedup, threads)]

# MFLOP/s
flops = 48000000  # 2 * 40000 * 600
v1_mflops = [flops / (t * 1e6) for t in v1_time]
v3_mflops = [flops / (t * 1e6) for t in v3_time]

# Set style
plt.style.use('seaborn-v0_8-whitegrid')
plt.rcParams['figure.figsize'] = (8, 6)
plt.rcParams['font.size'] = 12
plt.rcParams['axes.labelsize'] = 14
plt.rcParams['axes.titlesize'] = 16

# Create figures directory
import os
os.makedirs('figures', exist_ok=True)

# ============================================
# Plot 1: CPU Time
# ============================================
fig, ax = plt.subplots()
ax.plot(threads, v1_time, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_time, 'r-s', linewidth=2, markersize=8, label='V3 (static nowait)')
ax.axhline(y=seq_time, color='g', linestyle='--', linewidth=2, label=f'Sequential ({seq_time}s)')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('CPU Time (seconds)')
ax.set_title('CPU Time vs Number of Threads')
ax.set_xticks(threads)
ax.legend()
ax.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('figures/cpu_time.pdf', dpi=300, bbox_inches='tight')
plt.savefig('figures/cpu_time.png', dpi=300, bbox_inches='tight')
print("Saved: figures/cpu_time.pdf")

# ============================================
# Plot 2: Speedup
# ============================================
fig, ax = plt.subplots()
ax.plot(threads, v1_speedup, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_speedup, 'r-s', linewidth=2, markersize=8, label='V3 (static nowait)')
ax.plot(threads, ideal_speedup, 'k--', linewidth=2, label='Ideal speedup')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Speedup')
ax.set_title('Speedup vs Number of Threads')
ax.set_xticks(threads)
ax.legend()
ax.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('figures/speedup.pdf', dpi=300, bbox_inches='tight')
plt.savefig('figures/speedup.png', dpi=300, bbox_inches='tight')
print("Saved: figures/speedup.pdf")

# ============================================
# Plot 3: Efficiency
# ============================================
fig, ax = plt.subplots()
ax.plot(threads, v1_efficiency, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_efficiency, 'r-s', linewidth=2, markersize=8, label='V3 (static nowait)')
ax.axhline(y=1.0, color='k', linestyle='--', linewidth=2, label='Ideal efficiency')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Efficiency')
ax.set_title('Efficiency vs Number of Threads')
ax.set_xticks(threads)
ax.set_ylim([0, 1.1])
ax.legend()
ax.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('figures/efficiency.pdf', dpi=300, bbox_inches='tight')
plt.savefig('figures/efficiency.png', dpi=300, bbox_inches='tight')
print("Saved: figures/efficiency.pdf")

# ============================================
# Plot 4: MFLOP/s
# ============================================
fig, ax = plt.subplots()
ax.plot(threads, v1_mflops, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_mflops, 'r-s', linewidth=2, markersize=8, label='V3 (static nowait)')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('MFLOP/s')
ax.set_title('MFLOP/s vs Number of Threads')
ax.set_xticks(threads)
ax.legend()
ax.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('figures/mflops.pdf', dpi=300, bbox_inches='tight')
plt.savefig('figures/mflops.png', dpi=300, bbox_inches='tight')
print("Saved: figures/mflops.pdf")

# ============================================
# Combined plot (2x2)
# ============================================
fig, axes = plt.subplots(2, 2, figsize=(12, 10))

# CPU Time
ax = axes[0, 0]
ax.plot(threads, v1_time, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_time, 'r-s', linewidth=2, markersize=8, label='V3 (static)')
ax.axhline(y=seq_time, color='g', linestyle='--', linewidth=2, label='Sequential')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('CPU Time (s)')
ax.set_title('CPU Time')
ax.set_xticks(threads)
ax.legend(fontsize=10)
ax.grid(True, alpha=0.3)

# Speedup
ax = axes[0, 1]
ax.plot(threads, v1_speedup, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_speedup, 'r-s', linewidth=2, markersize=8, label='V3 (static)')
ax.plot(threads, ideal_speedup, 'k--', linewidth=2, label='Ideal')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Speedup')
ax.set_title('Speedup')
ax.set_xticks(threads)
ax.legend(fontsize=10)
ax.grid(True, alpha=0.3)

# Efficiency
ax = axes[1, 0]
ax.plot(threads, v1_efficiency, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_efficiency, 'r-s', linewidth=2, markersize=8, label='V3 (static)')
ax.axhline(y=1.0, color='k', linestyle='--', linewidth=2, label='Ideal')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Efficiency')
ax.set_title('Efficiency')
ax.set_xticks(threads)
ax.set_ylim([0, 1.1])
ax.legend(fontsize=10)
ax.grid(True, alpha=0.3)

# MFLOP/s
ax = axes[1, 1]
ax.plot(threads, v1_mflops, 'b-o', linewidth=2, markersize=8, label='V1 (barrier)')
ax.plot(threads, v3_mflops, 'r-s', linewidth=2, markersize=8, label='V3 (static)')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('MFLOP/s')
ax.set_title('MFLOP/s')
ax.set_xticks(threads)
ax.legend(fontsize=10)
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('figures/all_metrics.pdf', dpi=300, bbox_inches='tight')
plt.savefig('figures/all_metrics.png', dpi=300, bbox_inches='tight')
print("Saved: figures/all_metrics.pdf")

print("\nAll plots generated successfully!")
