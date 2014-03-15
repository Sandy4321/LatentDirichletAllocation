/*
 * SCVB0.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: Varad Pathak, Arul Samuel
 */

#include "SCVB0.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <algorithm>
#include "MiniBatch.h"

using namespace std;

SCVB0::SCVB0(int iter, int numberOfTopics, int vocabSize, int numOfDocs,
		int corpusSize) {
	iterations = iter;
	K = numberOfTopics;
	W = vocabSize;
	D = numOfDocs;
	C = corpusSize;

	s = 10;
	tau = 1000;
	kappa = 0.9;
	t = 1;

	alpha = 0.1;
	eta = 0.01;

	rhoPhi = s / pow((tau + t), kappa);
	rhoTheta = s / pow((tau + t), kappa);

	nPhi = new double*[W + 1];
	nTheta = new double*[D + 1];
	nz = new double[K];
	memset(nz, 0, sizeof(nz));

	for (int w = 0; w < W + 1; w++) {
		nPhi[w] = new double[K];
		for (int k = 0; k < K; ++k) {
			nPhi[w][k] = ((double) (rand() % (W * K))) / (W * K);
			nz[k] += nPhi[w][k];
		}
	}
	for (int d = 0; d < D + 1; d++) {
		nTheta[d] = new double[K];
		for (int k = 0; k < K; ++k) {
			nTheta[d][k] = ((double) (rand() % (D * K))) / (D * K);
		}
	}
}

SCVB0::~SCVB0() {
	for (int w = 0; w < W + 1; w++) {
		delete[] (nPhi[w]);
	}
	for (int d = 0; d < D + 1; d++) {
		delete[] (nTheta[d]);
	}
	delete[] (nPhi);
	delete[] (nTheta);
	delete[] (nz);
}

void SCVB0::run(MiniBatch miniBatch) {
	vector<Document> docVector = *miniBatch.docVector;
//	cout << "MiniBatchSize: " << miniBatch.M << endl;

	double **nPhiHat = new double*[W + 1];
	double *nzHat = new double[K];
	double **gamma = new double*[W + 1];

	for (int w = 0; w < W + 1; w++) {
		nPhiHat[w] = new double[K];
		gamma[w] = new double[K];
		memset(nPhiHat[w], 0, sizeof(nPhiHat[w]));
		memset(gamma[w], 0, sizeof(gamma[w]));
	}
	memset(nzHat, 0, sizeof(nzHat));

	// This is where original run method starts
	int j = 0;
	for (j = 0; j < (int) docVector.size(); j++) {
		Document doc = docVector[j];
		for (map<int, int>::iterator iter = doc.termDict.begin(); iter != doc.termDict.end(); iter++) {
			int term = iter->first;
			int k = 0;
			for (k = 0; k < K; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta) / (nz[k] + eta * miniBatch.M)) * (nTheta[doc.docId][k] + alpha);

				nTheta[doc.docId][k] = ((pow((1 - rhoTheta), doc.termDict[term]) * nTheta[doc.docId][k])
						+ ((1 - pow((1 - rhoTheta), doc.termDict[term])) * doc.Cj * gamma[term][k]));
			}
		}

		for (map<int, int>::iterator iter = doc.termDict.begin(); iter != doc.termDict.end(); ++iter) {
			int term = iter->first;
			int k = 0;
			for (k = 0; k < K; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta)	/ (nz[k] + eta * miniBatch.M)) * (nTheta[doc.docId][k] + alpha);

				nTheta[doc.docId][k] = ((pow((1 - rhoTheta), doc.termDict[term]) * nTheta[doc.docId][k])
						+ ((1 - pow((1 - rhoTheta), doc.termDict[term])) * doc.Cj * gamma[term][k]));

				nPhiHat[term][k] += nPhiHat[term][k] + (C * gamma[term][k]/ miniBatch.M);
				nzHat[k] += nzHat[k] + (C * gamma[term][k]/ miniBatch.M);
			}
		}
	}

	for (int k = 0; k < K; k++) {
		for (int w = 1; w < W + 1; w++) {
			nPhi[w][k] = ((1 - rhoPhi) * nPhi[w][k]) + (rhoPhi * nPhiHat[w][k]);
		}
		nz[k] = ((1 - rhoPhi) * nz[k]) + (rhoPhi * nzHat[k]);
	}
	for (int w = 0; w < W + 1; w++) {
		delete[](gamma[w]);
		delete[](nPhiHat[w]);
	}
	delete[](nPhiHat);
	delete[](gamma);
	delete[](nzHat);
}
