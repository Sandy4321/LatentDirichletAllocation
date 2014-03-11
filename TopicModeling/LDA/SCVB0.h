/*
 * SCVB0.h
 *
 *  Created on: Mar 7, 2014
 *      Author: Varad Pathak, Arul Samuel
 */

#ifndef SCVB0_H_
#define SCVB0_H_

#include <vector>
#include "Document.h"
#include <iostream>

using namespace std;

class SCVB0 {
public:
	int iterations;
	int K; //Number of Topics
	int W; //no of terms in vocab
	int D; //Total no of docs in corpus
	int d; //Number of documents to be processed by each mini-batch
	int C; //Total no of words in corpus
	intMap Cj; // Number of words in jth Doc
	int M; //number of documents in Mini-Batch

	int s;
	int tau;
	double kappa;
	int t;

	double rhoPhi;
	double rhoTheta;

	SCVB0(int iter, int numberOfTopics, int vocabSize, int numOfDocs,
			int corpusSize);
	virtual ~SCVB0();
	void run(vector<Document> docVector);
	/*void updateGamma(int i, int j, int Wij, double *gamma[], double *nPhi[],
			double *nTheta[], double *nz, double *eta, double *alpha);*/
	/*void updateNTheta(int j, int Wij, double **nTheta, double rhoTheta,
			double **gamma);
	void updateNPhi(double **nPhiHat, double *nPhi, double rhoPhi);
	void updateNZ(double *nzHat, double *nz, double rhoPhi,double *gamma[W][K]);*/
};

#endif /* SCVB0_H_ */
