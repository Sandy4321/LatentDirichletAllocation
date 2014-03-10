/*
 * SCVB0.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: vspathak
 */

#include "SCVB0.h"
#include <math.h>
#include <string.h>

using namespace std;

SCVB0::SCVB0(int iter, int numberOfTopics, int vocabSize, int numOfDocs,
		int corpusSize) {
	iterations = iter;
	K = numberOfTopics;
	W = vocabSize;
	D = numOfDocs;
	d = D / 6; //Assuming number of cores = 6
	C = corpusSize;
	M = 0;

	s = 10;
	tau = 1000;
	kappa = 0.9;
	t = 1;

	rhoPhi = s / pow((tau + t), kappa);
	rhoTheta = s / pow((tau + t), kappa);
}

SCVB0::~SCVB0() {
	// TODO Auto-generated destructor stub
}

void SCVB0::run(vector<Document> docVector) {

	double nPhi[W][K];
	double nTheta[d + 1][K];
	double nz[K];

	double alpha[K];
	double eta[W];

	memset(nPhi, 0, sizeof nPhi);
	memset(nTheta, 0, sizeof nTheta);
	memset(nz, 0, sizeof nz);
	memset(alpha, 0.1, sizeof alpha);
	memset(eta, 0.01, sizeof eta);

	//For each mini-batch
	double nPhiHat[W][K];
	double nzHat[K];

	double gamma[W][K];

	memset(nPhiHat, 0, sizeof nPhiHat);
	memset(nzHat, 0, sizeof nzHat);
	memset(gamma, 0, sizeof gamma);

	// This is where original run method starts
	M = 0;
	for (std::vector<Document>::iterator doc = docVector.begin();
			doc != docVector.end(); ++doc) {
		int temp = 0;
		for (map<int, int>::iterator iter = doc->termDict.begin();
				iter != doc->termDict.end(); ++iter) {
			int term = iter->first;
			temp += doc->termDict[term];
			Cj[doc->docId] = temp;
			M += temp;
		}
	}
	int j = 0;
	for (std::vector<Document>::iterator doc = docVector.begin();
			doc != docVector.end(); ++doc) {
		j += 1;
		int i = -1;
		for (map<int, int>::iterator iter = doc->termDict.begin();
				iter != doc->termDict.end(); ++iter) {
			int term = iter->first;
			i += 1;
			updateGamma(i, j, term, gamma, nPhi, nTheta, nz, eta, alpha);
			updateNTheta(i, j, term);
		}
		i = -1;
		for (map<int, int>::iterator iter = doc->termDict.begin();
				iter != doc->termDict.end(); ++iter) {
			int term = iter->first;
			i += 1;
			updateGamma(i, j, term, gamma, nPhi, nTheta, nz, eta, alpha);
			updateNTheta(i, j, term);
			int k = 0;
			for (k = 0; k < K - 1; k++) {
				nPhiHat[term][k] += nPhiHat[term][k] + C * gamma[term][k];
				nzHat[k] += nzHat[k] + C * gamma[term][k];
			}
		}
	}
	updateNPhi();
	updateNZ();
}

/*Equation 5*/
void SCVB0::updateGamma(int i, int j, int Wij, double *gamma[], double *nPhi[],
		double *nTheta[], double *nz, double *eta, double *alpha) {
	double sigmaEta = 0.0;
	for (int a = 0; a < sizeof(eta) / sizeof(double); a++) {
		double e = eta[a];
		sigmaEta += e;
	}
	for (int k = 0; k < K - 1; k++) {
		gamma[Wij][k] = ((nPhi[Wij][k] + eta[Wij]) / (nz[k] + sigmaEta))
				* (nTheta[j][k] + alpha[k]);
	}
}

/*Equation 9 instead of Equation 6, we are using "clumping"*/
void SCVB0::updateNTheta(int i, int j, int Wij) {

}

/*Equation 7*/
void SCVB0::updateNPhi() {

}

/*Equation 8*/
void SCVB0::updateNZ() {

}
