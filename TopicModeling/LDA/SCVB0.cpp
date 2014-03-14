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
//	K = numberOfTopics;
//	W = vocabSize;
//	D = numOfDocs;
//	d = D / 6; //Assuming number of cores = 6
//	C = corpusSize;

//For KOS
	K = 40;
	W = 6906;
	D = 3430;
	d = 3430; //Assuming number of cores = 6
	C = 353160;

//For NYT
//	K = 10;
//	W = 102660;
//	D = 300000;
//	d = 300000; //Assuming number of cores = 6
//	C = 69679427;

	M = 0;

	s = 10;
	tau = 1000;
	kappa = 0.9;
	t = 1;

	alpha = 0.1;
	eta = 0.01;

	rhoPhi = s / pow((tau + t), kappa);
	rhoTheta = s / pow((tau + t), kappa);
}

bool wayToSort(int i, int j) {
	return i > j;
}

void SCVB0::run(MiniBatch miniBatch) {
	vector<Document> docVector = *miniBatch.docVector;
//	cout << "MiniBatchSize: " << miniBatch.M << endl;

	float nPhi[W][K];
	float nTheta[d + 1][K];
	float nz[K];

	memset(nPhi, 0.0, sizeof nPhi);
	memset(nTheta, 0.0, sizeof nTheta);
	memset(nz, 0.0, sizeof nz);

	//For each mini-batch
	M = miniBatch.M;
	Cj = miniBatch.Cj;

	float nPhiHat[W][K];
	float nzHat[K];

	float gamma[W][K];

	memset(nPhiHat, 0, sizeof nPhiHat);
	memset(nzHat, 0, sizeof nzHat);
	memset(gamma, 0, sizeof gamma);

	// This is where original run method starts
	int j = 1;
	for (j = 1; j <= (int) docVector.size(); j++) {
		Document doc = docVector[j];
		for (map<int, int>::iterator iter = doc.termDict.begin();
				iter != doc.termDict.end(); ++iter) {
			int term = iter->first;
			int k = 0;
//#pragma omp parallel for shared(k, j)
			for (k = 0; k < K; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta) / (nz[k] + eta * M))
						* (nTheta[j][k] + alpha);

				nTheta[j][k] = ((pow((1 - rhoTheta), doc.termDict[term])
						* nTheta[j][k])
						+ ((1 - pow((1 - rhoTheta), doc.termDict[term])) * Cj[j]
								* gamma[term][k]));
			}
		}

		for (map<int, int>::iterator iter = doc.termDict.begin();
				iter != doc.termDict.end(); ++iter) {
			int term = iter->first;
			int k = 0;
//#pragma omp parallel for shared(k, j)
			for (k = 0; k < K; k++) {
				gamma[term][k] = ((nPhi[term][k] + eta) / (nz[k] + eta * M))
						* (nTheta[j][k] + alpha);

				nTheta[j][k] = ((pow((1 - rhoTheta), doc.termDict[term])
						* nTheta[j][k])
						+ ((1 - pow((1 - rhoTheta), doc.termDict[term])) * Cj[j]
								* gamma[term][k]));
				nPhiHat[term][k] += nPhiHat[term][k] + C * gamma[term][k];
				nzHat[k] += nzHat[k] + C * gamma[term][k];
			}
		}
	}
//	updateNPhi(nPhiHat, nPhi, rhoPhi);
//	updateNZ(nzHat, nz, rhoPhi, &gamma);
	int k = 0;
	int w = 0;
//#pragma omp parallel for shared(k) private(w)
	for (k = 0; k < K; k++) {
		for (w = 0; w < W; w++) {
			nPhiHat[w][k] = (C * gamma[w][k]) / M;
			nPhi[w][k] = ((1 - rhoPhi) * nPhi[w][k]) + (rhoPhi * nPhiHat[w][k]);
			nzHat[k] = nzHat[k] + gamma[w][k];
		}
		nzHat[k] = (C * nzHat[k]) / M;
		nz[k] = ((1 - rhoPhi) * nz[k]) + (rhoPhi * nzHat[k]);
	}
	for (int a = 1; a < d + 1; ++a) {
		//std::sort(nTheta[a], nTheta[a] + K - 1);
		for (int b = 0; b < K; ++b) {
			cout << nTheta[a][b] << " ";
		}
		cout << endl;
	}
}
