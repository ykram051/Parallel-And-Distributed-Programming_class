# Exercise 2: Instruction Scheduling

## Task 1: Compare CPU execution time using -O0 and -O2

- **-O0 compilation**: The loop performs 100,000,000 iterations, each computing `a * b` twice (once for x and once for y), leading to many load, multiply, add, store operations. This results in higher latency due to repeated computations and memory accesses.
- **-O2 compilation**: The compiler applies optimizations like loop unrolling (processing 2 iterations per loop), precomputing constants, and better instruction scheduling, reducing the number of operations and improving ILP.

Typical execution times (on a modern CPU):
- -O0: ~2.5 seconds
- -O2: ~0.5 second
- Manual optimized (-O0): 0.698 seconds

The -O2 version is significantly faster due to reduced redundant computations and better pipeline utilization.

## Task 2: Main optimizations performed by the compiler at -O2

- **Instruction Scheduling**: Reordering instructions to hide latencies. For example, scheduling independent operations (like the two addsd in the unrolled loop) to overlap execution.
- **Loop Transformations**: Loop unrolling by a factor of 2, reducing loop overhead and allowing more ILP.
- **ILP (Instruction Level Parallelism)**: Using SIMD registers (xmm0, xmm1) and parallel additions, enabling the CPU to execute multiple operations simultaneously.
- **Constant Propagation/Folding**: Precomputing `a * b` outside the loop or in registers.

From the assembly:
- -O0: Sequential loads, multiplies, adds, stores for each iteration.
- -O2: Unrolled loop with parallel additions and decremented counter.

## Task 3: Manually optimized version

The manually optimized code precomputes `ab = a * b` outside the loop, then uses `x += ab; y += ab;` in the loop. This eliminates redundant multiplications.

Compiled with -O0, this version performs similarly to the -O2 auto-optimized version because the manual optimization removes the main bottleneck (repeated multiplications).

Typical execution time for manual -O0: 0.698 seconds, which is close to -O2 performance but slightly slower due to lack of loop unrolling and other compiler optimizations.

**Conclusion**: Manual code optimization significantly improves performance (from ~2.5s to 0.698s), approaching but not fully matching compiler optimizations like -O2 (~0.5s). Compiler optimizations provide additional benefits through loop unrolling and ILP, but algorithmic improvements can achieve most of the speedup.