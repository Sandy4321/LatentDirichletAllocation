'''
Created on Mar 4, 2014

@author: vspathak
'''
import numpy
import math


class SCVB0(object):
    
    ''' Implementation of Stochastic CVB0'''
    def initParam(self):
        self.w = 12419
        self.k = 10
        self.d = 10
        self.nPhi = numpy.zeros((self.w, self.k), float)
        self.nTheta = numpy.zeros((self.d+1, self.k), float)
        self.nz = numpy.zeros((self.k), float)
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
        self.c = 467714
        self.Cj = {}
        self.M = 0
        '''for each mini-batch'''
        self.nPhiHat = numpy.zeros((self.w, self.k), float)
        self.nzHat = numpy.zeros((self.k), float)
        
        self.gamma = numpy.zeros((self.w, self.k), float)
    
    def run(self, docList):
        self.M = 0
        for doc in docList:
            temp = 0
            for term in doc.termDict :
                temp += doc.termDict[term]
            self.Cj[doc.id] = temp
            self.M += temp
        j = 0
        for doc in docList:
            j += 1
            i = -1
            for term in doc.termDict :
                i += 1
                self.updateGamma(i, j, term)
                self.updateNTheta(i, j, term)
            i = -1
            for term in doc.termDict :
                i += 1
                self.updateGamma(i, j, term)
                self.updateNTheta(i, j, term)
                for k in range(0, self.k - 1):
                    self.nPhiHat[term][k] += self.nPhiHat[term][k] + self.c * self.gamma[term][k]
                    self.nzHat[k] += self.nzHat[k] + self.c * self.gamma[term][k]
        self.updateNPhi()
        self.updateNZ()
        print self.nTheta
        print "\n"
        print self.nPhi[4496] ,self.nPhi[4497]  
    
    def updateGamma(self, i, j, Wij):
        # Equation 5
        sigmaEta = 0.0
        for e in self.eta:
            sigmaEta += e
        for k in range(0, self.k-1):
            self.gamma[Wij][k] = ((self.nPhi[Wij][k] + self.eta[Wij]) / (self.nz[k] + sigmaEta)) * (self.nTheta[j][k] + self.alpha[k])
        return

    def updateNTheta(self, i, j, Wij):
        # Equation 9 instead of Equation 6, we are using "clumping"
        for k in range(0, self.k-1):
            self.nTheta[j][k] = ((1 - self.rhoTheta) * self.nTheta[j][k]) + (self.rhoTheta * self.Cj[j] * self.gamma[Wij][k]) 
        return

    def updateNPhi(self):
        # Equation 7
        for k in range(0, self.k-1):
            for w in range(1, self.w):
                self.nPhiHat[w][k] = (self.c * self.gamma[w][k]) / self.M
                self.nPhi[w][k] = ((1 - self.rhoPhi) * self.nPhi[w][k]) + (self.rhoPhi * self.nPhiHat[w][k]) 
        return
    
    def updateNZ(self):
        # Equation 8
        for k in range(0, self.k-1):
            for w in range(1, self.w):
                self.nzHat[k] += self.gamma[w][k]
            self.nzHat[k] = (self.c * self.nzHat[k]) / self.M
            self.nz[k] = ((1 - self.rhoPhi) * self.nz[k]) + (self.rhoPhi * self.nzHat[k])  
        return
            
    def __init__(self):
        self.initParam()
        
