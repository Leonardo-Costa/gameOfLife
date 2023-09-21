from funcs import runProgram, plotGraph, analyzeData


def __main__():
    paths = {
        'openmp': '../c/openmp/main.exe',
        'pthreads': '../c/pthreads/main.exe'
    }

    analyzeData(paths['openmp'], 'openmp', generations=10)
    analyzeData(paths['pthreads'], 'pthreads', generations=10)
    
    
if __name__ == '__main__':
    __main__()
