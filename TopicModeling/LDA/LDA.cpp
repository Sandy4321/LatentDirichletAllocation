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
#include "MiniBatch.h"
#include "Term.h"
#include <omp.h>
#include <fstream>
#include <algorithm>

#define SUBMISSION false

int currTopic = 0;
struct myclass {
	bool operator()(Term i, Term j) {
		return ((*i.prob)[currTopic] > (*j.prob)[currTopic]);
	}
} myobject;

LDA::LDA(string file, int iter, int topics) {
	numOfDoc = 0;
	numOfTerms = 0;
	numOfWordsInCorpus = 0;
	fileName = file;
	miniBatches = new vector<MiniBatch>();
	iterations = iter;
	numOfTopics = topics;
}

LDA::~LDA() {
	miniBatches->clear();
}

void LDA::printResults(SCVB0* scvb0) {
	cout << "Writing results to file" << endl;
	vector<Term>* termVector = new vector<Term>();
	//Reading the Vocab file
	ifstream myVocabFile;
	myVocabFile.open("vocab.kos.txt");
	int wordId = 1;
	string word;
	int eof = 0;
	while (!eof) {
		if (!(myVocabFile >> word)) {
			eof = 1;
			break;
		}
		Term* term = new Term(wordId, word);
		termVector->push_back(*term);
		wordId++;
	}
	int p = 0;
#pragma omp parallel for shared(p) num_threads(2)
	for (p = 0; p < 2; p++) {
		if (p == 0) {
			ofstream doctopicFile;
			doctopicFile.open("doctopic.txt");
			// Each document with its topic allocation
			for (int d = 1; d < scvb0->D + 1; ++d) {
				double k_total = 0;
				for (int k = 0; k < scvb0->K; k++) {
					k_total += scvb0->nTheta[d][k];
				}
				for (int k = 0; k < scvb0->K - 1; k++) {
					doctopicFile << scvb0->nTheta[d][k] / k_total << ",";
				}
				doctopicFile << scvb0->nTheta[d][scvb0->K - 1] / k_total
						<< endl;
			}
			doctopicFile.close();
		} else {
			for (int k = 0; k < scvb0->K; k++) {
				double k_total = 0;
				for (std::vector<Term>::iterator it = termVector->begin();
						it != termVector->end(); it++) {
					Term term = *it;
					k_total += scvb0->nPhi[term.wordId][k];
				}
				for (std::vector<Term>::iterator it = termVector->begin();
						it != termVector->end(); it++) {
					Term term = *it;
					term.prob->push_back(scvb0->nPhi[term.wordId][k] / k_total);
				}
			}
#if SUBMISSION
			ofstream topicFile;
			topicFile.open("topic.txt");
			int counter = 1;
			for (int k = 0; k < numOfTopics; k++) {
				currTopic = k;
				std::sort(termVector->begin(), termVector->end(), myobject);
				counter = 1;
				for (std::vector<Term>::iterator it = termVector->begin();
						it != termVector->end(); it++) {
					Term term = *it;
					if (counter < 100) {
						topicFile << term.wordId << ":" << (*term.prob)[k]
						<< ",";
					} else if (counter == 100) {
						topicFile << term.wordId << ":" << (*term.prob)[k];
					} else {
						break;
					}
					counter++;
				}
				topicFile << endl;
			}
			topicFile.close();
#else
			ofstream topicFile;
			topicFile.open("topic.txt");
			int counter = 1;
			for (int k = 0; k < numOfTopics; k++) {
				currTopic = k;
				std::sort(termVector->begin(), termVector->end(), myobject);
				topicFile << "Topic " << k + 1 << " : " << endl;
				counter = 1;
				for (std::vector<Term>::iterator it = termVector->begin();
						it != termVector->end(); it++) {
					if (counter < 21) {
						Term term = *it;
						topicFile << term.word << " " << (*term.prob)[k]
								<< endl;
					} else {
						break;
					}
					counter++;
				}
				topicFile << endl;
			}
			topicFile.close();

#endif
		}
	}
}

SCVB0 * LDA::parseDataFile() {
	ifstream inputfile(fileName.c_str());
	inputfile >> numOfDoc;
	inputfile >> numOfTerms;
	inputfile >> numOfWordsInCorpus;
	cout << numOfDoc << " " << numOfTerms << " " << numOfWordsInCorpus << endl;
	cout << "--------------------" << endl;

	inputfile.close();

	SCVB0 *scvb0 = new SCVB0(iterations, numOfTopics, numOfTerms, numOfDoc,
			numOfWordsInCorpus);
	int batchSize = 100;
	int a = 0;
#pragma omp parallel for shared(a)
	for (a = 1; a <= numOfDoc; a += batchSize) {
		int docId, wordId, freq;
		ifstream infile(fileName.c_str());
		//Ignoring first three lines of the file
		int x, y, z;
		infile >> x;
		infile >> y;
		infile >> z;
		int eof = 0;
		if (!(infile >> docId >> wordId >> freq)) {
			eof = 1;
		}
		int miniBatchStartDoc = a;
		while (docId != miniBatchStartDoc) {
			infile >> docId >> wordId >> freq;
		}
		//		cout << "Minibatch: " << docId << " - " << docId + batchSize - 1 << endl;
		MiniBatch* miniBatch = new MiniBatch();
		miniBatch->M = 0;
		std::vector<Document>* docVector = miniBatch->docVector;
		while (!eof && (docId < miniBatchStartDoc + batchSize)) {
			map<int, int>* termMap = new map<int, int>();
			int oldDocId = docId;
			int Cj = 0;
			while (docId == oldDocId) {
				(*termMap)[wordId] = freq;
				Cj += freq;
				if (!(infile >> docId >> wordId >> freq)) {
					eof = 1;
					break;
				}
			}
			Document* newDoc = new Document(oldDocId, *termMap);
			newDoc->Cj = Cj;
			miniBatch->M += Cj;
			docVector->push_back(*newDoc);
		}
		miniBatches->push_back(*miniBatch);
		infile.close();
	}
	return scvb0;
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

	int nProcessors = omp_get_max_threads();
	omp_set_num_threads(nProcessors);

	double tStart = omp_get_wtime();
	LDA *lda = parseCommandLine(argv, argc);
	SCVB0 *scvb0 = lda->parseDataFile();

	for (int itr = 0; itr < lda->iterations; ++itr) {
		cout << "Iteration: " << itr + 1 << endl;
		int m = 0;
#pragma omp parallel for shared(m)
		for (m = 0; m < (int) lda->miniBatches->size(); m++) {
			scvb0->run((*lda->miniBatches)[m]);
		}
	}

	lda->printResults(scvb0);

	delete scvb0;
	delete lda;
	double tEnd = omp_get_wtime();
	cout << "Done." << endl;
	printf("Time taken: %.2fs\n", (double) (tEnd - tStart));
	return 0;
}
