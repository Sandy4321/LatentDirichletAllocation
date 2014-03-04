import csv
import numpy
import Document
import SCVB0
from token import NEWLINE


f = open("/media/WORK/CS686/Mini-Project/data.txt", 'r')
try:
    numOfDoc = int(f.readline())
    numOfTerms = int(f.readline())
    numOfWordsInCorpus = int(f.readline())
    TDArray = numpy.zeros((102660, 1000), int)
    block = f.readline()
    docList = []
    while (block) != "":
        row = block.split()
        docId = int(row[0])
        termDict = {}
        while (docId == int(row[0])) :
            docId = int(row[0]) 
            wordId = int(row[1])
            freq = int(row[2])
            termDict[wordId] = freq
            block = f.readline()
            if(block == ""):
                break
            row = block.split()
        newDoc = Document.Document(docId, termDict)
        docList.append(newDoc)
    scvb0 = SCVB0.SCVB0()
    scvb0.run(docList)
    
finally:
    f.close()
