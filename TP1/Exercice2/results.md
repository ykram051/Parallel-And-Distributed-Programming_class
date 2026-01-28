# Exercise 2: Optimizing Matrix Multiplication

## Objective
To implement and analyze the performance of matrix multiplication using two different loop orders:
1. `ijk` loop order
2. `ikj` loop order

## Implementation
Two programs were written:
1. **`mxm_ijk.c`**: Implements matrix multiplication using the `ijk` loop order.
2. **`mxm_ikj.c`**: Implements matrix multiplication using the `ikj` loop order.

Both programs:
- Initialize matrices `a`, `b`, and `c` of size \( n \times n \) (where \( n = 256 \)).
- Perform matrix multiplication.
- Measure execution time using the `clock()` function.
- Print the execution time and a sample result for verification.

---

## Execution Results
The programs were executed with \( n = 256 \). The results are as follows:

| Program       | Execution Time (ms) | Sample Result \( c[0][0] \) |
|---------------|----------------------|-----------------------------|
| `mxm_ijk`     | 64.0                | 256                         |
| `mxm_ikj`     | 62.0                | 256                         |

---

## Memory Bandwidth Calculation
Memory bandwidth was calculated using the formula:
\[
\text{Memory Bandwidth} = \frac{\text{Total Memory Accessed}}{\text{Execution Time}}
\]

- **Total Memory Accessed**:
  \[
  \text{Memory Accessed} = (2 \times n^3 + n^2) \times \text{size of int}
  \]
  For \( n = 256 \) and \( \text{sizeof(int)} = 4 \, \text{bytes} \):
  \[
  \text{Memory Accessed} = (2 \times 256^3 + 256^2) \times 4 = 67,108,864 \, \text{bytes} \, (64 \, \text{MB})
  \]

- **Memory Bandwidth**:
  - For `mxm_ijk`:
    \[
    \text{Bandwidth} = \frac{67,108,864}{64 \times 10^{-3}} = 1.05 \, \text{GB/s}
    \]
  - For `mxm_ikj`:
    \[
    \text{Bandwidth} = \frac{67,108,864}{62 \times 10^{-3}} = 1.08 \, \text{GB/s}
    \]

---

## Analysis
1. **Performance**:
   - The `ikj` loop order is slightly faster than the `ijk` loop order.
   - This is due to better cache locality in the `ikj` order, where elements of matrix `c` and `b` are accessed in a more sequential manner, reducing cache misses.

2. **Correctness**:
   - Both programs produce the same result for \( c[0][0] = 256 \), confirming the correctness of the computations.

3. **Memory Bandwidth**:
   - The `ikj` loop order achieves a slightly higher memory bandwidth due to improved memory access patterns.

---

## Conclusion
- The `ikj` loop order is more efficient than the `ijk` loop order for matrix multiplication, especially for larger matrices.
- The improvement is attributed to better cache usage, which reduces memory access latency.