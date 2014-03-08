/*
 * LDA.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: vspathak
 */

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "LDA.h"

LDA::LDA() {
	numOfDoc = 0;
	numOfTerms = 0;
	numOfWordsInCorpus = 0;

	iterations = 0;
	numOfTopics = 0;
}

LDA::LDA(string file, int iter, int topics) {
	numOfDoc = 0;
	numOfTerms = 0;
	numOfWordsInCorpus = 0;
	fileName = file;

	iterations = iter;
	numOfTopics = topics;
}

LDA::~LDA() {
	// TODO Auto-generated destructor stub
}

void LDA::parseDataFile() {
	ifstream infile(fileName.c_str());

	infile >> numOfDoc;
	infile >> numOfTerms;
	infile >> numOfWordsInCorpus;
	cout << numOfDoc << " " << numOfTerms << " " << numOfWordsInCorpus << "\n";
	cout << "----------------------------------------------\n";
	int docId, wordId, freq;
	while (infile >> docId >> wordId >> freq) {
		cout << docId << " " << wordId << " " << freq << "\n";
		//Document *doc = new Document();
	}
}
LDA *parseCommandLine(int argv, char *argc[]) {
	argv--, argc++;
	if (argv < 3) {
		cerr
				<< "Malformed command. Correct format: fastLDA docword.txt iterations NumOfTopics"
				<< endl;
		exit(1);
	}
	LDA *lda = new LDA(string(argc[0]), atoi(argc[1]), atoi(argc[2]));
	return lda;
}

int main(int argv, char *argc[]) {

	LDA *lda = parseCommandLine(argv, argc);
	lda->parseDataFile();
	return 0;
}
