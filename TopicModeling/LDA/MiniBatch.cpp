/*
 * MiniBatch.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: vspathak
 */

#include "MiniBatch.h"

MiniBatch::MiniBatch(int m, vector<Document> *documentVector) {
	M = m;
	docVector = documentVector;
}

MiniBatch::MiniBatch() {
	M = 0;
	docVector = new vector<Document>();
}

MiniBatch::~MiniBatch() {
	// TODO Auto-generated destructor stub
}

