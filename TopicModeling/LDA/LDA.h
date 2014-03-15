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

using namespace std;

typedef std::map<int,string> stringMap;

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
	bool myCompFunction(Term i, Term j);
};

#endif /* LDA_H_ */
