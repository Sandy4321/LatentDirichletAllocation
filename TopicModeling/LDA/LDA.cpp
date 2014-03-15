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


int currK=0;
struct myclass {
  bool operator() (Term i,Term j) { return ((*i.prob)[currK] > (*j.prob)[currK]);}
} myobject;
LDA::LDA(string file, int iter, int topics) {
	numOfDoc = 0;
	numOfTerms = 0;
	numOfWordsInCorpus = 0;
	fileName = file;

	iterations = iter;
	numOfTopics = topics;
}


bool LDA::myCompFunction(Term i, Term j) {
	return ((*i.prob)[currK] > (*j.prob)[currK]);
}

void LDA::parseDataFile() {
	ifstream inputfile(fileName.c_str());

	int nProcessors = omp_get_max_threads();
	omp_set_num_threads(nProcessors);
	int numOfMiniBatches = nProcessors;
	MiniBatch *miniBatches[numOfMiniBatches];
	for (int c = 0; c < numOfMiniBatches; ++c) {
		miniBatches[c] = new MiniBatch();
	}

	inputfile >> numOfDoc;
	inputfile >> numOfTerms;
	inputfile >> numOfWordsInCorpus;
	cout << numOfDoc << " " << numOfTerms << " " << numOfWordsInCorpus << endl;
	cout << "--------------------" << endl;

	int batchSize = (int) numOfDoc / (numOfMiniBatches);
	int a = 0;
#pragma omp parallel for shared(a)
	for (a = 0; a < numOfMiniBatches; ++a) {
		int docId, wordId, freq;
		ifstream infile(fileName.c_str());
		int x, y, z;
		infile >> x;
		infile >> y;
		infile >> z;
		int eof = 0;
		if (!(infile >> docId >> wordId >> freq)) {
			eof = 1;
		}
		int miniBatchStartDoc = (batchSize * a) + 1;
		while (docId != miniBatchStartDoc) {
			infile >> docId >> wordId >> freq;
		}
		cout<<"Minibatch starts at: "<<docId<<" MiniBatch Ends at: "<< docId+ batchSize-1<<endl;
		MiniBatch *nextBatch = miniBatches[a];
		nextBatch->M = 0;
		std::vector<Document> *docVector = nextBatch->docVector;
		while (!eof && (docId < miniBatchStartDoc + batchSize)) {
			intMap termMap;
			int oldDocId = docId;
			int Cj = 0;
			while (docId == oldDocId) {
				termMap[wordId] = freq;
				Cj += freq;
				if (!(infile >> docId >> wordId >> freq)) {
					eof = 1;
					break;
				}
			}
			Document *newDoc = new Document(oldDocId, termMap);
			newDoc->Cj = Cj;
			nextBatch->M += Cj;
			docVector->push_back(*newDoc);
		}
	}
	int b = 0;
	SCVB0 *scvb0 = new SCVB0(iterations, numOfTopics, numOfTerms, numOfDoc,
			numOfWordsInCorpus);
//#pragma omp parallel for shared(b)
	for (b = 0; b < numOfMiniBatches; ++b) {
		MiniBatch *minibatch = miniBatches[b];
		scvb0->run(*minibatch);
	}
	for (int a = 1; a < scvb0->D + 1; ++a) {
		//std::sort(nTheta[a], nTheta[a] + K - 1);
		cout << a << endl;
		for (int b = 0; b < scvb0->K; ++b) {
			cout << scvb0->nTheta[a][b] << " ";
		}
		cout << endl;
	}
	vector<Term> *termVector = new vector<Term>();
	//Reading the Vocab file
	ifstream myVocabFile;
	myVocabFile.open("vocab.kos.txt");
	int wordId=1;
	string word;
	int eof=0;
	while(!eof)
	{
		if (!(myVocabFile >> word)) {
			eof = 1;
			break;
		}
		Term *term = new Term(wordId, word);
		termVector->push_back(*term);
		wordId++;
	}
	cout<<" Term Vector length is "<<termVector->size();
	//Writing the output to the files
	ofstream myFile;
	
	myFile.open("doctopic.txt");	
	// Each document with its topic allocation
	
	for (int a = 1; a < scvb0->D + 1; ++a) {
		myFile <<"Document "<<a<<" : ";
		//std::sort(nTheta[a], nTheta[a] + K - 1);
		for (int b = 0; b < scvb0->K; b++) {
			myFile << scvb0->nTheta[a][b] << " ";
		}
		myFile << endl;
	}
	for (int b = 1; b < (scvb0->W)+1; b++) {
		for (int a = 0; a < scvb0->K; a++) {
			if(b == 6906){
				cout<<"second "<<b<<" "<<a<<" "<< scvb0->nPhi[b][a]<<endl;
			}
			(*termVector)[b].prob->push_back(scvb0->nPhi[b][a]);
		}
	}

	myFile.close();
	myFile.open("");
	myFile.open("topic.txt");
	for (int b = 0; b < scvb0->K; ++b) {
		currK = b;
		std::sort(termVector->begin(), termVector->end(), myobject);
		myFile <<"\n Topic "<<b+1<<" : ";
		for (int a = 1; a < 21; a++) {
			myFile <<" Word Id : " <<(*termVector)[a].wordId<<" "<<(*((*termVector)[a].prob))[b] << "\n";
		}
		myFile << endl;
	}
	myFile.close();
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
