# Exercise 3: Sequential vs. Parallel Code Analysis & Amdahl’s Law

## Question 1 — Code Analysis
1. **Strictly sequential part:** `add_noise` function - it computes each element based on the previous one, creating a dependency chain.
2. **Parallelizable part:** `init_b`, `compute_addition`, and `reduction` - these loops have no dependencies between iterations and can be parallelized.
3. **Time complexity:**
   - `add_noise`: O(N)
   - `init_b`: O(N)
   - `compute_addition`: O(N)
   - `reduction`: O(N)

## Question 2 — Profiling with Valgrind/Callgrind
With Valgrind profiling (slower due to instrumentation):
- Total instructions: 6,400,235,763 Ir
- add_noise: 1,800,000,015 Ir (28.12%)
- init_b: 1,200,000,026 Ir (18.75%)
- compute_addition: 2,200,000,028 Ir (34.37%)
- reduction: 1,200,000,028 Ir (18.75%)
- Total time: 14.037228 s

## Question 3 — Sequential Fraction \(f_s\)
Measured times (normal run, N=100000000):
- add_noise: 0.592232 s (sequential)
- init_b: 0.468825 s (parallel)
- compute_addition: 0.550309 s (parallel)
- reduction: 0.260550 s (parallel)
- Total time: 1.872146 s

\(f_s = \frac{0.592232}{1.872146} \approx 0.316\)

With Valgrind profiling (slower due to instrumentation):
- Total instructions: 6,400,235,763 Ir
- add_noise: 1,800,000,015 Ir (28.12%)
- init_b: 1,200,000,026 Ir (18.75%)
- compute_addition: 2,200,000,028 Ir (34.37%)
- reduction: 1,200,000,028 Ir (18.75%)
- Total time: 14.037228 s

## Question 4 — Strong Scaling (Amdahl’s Law)
Speedup \(S(p) = \frac{1}{0.316 + \frac{0.684}{p}}\)

Speedup curve (approximate values):
- p=1: 1.00
- p=2: 1.58
- p=4: 2.35
- p=8: 3.00
- p=16: 3.58
- p=32: 3.92
- p=64: 4.12

(Plot: Speedup increases but flattens due to fixed sequential fraction.)

## Question 5 — Weak Scaling (Gustafson’s Law)
Assuming N increases proportionally with p (e.g., N_p = p * N_1), \(f_s\) decreases as sequential time stays constant while parallel time scales.
Gustafson speedup \(S(p) = p - 0.316 \times (p - 1)\)

Speedup curve (approximate values):
- p=1: 1.00
- p=2: 1.68
- p=4: 3.37
- p=8: 6.74
- p=16: 13.47
- p=32: 26.95
- p=64: 53.89

(Plot: Near-linear increase as problem size scales with p.)

## Conclusion
Amdahl’s law shows limited speedup due to the fixed sequential fraction (~4x max), while Gustafson’s law predicts much better scaling for growing problem sizes. Profiling confirms time distribution matches instruction counts.