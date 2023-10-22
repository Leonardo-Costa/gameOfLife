from funcs import readCSV, plotGraph, plotGraphs, createPerformanceTable

def analyzeData():
    openmp, pthreads = readCSV()

    plotGraph(openmp, 'openmp')
    plotGraph(pthreads, 'pthreads')
    plotGraphs(openmp, pthreads)

    createPerformanceTable(openmp)
    createPerformanceTable(pthreads)

