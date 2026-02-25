"""
Plot Generator for MPI Performance Analysis
Generates Speedup and Efficiency plots from output.txt files

Run this script from the TP5 directory:
    python plot_performance.py
"""

import matplotlib.pyplot as plt
import numpy as np
import re
import os

def parse_exercise4_output(filepath):
    """Parse Exercise 4 output.txt file to extract performance data."""
    data = {
        'processes': [],
        'serial_time': [],
        'parallel_time': [],
        'speedup': [],
        'efficiency': []
    }
    
    try:
        with open(filepath, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"File not found: {filepath}")
        return None
    
    # Split by each run
    runs = re.split(r'\[.*?\]\$.*?mpirun', content)
    
    for run in runs:
        if not run.strip():
            continue
        
        # Extract number of processes
        np_match = re.search(r'Number of processes:\s*(\d+)', run)
        serial_match = re.search(r'Serial time:\s*([\d.]+)', run)
        parallel_match = re.search(r'Parallel time:\s*([\d.]+)', run)
        speedup_match = re.search(r'Speedup:\s*([\d.]+)', run)
        efficiency_match = re.search(r'Efficiency:\s*([\d.]+)', run)
        
        if all([np_match, serial_match, parallel_match, speedup_match, efficiency_match]):
            data['processes'].append(int(np_match.group(1)))
            data['serial_time'].append(float(serial_match.group(1)))
            data['parallel_time'].append(float(parallel_match.group(1)))
            data['speedup'].append(float(speedup_match.group(1)))
            data['efficiency'].append(float(efficiency_match.group(1)))
    
    return data if data['processes'] else None

def parse_exercise5_output(filepath):
    """Parse Exercise 5 output.txt file to extract performance data."""
    data = {
        'processes': [],
        'serial_time': [],
        'parallel_time': [],
        'speedup': [],
        'efficiency': []
    }
    
    try:
        with open(filepath, 'r') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"File not found: {filepath}")
        return None
    
    if not content.strip():
        print(f"File is empty: {filepath}")
        return None
    
    # Split by separator lines
    runs = re.split(r'={30,}', content)
    
    for run in runs:
        if 'Pi Calculation Results' not in run and 'Number of processes' not in run:
            continue
        
        np_match = re.search(r'Number of processes:\s*(\d+)', run)
        serial_match = re.search(r'Serial time:\s*([\d.]+)', run)
        parallel_match = re.search(r'Parallel time:\s*([\d.]+)', run)
        speedup_match = re.search(r'Speedup:\s*([\d.]+)', run)
        efficiency_match = re.search(r'Efficiency:\s*([\d.]+)', run)
        
        if all([np_match, serial_match, parallel_match, speedup_match, efficiency_match]):
            data['processes'].append(int(np_match.group(1)))
            data['serial_time'].append(float(serial_match.group(1)))
            data['parallel_time'].append(float(parallel_match.group(1)))
            data['speedup'].append(float(speedup_match.group(1)))
            data['efficiency'].append(float(efficiency_match.group(1)))
    
    return data if data['processes'] else None

def plot_speedup_efficiency(data, title, output_filename):
    """Generate speedup and efficiency plots."""
    if data is None:
        print(f"No data available for {title}")
        return
    
    processes = np.array(data['processes'])
    speedup = np.array(data['speedup'])
    efficiency = np.array(data['efficiency'])
    
    # Sort by number of processes
    sort_idx = np.argsort(processes)
    processes = processes[sort_idx]
    speedup = speedup[sort_idx]
    efficiency = efficiency[sort_idx]
    
    fig, axes = plt.subplots(1, 2, figsize=(12, 5))
    
    # Speedup plot
    ax1 = axes[0]
    ax1.plot(processes, speedup, 'bo-', linewidth=2, markersize=8, label='Measured Speedup')
    ax1.plot(processes, processes, 'r--', linewidth=1.5, label='Ideal Speedup (Linear)')
    ax1.set_xlabel('Number of Processes', fontsize=12)
    ax1.set_ylabel('Speedup', fontsize=12)
    ax1.set_title(f'{title} - Speedup', fontsize=14)
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_xticks(processes)
    
    # Efficiency plot
    ax2 = axes[1]
    ax2.plot(processes, efficiency, 'go-', linewidth=2, markersize=8, label='Measured Efficiency')
    ax2.axhline(y=100, color='r', linestyle='--', linewidth=1.5, label='Ideal Efficiency (100%)')
    ax2.set_xlabel('Number of Processes', fontsize=12)
    ax2.set_ylabel('Efficiency (%)', fontsize=12)
    ax2.set_title(f'{title} - Efficiency', fontsize=14)
    ax2.legend()
    ax2.grid(True, alpha=0.3)
    ax2.set_xticks(processes)
    ax2.set_ylim(bottom=0)
    
    plt.tight_layout()
    plt.savefig(output_filename, dpi=150, bbox_inches='tight')
    plt.show()
    print(f"Plot saved to: {output_filename}")

def plot_execution_times(data, title, output_filename):
    """Generate execution time comparison plot."""
    if data is None:
        print(f"No data available for {title}")
        return
    
    processes = np.array(data['processes'])
    serial_time = np.array(data['serial_time'])
    parallel_time = np.array(data['parallel_time'])
    
    # Sort by number of processes
    sort_idx = np.argsort(processes)
    processes = processes[sort_idx]
    serial_time = serial_time[sort_idx]
    parallel_time = parallel_time[sort_idx]
    
    fig, ax = plt.subplots(figsize=(8, 5))
    
    x = np.arange(len(processes))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, serial_time, width, label='Serial Time', color='steelblue')
    bars2 = ax.bar(x + width/2, parallel_time, width, label='Parallel Time', color='coral')
    
    ax.set_xlabel('Number of Processes', fontsize=12)
    ax.set_ylabel('Time (seconds)', fontsize=12)
    ax.set_title(f'{title} - Execution Time Comparison', fontsize=14)
    ax.set_xticks(x)
    ax.set_xticklabels(processes)
    ax.legend()
    ax.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    plt.savefig(output_filename, dpi=150, bbox_inches='tight')
    plt.show()
    print(f"Plot saved to: {output_filename}")

def main():
    """Main function to generate all plots."""
    # Get script directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    # Exercise 4: Matrix-Vector Multiplication
    ex4_path = os.path.join(script_dir, 'Exercice4', 'output.txt')
    ex4_data = parse_exercise4_output(ex4_path)
    
    if ex4_data:
        print("\n=== Exercise 4: Matrix-Vector Multiplication ===")
        print(f"Processes: {ex4_data['processes']}")
        print(f"Speedup: {ex4_data['speedup']}")
        print(f"Efficiency: {ex4_data['efficiency']}")
        
        plot_speedup_efficiency(
            ex4_data, 
            'Matrix-Vector Multiplication (1000x1000)',
            os.path.join(script_dir, 'Exercice4', 'speedup_efficiency_ex4.png')
        )
        
        plot_execution_times(
            ex4_data,
            'Matrix-Vector Multiplication (1000x1000)',
            os.path.join(script_dir, 'Exercice4', 'execution_time_ex4.png')
        )
    else:
        print("No data found for Exercise 4. Make sure Exercice4/output.txt contains valid data.")
    
    # Exercise 5: Pi Calculation
    ex5_path = os.path.join(script_dir, 'Exercice5', 'output.txt')
    ex5_data = parse_exercise5_output(ex5_path)
    
    if ex5_data:
        print("\n=== Exercise 5: Pi Calculation ===")
        print(f"Processes: {ex5_data['processes']}")
        print(f"Speedup: {ex5_data['speedup']}")
        print(f"Efficiency: {ex5_data['efficiency']}")
        
        plot_speedup_efficiency(
            ex5_data,
            'Pi Calculation',
            os.path.join(script_dir, 'Exercice5', 'speedup_efficiency_ex5.png')
        )
        
        plot_execution_times(
            ex5_data,
            'Pi Calculation',
            os.path.join(script_dir, 'Exercice5', 'execution_time_ex5.png')
        )
    else:
        print("\nNo data found for Exercise 5.")
        print("Run the Pi calculation with different process counts and save output:")
        print("  mpirun -np 1 ./pi_calculation 100000000 >> output.txt")
        print("  mpirun -np 2 ./pi_calculation 100000000 >> output.txt")
        print("  mpirun -np 4 ./pi_calculation 100000000 >> output.txt")
        print("  mpirun -np 8 ./pi_calculation 100000000 >> output.txt")

if __name__ == '__main__':
    main()
