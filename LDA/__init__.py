import csv
import numpy
from token import NEWLINE


f = open("/media/WORK/CS686/Mini-Project/data.txt", 'r')
try:
    numOfDoc = int(f.readline())
    numOfTerms = int(f.readline())
    numOfWordsInCorpus = int(f.readline())
    TDArray = numpy.zeros((102660, 1000), int)
    block = f.readline()
    while (block) != "":
        row = block.split()
        docId = int(row[0])
        wordId = int(row[1])
        freq = int(row[2])
        TDArray[wordId - 1][docId - 1] += freq
        block = f.readline()
    for i in range(0, 102660):
        for j in range(0, 1000):
            print TDArray[i][j],
        print "\n"
finally:
    f.close()
