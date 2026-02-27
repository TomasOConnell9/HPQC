import matplotlib.pyplot as plt
import numpy as np

message_sizes1 = np.array([16, 64, 256, 1034, 4096, 8192, 32768, 131072, 524288, 2097152])
avg_times1 = np.array([0.000001, 0.000001, 0.000001, 0.000002, 0.000012, 0.000012, 0.000020, 0.000041, 0.000133, 0.000492])

message_sizes2 = np.array([16, 64, 256, 1024, 4096, 8182, 32768, 131072, 524288, 2097152])
avg_times2 = np.array([0.000001, 0.000001, 0.000001, 0.000001, 0.000008, 0.000009, 0.000015, 0.000038, 0.000130, 0.000495])

m1, c1 = np.polyfit(message_sizes1, avg_times1, 1)
m2, c2 = np.polyfit(message_sizes2, avg_times2, 1)

latency1_us = c1 * 1e6
peak_bandwidth1_mbs = 1/m1 / 1e6

latency2_us = c2 * 1e6
peak_bandwidth2_mbs = 1/m2 / 1e6

print(f"Run 1 Latency: {latency1_us:.3f} μs")
print(f"Run 1 Peak Bandwidth: {peak_bandwidth1_mbs:.2f} MB/s")
print(f"Run 2 Latency: {latency2_us:.3f} μs")
print(f"Run 2 Peak Bandwidth: {peak_bandwidth2_mbs:.2f} MB/s")

plt.figure(figsize=(10,6))
plt.scatter(message_sizes1, avg_times1, color='green')
plt.plot(message_sizes1, m1*message_sizes1, color='blue', linestyle='-')

plt.scatter(message_sizes2, avg_times2, color='red')
plt.plot(message_sizes2, m2*message_sizes2, color='orange', linestyle='-')

plt.xlabel('Message Size (Bytes, log scale)')
plt.ylabel('Average Time per PingPong (s)')
plt.title('MPI PingPong: Time vs Message Size with Linear Fit')
plt.ylim(-0.00001, 0.0005)
plt.savefig("plot.png")
plt.grid()

plt.show()
