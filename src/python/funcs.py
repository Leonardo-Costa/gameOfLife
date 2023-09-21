import subprocess
import matplotlib.pyplot as plt


def runProgram(path, numThreads, generations):
    try:
        result = subprocess.run([path, str(numThreads), str(generations)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        output_str = result.stdout.strip()

    except FileNotFoundError:
        print(f"The C program '{path}' was not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
    return float(output_str)

def plotGraph(dataRootFolder, seconds, name):
    plt.xlabel('Número de threads')
    plt.ylabel('Segundos')
    plt.title(f'{name}')
    plt.bar(list(range(len(seconds))), seconds)
    plt.savefig(dataRootFolder + f'{name}.png')
    plt.clf()

def analyzeData(path, name, threads=[1, 2, 4, 8], dataRootFolder='../../data/', generations=200):
    seconds = []

    for i in threads:
        seconds.append(runProgram(path, i, generations))

    plotGraph(dataRootFolder, seconds, name)
    
    return seconds