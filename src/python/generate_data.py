from funcs import getPerformance, writeCSV

def generateData():
    paths = {
        'openmp': '../c/openmp/main.exe',
        'pthreads': '../c/pthreads/main.exe'
    }

    openmp = getPerformance(paths['openmp'], generations=2000)
    pthreads = getPerformance(paths['pthreads'], generations=2000)

    writeCSV(openmp, pthreads)
