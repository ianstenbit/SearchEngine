#ifndef INDEX_INTERFACE_H
#define INDEX_INTERFACE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class IndexInterface{
public:
	virtual ~IndexInterface() {}

	virtual int addDocument(string name, string author, string date) = 0;
	virtual bool addWordForDocument(int documentIndex, string word) = 0;

	virtual vector<string> getDocumentsForWord(string word)= 0;

	virtual bool sortComparator(int, int, vector<string>) = 0;

	virtual bool writeToFile(string) = 0;
	virtual bool readFromFile(string) = 0;

	virtual int numWords() = 0;
	virtual int numDocs() = 0;

	virtual pair<string, string> getAuthorAndTimeForDocNamed(string name) = 0;

private:
	vector<pair<string, int> > documentTitles;
	map<string, pair<string, string> > authorsAndDates;

};

#endif