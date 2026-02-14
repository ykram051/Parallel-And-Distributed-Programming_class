import matplotlib.pyplot as plt
import numpy as np

# Example data, replace with your actual measurements
threads = [1, 2, 4, 8, 16]
times = [10.0, 5.5, 3.0, 2.0, 1.8]  # Replace with your measured times

speedup = [times[0]/t for t in times]
efficiency = [s/t for s, t in zip(speedup, threads)]

plt.figure(figsize=(8, 6))
plt.plot(threads, speedup, marker='o', label='Speedup')
plt.plot(threads, efficiency, marker='s', label='Efficiency')
plt.xlabel('Number of Threads')
plt.ylabel('Speedup / Efficiency')
plt.title('Speedup and Efficiency for Matrix Multiplication')
plt.legend()
plt.grid(True)
plt.savefig('matmul_speedup_efficiency.png')
plt.show()
