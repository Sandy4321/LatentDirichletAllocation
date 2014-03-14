/*
 * MiniBatch.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: vspathak
 */

#include "MiniBatch.h"

MiniBatch::MiniBatch(int m, vector<Document> *documentVector,
		intMap docSizes) {
	M = m;
	docVector = documentVector;
	Cj = docSizes;
}

MiniBatch::MiniBatch() {
	M = 0;
	docVector = new vector<Document>();
}

MiniBatch::~MiniBatch() {
	// TODO Auto-generated destructor stub
}

