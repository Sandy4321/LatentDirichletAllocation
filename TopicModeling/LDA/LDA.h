/*
 * LDA.h
 *
 *  Created on: Mar 7, 2014
 *      Author: Varad Pathak, Arul Samuel
 */

#ifndef LDA_H_
#define LDA_H_

#include <omp.h>

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
	void parseDataFile();
};

#endif /* LDA_H_ */
