'''
Created on Mar 4, 2014

@author: vspathak
'''
import numpy


class SCVB0(object):
    
    ''' Implementation of Stochastic CVB0'''
    def initParam(self):
        self.w = 102660
        self.k = 100
        self.d = 1000
        self.nPhi = numpy.zeros((self.w, self.k), int)
        self.nTheta = numpy.zeros((self.d, self.k), int)
        self.nz = numpy.zeros((self.k), int)
        '''for each mini-batch'''
        self.nPhiHat = numpy.zeros((self.w, self.k), int)
        self.nzHat = numpy.zeros((self.k), int)
    
    def run(self, docList):
        for doc in docList:
            for term in doc.termDict :
                self.updateGamma()
                self.updateNTheta()
            for term in doc.termDict :
                self.updateGamma()
                self.updateNTheta()
                for k in range(0, self.k - 1):
                    self.nPhiHat[term][k] += self.nPhiHat[term][k]  # + C * gamma
                    self.nzHat[k] += self.nzHat[k]  # + C * gamma
        self.updateNPhi()
        self.updateNZ()
    
    def updateGamma(self):
        # Equation 5
        return

    def updateNTheta(self):
        # Equation 6
        return

    def updateNPhi(self):
        # Equation 7
        return
    
    def updateNZ(self):
        # Equation 8
        return
            
    def __init__(self):
        self.initParam()
        
