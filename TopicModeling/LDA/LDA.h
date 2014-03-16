/*
 * LDA.h
 *
 *  Created on: Mar 7, 2014
 *      Author: Varad Pathak, Arul Samuel
 */

#ifndef LDA_H_
#define LDA_H_

#include <omp.h>
#include <map>
#include <string>
#include "Term.h"
#include "SCVB0.h"

using namespace std;

class LDA {
public:
	int numOfDoc;
	int numOfTerms;
	int numOfWordsInCorpus;
	int iterations;
	int numOfTopics;
	string fileName;
	LDA(string fileName, int iter, int topics);
	int main(int argv, char *argc[]);
	SCVB0* parseDataFile();
	void printResults(SCVB0* scvb0);
	void executeSCVB0(SCVB0* scvb0);
};

#endif /* LDA_H_ */
