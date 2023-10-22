#Leonardo Loureiro Costa
import subprocess
import matplotlib.pyplot as plt
import numpy as np
import csv
import pandas as pd

dataFolder = '../../../data/omp_critical_reduction/'

def runProgram(path, numThreads, generations):
    print(f'runnning {path} with {numThreads} thread(s) for {generations} generations')
    try:
        result = subprocess.run([path, str(numThreads), str(generations)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output_str = result.stdout.strip()

    except FileNotFoundError:
        print(f"The C program '{path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    return float(output_str)

def plotGraph(seconds, name, threads=[1, 2, 4, 8, 16, 32], dataRootFolder=dataFolder+'images/'):
    plt.xlabel('Número de threads')
    plt.ylabel('Segundos')
    plt.title(f'{name}')
    plt.bar(list(range(len(seconds))), seconds, label=name)
    plt.xticks(list(range(len(threads))), [str(thread) for thread in threads])
    
    plt.legend()
    
    plt.savefig(dataRootFolder + f'{name}.png')
    plt.clf()


def plotGraphs(seconds_1, seconds_2, threads=[1, 2, 4, 8, 16, 32], dataRootFolder=dataFolder+'images/'):
    plt.figure(figsize=(12, 5))
    
    plt.subplot(1, 2, 1)
    plt.xlabel('Número de threads')
    plt.ylabel('Tempo de execução em segundos (s)')
    
    x = np.arange(len(threads))
    bar_width = 0.45

    bars1 = plt.bar(x - bar_width/2, seconds_1, bar_width, label='OpenMP')
    bars2 = plt.bar(x + bar_width/2, seconds_2, bar_width, label='Pthreads')
    
    plt.xticks(x, [str(thread) for thread in threads])
    plt.legend()
    
    for bar1, bar2, sec1, sec2 in zip(bars1, bars2, seconds_1, seconds_2):
        plt.text(bar1.get_x() + bar1.get_width()/2, sec1, f'{sec1:.2f} s', ha='center', va='bottom', fontsize=8)
        plt.text(bar2.get_x() + bar2.get_width()/2, sec2, f'{sec2:.2f} s', ha='center', va='bottom', fontsize=8)
    
    plt.subplot(1, 2, 2)
    plt.xlabel('Número de threads')
    plt.ylabel('Speedup')
    
    plt.plot(threads, threads, linestyle='--', color='lightgray', label='Speedup ideal')
    
    speedup_1 = [seconds_1[0] / t for t in seconds_1]
    speedup_2 = [seconds_2[0] / t for t in seconds_2]

    max_speedup = max(max(speedup_1), max(speedup_2))
    plt.ylim(0, 2 * max_speedup)

    plt.plot(threads, speedup_1, marker='o', label='OpenMP Speedup')
    plt.plot(threads, speedup_2, marker='s', label='Pthreads Speedup')
    
    max_y_value = max(max(speedup_1), max(speedup_2))
    max_x_position = threads[speedup_1.index(max(speedup_1))]
    plt.text(max_x_position, max_y_value + 0.3, f'{max_y_value:.2f}', ha='center', va='bottom', fontsize=10)

    plt.xticks(threads)
    plt.yticks(np.arange(0, 1.2 * max_speedup, 1))


    plt.legend()

    plt.tight_layout()
    plt.savefig(dataRootFolder + 'openmp_pthreads.png')
    plt.clf()

def getPerformance(path, threads=[1, 2, 4, 8, 16, 32], generations=20):
    seconds = []

    for i in threads:
        seconds.append(runProgram(path, i, generations))

    return seconds

def writeCSV(openmp, pthreads, output_filename=dataFolder+'csv/performance_data.csv'):
    combined_names = list(zip(openmp, pthreads))

    with open(output_filename, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(['openmp', 'pthreads'])
        csvwriter.writerows(combined_names)

def readCSV(input_filename=dataFolder+'csv/performance_data.csv'):
    openmp = []
    pthreads = []

    with open(input_filename, 'r') as csvfile:
        csvreader = csv.reader(csvfile)

        next(csvreader)

        for row in csvreader:
            openmp_row, pthreads_row= row
            openmp.append(float(openmp_row))
            pthreads.append(float(pthreads_row))

    return openmp, pthreads

def createPerformanceTable(seconds_array, threads=[1, 2, 4, 8, 16, 32], output_folder=dataFolder+''):
    base_time = seconds_array[0]
    speedup = [round(base_time / seconds, 2) for seconds in seconds_array]
    efficiency = [round(s / t, 2) for s, t in zip(speedup, threads)]
    
    data = {
        'threads': threads,
        'execution_time': seconds_array,
        'speedup': speedup,
        'efficiency ': efficiency
    }
    df = pd.DataFrame(data)

    csv_filename = f'{output_folder}csv/performance_table.csv'
    df.to_csv(csv_filename, index=False)

    plt.figure(figsize=(8, 6))
    ax = plt.gca()
    ax.axis('off')
    table = ax.table(cellText=df.values, colLabels=df.columns, cellLoc='center', loc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(12)
    table.scale(1, 1.5)
    plt.savefig(f'{output_folder}images/performance_table.png', bbox_inches='tight')
    plt.clf()

def compile():
    try:        
        openmp_compile_command = f'gcc -fopenmp -o ../c/openmp/main ../c/openmp/main.c ../c/openmp/gameoflife.c'
        subprocess.run(openmp_compile_command)

        pthreads_compile_command = f'gcc -o ../c/pthreads/main ../c/pthreads/main.c -lpthread ../c/pthreads/gameoflife.c'
        subprocess.run(pthreads_compile_command)
    except:
        print('erro ao compilar')
    else:
        print('sucesso ao compilar')