'''
Created on Mar 4, 2014

@author: vspathak
'''
import numpy
import math


class SCVB0(object):
    
    ''' Implementation of Stochastic CVB0'''
    def initParam(self):
        self.w = 102660
        self.k = 100
        self.d = 1000
        self.nPhi = numpy.zeros((self.w, self.k), int)
        self.nTheta = numpy.zeros((self.d, self.k), int)
        self.nz = numpy.zeros((self.k), int)
        self.s = 10
        self.tau = 1000
        self.kappa = 0.9
        self.t = 1
        self.rhoPhi = self.s / math.pow((self.tau + self.t), self.kappa)
        self.rhoTheta = self.s / math.pow((self.tau + self.t), self.kappa)
        self.alpha = numpy.empty(self.k)
        self.alpha.fill(0.1)  # 0.1
        self.eta = numpy.empty(self.w)
        self.eta.fill(0.01)  # 0.01
        self.c = 100000000
        '''for each mini-batch'''
        self.nPhiHat = numpy.zeros((self.w, self.k), int)
        self.nzHat = numpy.zeros((self.k), int)
        
        self.gamma = numpy.zeros((self.w, self.k), int)
    
    def run(self, docList):
        j = 0
        for doc in docList:
            j += 1
            i = 0
            for term in doc.termDict :
                i += 1
                self.updateGamma(i, j, term)
                self.updateNTheta(i, j)
            i = 0
            for term in doc.termDict :
                i += 1
                self.updateGamma(i, j, term)
                self.updateNTheta(i, j)
                for k in range(0, self.k - 1):
                    self.nPhiHat[term][k] += self.nPhiHat[term][k] + self.c * self.gamma[term][k]
                    self.nzHat[k] += self.nzHat[k] + self.c * self.gamma[term][k]
        self.updateNPhi()
        self.updateNZ()
    
    def updateGamma(self, i, j, Wij):
        # Equation 5
        sigmaEta = 0.0
        for e in self.eta:
            sigmaEta += e
        for i in range(1, self.k):
            self.gamma[Wij][i] = ((self.nPhi[Wij][i] + self.eta[Wij]) / (self.nz[i] + sigmaEta)) * (self.nTheta[j][i] + self.alpha[i])
        return

    def updateNTheta(self, i, j, Wij):
        # Equation 9 instead of Equation 6, we are using "clumping"
        # Need to use Cj instead of C
        for k in range(1, self.k):
            self.nTheta[j][k] = ((1-self.rhoTheta)*self.nTheta[j][k]) + (self.rhoTheta * self.c * self.gamma[Wij][k]) 
        return

    def updateNPhi(self):
        # Equation 7
        return
    
    def updateNZ(self):
        # Equation 8
        return
            
    def __init__(self):
        self.initParam()
        
