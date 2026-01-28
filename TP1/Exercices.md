# **Exercice 1**



PS C:\\Users\\USER\\Desktop\\S8\\Parallel and distributed programming\\TP1>

&nbsp;.\\stride.exe

stride,sum,time(msec),rate(MB/s)

1,1000000.000000,3.000000,2543.131510

2,1000000.000000,2.000000,3814.697266

3,1000000.000000,3.000000,2543.131510

4,1000000.000000,2.000000,3814.697266

5,1000000.000000,3.000000,2543.131510

6,1000000.000000,3.000000,2543.131510

7,1000000.000000,3.000000,2543.131510

8,1000000.000000,4.000000,1907.348633

9,1000000.000000,4.000000,1907.348633

10,1000000.000000,4.000000,1907.348633

11,1000000.000000,4.000000,1907.348633

12,1000000.000000,5.000000,1525.878906

13,1000000.000000,7.000000,1089.913504

14,1000000.000000,6.000000,1271.565755

15,1000000.000000,10.000000,762.939453

16,1000000.000000,7.000000,1089.913504

17,1000000.000000,10.000000,762.939453

18,1000000.000000,7.000000,1089.913504

19,1000000.000000,9.000000,847.710503

20,1000000.000000,9.000000,847.710503





PS C:\\Users\\USER\\Desktop\\S8\\Parallel and distributed programming\\TP1> gcc -O2 -o stride stride.c

PS C:\\Users\\USER\\Desktop\\S8\\Parallel and distributed programming\\TP1> .\\stride.exe

1,1000000.000000,1.000000,7629.394531

2,1000000.000000,2.000000,3814.697266

3,1000000.000000,2.000000,3814.697266

4,1000000.000000,1.000000,7629.394531

5,1000000.000000,2.000000,3814.697266

6,1000000.000000,2.000000,3814.697266

7,1000000.000000,3.000000,2543.131510

8,1000000.000000,4.000000,1907.348633

9,1000000.000000,4.000000,1907.348633

10,1000000.000000,5.000000,1525.878906

11,1000000.000000,4.000000,1907.348633

12,1000000.000000,4.000000,1907.348633

13,1000000.000000,5.000000,1525.878906

14,1000000.000000,6.000000,1271.565755

15,1000000.000000,6.000000,1271.565755

16,1000000.000000,7.000000,1089.913504

17,1000000.000000,5.000000,1525.878906

18,1000000.000000,5.000000,1525.878906

19,1000000.000000,5.000000,1525.878906

20,1000000.000000,5.000000,1525.878906



-----------------------------------------------------------------------------------------------

# **Exercise 2**



\#### \*\*Objective\*\*

To implement and analyze the performance of matrix multiplication using two different loop orders:

1\. `ijk` loop order

2\. `ikj` loop order



\#### \*\*Implementation\*\*

Two programs were written:

1\. \*\*mxm\_ijk.c\*\*: Implements matrix multiplication using the `ijk` loop order.

2\. \*\*mxm\_ikj.c\*\*: Implements matrix multiplication using the `ikj` loop order.



Both programs:

\- Initialize matrices `a`, `b`, and `c` of size \\( n \\times n \\) (where \\( n = 256 \\)).

\- Perform matrix multiplication.

\- Measure execution time using the `clock()` function.

\- Print the execution time and a sample result for verification.







\#### \*\*Execution Results\*\*

The programs were executed with \\( n = 256 \\). The results are as follows:



| Program       | Execution Time (ms) | Sample Result \\( c\[0]\[0] \\) |

|---------------|----------------------|-----------------------------|

| `mxm\_ijk`     | 64.0                | 256                         |

| `mxm\_ikj`     | 62.0                | 256                         |



\#### \*\*Memory Bandwidth Calculation\*\*

Memory bandwidth was calculated using the formula:

\\\[

\\text{Memory Bandwidth} = \\frac{\\text{Total Memory Accessed}}{\\text{Execution Time}}

\\]



\- \*\*Total Memory Accessed\*\*:

&nbsp; \\\[

&nbsp; \\text{Memory Accessed} = (2 \\times n^3 + n^2) \\times \\text{size of int}

&nbsp; \\]

&nbsp; For \\( n = 256 \\) and \\( \\text{sizeof(int)} = 4 \\, \\text{bytes} \\):

&nbsp; \\\[

&nbsp; \\text{Memory Accessed} = (2 \\times 256^3 + 256^2) \\times 4 = 67,108,864 \\, \\text{bytes} \\, (64 \\, \\text{MB})

&nbsp; \\]



\- \*\*Memory Bandwidth\*\*:

&nbsp; - For `mxm\_ijk`:

&nbsp;   \\\[

&nbsp;   \\text{Bandwidth} = \\frac{67,108,864}{64 \\times 10^{-3}} = 1.05 \\, \\text{GB/s}

&nbsp;   \\]

&nbsp; - For `mxm\_ikj`:

&nbsp;   \\\[

&nbsp;   \\text{Bandwidth} = \\frac{67,108,864}{62 \\times 10^{-3}} = 1.08 \\, \\text{GB/s}

&nbsp;   \\]



\#### \*\*Analysis\*\*

1\. \*\*Performance\*\*:

&nbsp;  - The `ikj` loop order is slightly faster than the `ijk` loop order.

&nbsp;  - This is due to better cache locality in the `ikj` order, where elements of matrix `c` and `b` are accessed in a more sequential manner, reducing cache misses.



2\. \*\*Correctness\*\*:

&nbsp;  - Both programs produce the same result for \\( c\[0]\[0] = 256 \\), confirming the correctness of the computations.



3\. \*\*Memory Bandwidth\*\*:

&nbsp;  - The `ikj` loop order achieves a slightly higher memory bandwidth due to improved memory access patterns.



\#### \*\*Conclusion\*\*

\- The `ikj` loop order is more efficient than the `ijk` loop order for matrix multiplication, especially for larger matrices.

\- The improvement is attributed to better cache usage, which reduces memory access latency.

---------------------------------------------------------------------------------------------

Exercice 3



