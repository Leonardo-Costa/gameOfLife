#Leonardo Loureiro Costa
from funcs import getPerformance, writeCSV

def generateData():
    paths = {
        'openmp': '../c/openmp/main.exe',
        'pthreads': '../c/pthreads/main.exe'
    }

    openmp = getPerformance(paths['openmp'], generations=20)
    pthreads = getPerformance(paths['pthreads'], generations=20)

    writeCSV(openmp, pthreads)
