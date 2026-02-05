## Tasks
1. **Measure sequential fraction:** Using Valgrind/Callgrind (total Ir: 25,048,263,202). Sequential: `generate_noise` (O(N)), parallel: `init_matrix` (O(N^2)), `matmul` (O(N^3)).
2. **Strong scaling (Amdahl’s Law):** \(f_s \approx 0.0000028\) (generate_noise time / total time). Speedup \(S(p) = \frac{1}{0.0000028 + \frac{0.9999972}{p}}\) ≈ \(\frac{p}{p - 0.9999972 \times (p-1)}\) ≈ p (nearly linear).

Speedup curve (approximate values, nearly ideal):
- p=1: 1.00
- p=2: 2.00
- p=4: 4.00
- p=8: 8.00
- p=16: 16.00
- p=32: 32.00
- p=64: 64.00

(Plot: Nearly linear speedup due to negligible sequential fraction.)

3. **Weak scaling (Gustafson’s Law):** \(S(p) = p - 0.0000028 \times (p - 1)\) ≈ p.

Speedup curve (approximate values):
- p=1: 1.00
- p=2: 2.00
- p=4: 4.00
- p=8: 8.00
- p=16: 16.00
- p=32: 32.00
- p=64: 64.00

(Plot: Linear scaling as problem size increases with p.)
5. **Compare with Exercise 3:** Ex3 has \(f_s = 0.316\) (significant sequential bottleneck), limiting speedup to ~4x. Ex4 has negligible \(f_s\), allowing much better parallelization, but dominated by O(N^3) matmul complexity.

## Measured Times (N=1000)
- generate_noise: 0.000010 s (sequential)
- init_matrix (A): 0.007771 s (parallel)
- init_matrix (B): 0.006898 s (parallel)
- matmul: 3.501092 s (parallel)
- Total: 3.515931 s

## Conclusion
Ex4 scales much better than Ex3 due to minimal sequential fraction, but matrix multiplication's cubic complexity makes it more sensitive to N. Amdahl and Gustafson predict similar near-ideal scaling here.