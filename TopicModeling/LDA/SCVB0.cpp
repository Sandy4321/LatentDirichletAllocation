/*
 * SCVB0.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: vspathak
 */

#include "SCVB0.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

SCVB0::SCVB0(int iter, int numberOfTopics, int vocabSize, int numOfDocs,
		int corpusSize) {
	iterations = iter;
//	K = numberOfTopics;
//	W = vocabSize;
//	D = numOfDocs;
//	d = D / 6; //Assuming number of cores = 6
//	C = corpusSize;

	K = 10;
	W = 6906;
	D = 10;
	d = 10; //Assuming number of cores = 6
	C = 467714;
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

	memset(nPhi, 0.0, sizeof nPhi);
	memset(nTheta, 0.0, sizeof nTheta);
	memset(nz, 0.1, sizeof nz);
	memset(alpha, 0, sizeof alpha);
	memset(eta, 0, sizeof eta);

	for (int b = 0; b < K; ++b)
		alpha[b] += 0.1;
	for (int b = 0; b < W; ++b)
		eta[b] += 0.01;

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
//			updateGamma(i, j, term, &gamma, &nPhi, &nTheta, nz, eta, alpha);
			double sigmaEta = 0.0;
			for (int a = 0; a < W; a++) {
				double e = eta[a];
				sigmaEta += e;
			}
			for (int k = 0; k < K - 1; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta[term])
						/ (nz[k] + sigmaEta)) * (nTheta[j][k] + alpha[k]);
			}
//			updateNTheta(j, term, nTheta, rhoTheta, gamma);
			for (int k = 0; k < K - 1; k++) {
				nTheta[j][k] = (((1 - rhoTheta) * nTheta[j][k])
						+ (rhoTheta * Cj[j] * gamma[term][k]));
			}
		}

		i = -1;
		for (map<int, int>::iterator iter = doc->termDict.begin();
				iter != doc->termDict.end(); ++iter) {
			int term = iter->first;
			i += 1;
//			updateGamma(i, j, term, gamma, nPhi, nTheta, nz, eta, alpha);
			double sigmaEta = 0.0;
			for (int a = 0; a < W; a++) {
				double e = eta[a];
				sigmaEta += e;
			}
			//cout<<"Sigma Eta: "<<sigmaEta<<endl;
			for (int k = 0; k < K - 1; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta[term])
						/ (nz[k] + sigmaEta)) * (nTheta[j][k] + alpha[k]);
				//cout <<"calc gamma "<<(nz[k] + sigmaEta)<<endl;
			}
//			updateNTheta(j, term, nTheta, rhoTheta, gamma);
			for (int k = 0; k < K - 1; k++) {
				nTheta[j][k] = (((1 - rhoTheta) * nTheta[j][k])
						+ (rhoTheta * Cj[j] * gamma[term][k]));
//			}
//			for (int k = 0; k < K - 1; k++) {
				nPhiHat[term][k] += nPhiHat[term][k] + C * gamma[term][k];
				nzHat[k] += nzHat[k] + C * gamma[term][k];
			}
		}
	}
//	updateNPhi(nPhiHat, nPhi, rhoPhi);
	for (int k = 0; k < K - 1; k++) {
		for (int w = 0; w < W; w++) {
			nPhiHat[w][k] = (C * gamma[w][k]) / M;
			nPhi[w][k] = ((1 - rhoPhi) * nPhi[w][k]) + (rhoPhi * nPhiHat[w][k]);
		}
	}
//	updateNZ(nzHat, nz, rhoPhi, &gamma);
	for (int k = 0; k < K - 1; k++) {
		for (int i = 0; i < W; i++) {
			nzHat[k] = nzHat[k] + gamma[i][k];
		}
		nzHat[k] = (C * nzHat[k]) / M;
		nz[k] = ((1 - rhoPhi) * nz[k]) + (rhoPhi * nzHat[k]);
	}
	for (int a = 0; a < d+1; ++a) {
		for (int b = 0; b < K; ++b) {
			cout << nTheta[a][b] << " ";
		}
		cout << endl;
	}
}
