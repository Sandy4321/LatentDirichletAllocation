/*
 * Document.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: vspathak
 */

#include "Document.h"

Document::Document() {
	docId = 0;
}

Document::Document(int id, intMap termMap) {
	docId = id;
	termDict = termMap;
}

Document::~Document() {
	// TODO Auto-generated destructor stub
}

