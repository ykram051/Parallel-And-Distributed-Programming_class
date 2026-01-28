import pandas as pd
import matplotlib.pyplot as plt

O0 = pd.read_csv('stride0.csv', encoding='latin1')
O2 = pd.read_csv('stride2.csv', encoding='latin1')

plt.figure(figsize=(12,5))

plt.subplot(1,2,1)
plt.plot(O0['stride'], O0['time(msec)'], label='O0', marker='o')
plt.plot(O2['stride'], O2['time(msec)'], label='O2', marker='o')
plt.xlabel('Stride')
plt.ylabel('Time (ms)')
plt.title('Execution Time vs Stride')
plt.legend()

plt.subplot(1,2,2)
plt.plot(O0['stride'], O0['rate(MB/s)'], label='O0', marker='o')
plt.plot(O2['stride'], O2['rate(MB/s)'], label='O2', marker='o')
plt.xlabel('Stride')
plt.ylabel('Bandwidth (MB/s)')
plt.title('Bandwidth vs Stride')
plt.legend()

plt.tight_layout()
plt.savefig('stride_comparison.png')
plt.show()