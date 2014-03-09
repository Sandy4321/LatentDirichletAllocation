/*
 * Document.h
 *
 *  Created on: Mar 7, 2014
 *      Author: vspathak
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <map>
#include <string>

using namespace std;

typedef std::map<int,int> intMap;

class Document {
public:
	int docId;
	intMap termDict;
	Document();
	Document(int id, intMap termMap);
	virtual ~Document();
};

#endif /* DOCUMENT_H_ */
