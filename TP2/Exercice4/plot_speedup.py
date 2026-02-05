#!/usr/bin/env python3

import matplotlib
matplotlib.use('Agg')  # Use non-interactive backend
import matplotlib.pyplot as plt

# Exercise 4: Speedup Curves for Amdahl and Gustafson Laws
# fs ≈ 0.0000028 (from measured data)

fs = 0.0000028
p_values = [1, 2, 4, 8, 16, 32, 64]

# Calculate speedups
amdahl_speedups = [1 / (fs + (1 - fs) / p) for p in p_values]
gustafson_speedups = [p - fs * (p - 1) for p in p_values]

# Plot Amdahl
plt.figure(figsize=(8, 6))
plt.plot(p_values, amdahl_speedups, marker='o', label="Amdahl's Law", color='blue')
plt.xlabel('Processors (p)')
plt.ylabel('Speedup')
plt.title("Amdahl's Law Speedup Curve (Exercise 4)")
plt.grid(True)
plt.legend()
plt.savefig('amdahl_ex4.png', dpi=300, bbox_inches='tight')
plt.close()

# Plot Gustafson
plt.figure(figsize=(8, 6))
plt.plot(p_values, gustafson_speedups, marker='o', label="Gustafson's Law", color='red')
plt.xlabel('Processors (p)')
plt.ylabel('Speedup')
plt.title("Gustafson's Law Speedup Curve (Exercise 4)")
plt.grid(True)
plt.legend()
plt.savefig('gustafson_ex4.png', dpi=300, bbox_inches='tight')
plt.close()

print("Plots saved as amdahl_ex4.png and gustafson_ex4.png")