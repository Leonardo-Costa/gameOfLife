#Leonardo Loureiro Costa
import sys
import os

from generate_data import generateData
from analyze_data import analyzeData
from funcs import compile


def main():
    data_file = '../../../data/csv/performance_data.csv'

    compile()

    if (len(sys.argv) > 1 and sys.argv[1] == 'force') or not os.path.exists(data_file):
        generateData()
    analyzeData()

if __name__ == '__main__':
    main()
