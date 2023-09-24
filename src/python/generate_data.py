from funcs import getPerformance, writeCSV

def generateData():
    paths = {
        'openmp': '../c/openmp/main.exe',
        'pthreads': '../c/pthreads/main.exe'
    }

    openmp = getPerformance(paths['openmp'], generations=10)
    pthreads = getPerformance(paths['pthreads'], generations=10)

    writeCSV(openmp, pthreads)
