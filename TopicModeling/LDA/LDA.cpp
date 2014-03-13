/*
 * LDA.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: Varad Pathak, Arul Samuel
 */

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "LDA.h"
#include "Document.h"
#include "SCVB0.h"
#include <omp.h>

LDA::LDA(string file, int iter, int topics) {
	numOfDoc = 0;
	numOfTerms = 0;
	numOfWordsInCorpus = 0;
	fileName = file;

	iterations = iter;
	numOfTopics = topics;
}

void LDA::parseDataFile() {
	ifstream infile(fileName.c_str());

	int nProcessors = omp_get_max_threads();
	omp_set_num_threads(nProcessors);
	int numOfMiniBatches = nProcessors;
	std::vector<Document> *miniBatches[numOfMiniBatches];
	int batchSize = (int) numOfDoc / (numOfMiniBatches);
	for (int c = 0; c < numOfMiniBatches; ++c) {
		miniBatches[c] = new vector<Document>();
	}

	infile >> numOfDoc;
	infile >> numOfTerms;
	infile >> numOfWordsInCorpus;
	cout << numOfDoc << " " << numOfTerms << " " << numOfWordsInCorpus << endl;
	cout << "--------------------" << endl;
	int docId, wordId, freq;
	int eof = 0;
	if (!(infile >> docId >> wordId >> freq)) {
		eof = 1;
	}
	int a = 0;
#pragma omp parallel for shared(a) private(infile)
	for (a = 0; a < numOfMiniBatches; ++a) {
		int miniBatchStartDoc = batchSize * a;
		while (docId != miniBatchStartDoc) {
			infile >> docId >> wordId >> freq;
		}
		std::vector<Document> *docVector = miniBatches[a];
		while (!eof) {
			intMap termMap;
			int oldDocId = docId;
			while (docId == oldDocId) {
				//			cout << docId << " " << wordId << " " << freq << endl;
				termMap[wordId] = freq;
				if (!(infile >> docId >> wordId >> freq)) {
					eof = 1;
					break;
				}
			}
			Document *newDoc = new Document(oldDocId, termMap);
			docVector->push_back(*newDoc);
		}

	}
	int b = 0;
	for (b = 0; b < numOfMiniBatches; ++b) {
		std::vector<Document> *docVector = miniBatches[a];
		for (int i = 0; i < (int) docVector->size(); i++) {
			//Document *D = docVector[i];
		}
//		SCVB0 *scvb0 = new SCVB0(iterations, numOfTopics, numOfTerms, numOfDoc,
//				numOfWordsInCorpus);
//		scvb0->run(*miniBatches[b]);
	}
	//return tEnd;
}
LDA *parseCommandLine(int argv, char *argc[]) {
	argv--, argc++;
	if (argv < 3) {
		cerr
				<< "Malformed command. Correct format: ./fastLDA docword.txt iterations NumOfTopics"
				<< endl;
		exit(1);
	}
	LDA *lda = new LDA(string(argc[0]), atoi(argc[1]), atoi(argc[2]));
	return lda;
}

int main(int argv, char *argc[]) {

	double tStart = omp_get_wtime();
	LDA *lda = parseCommandLine(argv, argc);
	lda->parseDataFile();
	double tEnd = omp_get_wtime();
	printf("Time taken: %.2fs\n", (double) (tEnd - tStart));
	return 0;
}
