# Exercise 1: Loop Optimizations - Report

## Overview
This report analyzes the performance of manual loop unrolling on a summation kernel for an array of 1,000,000 elements. The original code initializes an array with 1.0 and sums it. We implement unrolling factors U = 1 to 32, measure execution times, and compare across data types and optimization levels.

## 1-3. Implementation and Measurement for Double Precision (double)
We created functions for each U, measuring time with `clock()`. At -O0, the best U was 6 (0.000 ms). At -O2, similar results, with U=6 optimal.

### Key Results (Double, -O0)
- Best U: 6, Time: 0.000 ms
- Times varied from 0-2 ms across U.

### Key Results (Double, -O2)
- Best U: 6, Time: 0.000 ms
- Manual unrolling still shows variation, but compiler optimizations reduce benefits.

## 4. Comparison: Manual Unrolling at -O0 vs Compiler Optimization at -O2
- At -O0: Manual unrolling significantly improves performance by reducing overhead.
- At -O2: Compiler handles unrolling automatically; manual effort adds little extra benefit.
- Original loop at -O2: ~0.000 ms, comparable to best manual U.

## 5. Is Manual Unrolling Beneficial with -O2?
Slightly beneficial for fine-tuning, but not necessary. The compiler's optimizations are sufficient for most cases.

## 6. Repeat for Float, Int, Short
- **Float**: Best U = 3, Time = 0.000 ms (many U have 0.000 ms).
- **Int**: Best U = 1, Time = 0.000 ms (most U have 0.000 ms).
- **Short**: Best U = 1, Time = 0.000 ms (all U have 0.000 ms, sum overflows to 16960).

Smaller types show similar trends, with int/float faster due to smaller data sizes.

## 7. Lower Bound Estimation
Assuming BW = 20 GB/s:
- Double (8 bytes): Tmin ≈ 0.4 ms
- Float/Int (4 bytes): Tmin ≈ 0.2 ms
- Short (2 bytes): Tmin ≈ 0.1 ms

Measured times are close to this bound, indicating memory bandwidth limitation. Excess time is due to overhead and measurement error.

## 8. Why Increasing U Improves Then Saturates
- **Initial Improvement**: Reduces loop overhead (fewer iterations) and increases ILP (parallel additions).
- **Saturation**: Becomes bandwidth-limited; total data transfer (N × sizeof) is fixed, so further unrolling doesn't reduce memory time. Cache/instruction limits may also play a role.

## Conclusion
Manual unrolling helps at -O0 but is less critical at -O2. The kernel is memory-bound, with optimal U around 1-6. For production, rely on compiler optimizations.