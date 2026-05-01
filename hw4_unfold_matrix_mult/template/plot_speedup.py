import subprocess
import matplotlib.pyplot as plt

sizes = [256, 512, 1024, 2048]
thread_counts = [1, 2, 4, 8, 16]

times = {}
for n in sizes:
    times[n] = {}
    for t in thread_counts:
        print(f"Running n={n}, t={t}...")
        result = subprocess.run(["./hw4.bin", str(n), str(t), "0"],
                                capture_output=True, text=True)
        times[n][t] = float(result.stdout.strip())

threads = [1, 2, 4, 8, 16]

# write raw results to CSV
with open('results.csv', 'w') as f:
    f.write('n,threads,time_seconds,speedup\n')
    for n, t_times in times.items():
        base = t_times[1]
        for t in threads:
            speedup = base / t_times[t]
            f.write(f'{n},{t},{t_times[t]:.6f},{speedup:.4f}\n')
print("Saved results.csv")

# write pivoted tables to summary CSV
with open('summary.csv', 'w') as f:
    header = 'n,' + ','.join(f't={t}' for t in threads)

    f.write('Execution Time (seconds)\n')
    f.write(header + '\n')
    for n, t_times in times.items():
        row = ','.join(f'{t_times[t]:.4f}' for t in threads)
        f.write(f'{n},{row}\n')

    f.write('\n')

    f.write('Speedup T(1)/T(t)\n')
    f.write(header + '\n')
    for n, t_times in times.items():
        base = t_times[1]
        row = ','.join(f'{base/t_times[t]:.2f}' for t in threads)
        f.write(f'{n},{row}\n')
print("Saved summary.csv")

fig, ax = plt.subplots(figsize=(7, 5))

for n, t_times in times.items():
    base = t_times[1]
    speedups = [base / t_times[t] for t in threads]
    ax.plot(threads, speedups, marker='o', label=f'n={n}')

ax.plot(threads, threads, linestyle='--', color='gray', label='Ideal linear')

ax.set_xlabel('Number of Threads')
ax.set_ylabel('Speedup  T(1) / T(t)')
ax.set_title('Speedup vs Number of Threads')
ax.set_xticks(threads)
ax.legend()
ax.grid(True, linestyle=':', alpha=0.6)

plt.tight_layout()
plt.savefig('speedup_plot.png', dpi=150)
print("Saved speedup_plot.png")
