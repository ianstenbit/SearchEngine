#include "XMLParser.h"
#include "porter2_stemmer.h"
#include "rapidxml/rapidxml.hpp"
#include <sstream>

XMLParser::XMLParser(string fileName){
	is.open(fileName);

	cout << "File opened!\n";
}

bool XMLParser::isXMLTag(string token){
   return token.find("<") != string::npos
      || token.find(">") != string::npos
      || token.find("=") != string::npos
      || token.find("-") != string::npos
      || token.find(".") != string::npos
      || token.find(";") != string::npos
      || token.find(":") != string::npos
      || token.find("(") != string::npos
      || token.find(")") != string::npos
      || token.find("/") != string::npos
      || token.find("]") != string::npos
      || token.find("[") != string::npos
      || token.find(",") != string::npos
      || token.find("|") != string::npos
      || token.find("\'") != string::npos
      || token.find("}") != string::npos
      || token.find("{") != string::npos;
}

bool XMLParser::readFile(IndexInterface* index){
	skipIntroPage();
	int num = 0;
	while(!is.eof()){
		num++;
		cout << num << endl;
		writeDocToIndex(index);
	}
	return true;
}

bool XMLParser::skipIntroPage(){

	string token = "";
	while(token != "<page>") is >> token;

	return true;
}

bool XMLParser::readFileToIndex(string fileName, IndexInterface* index){
	rapidxml::xml_document<> doc;
   rapidxml::xml_node<> * root_node;
   // Read the xml file into a vector
   ifstream theFile (fileName);

   cout << "loaded the file\n";

   vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
   buffer.push_back('\0');
   // Parse the buffer using the xml file parsing library into doc 
   doc.parse<0>(&buffer[0]);
   // Find our root node
   root_node = doc.first_node("mediawiki");

   cout << "found root node\n";


   int num = 0;
   for (rapidxml::xml_node<> * document_node = root_node->first_node("page"); document_node; document_node = document_node->next_sibling()){
      rapidxml::xml_node<> * text;
      text = document_node->first_node("revision")->first_node("text");
      string docName = document_node->first_node("title")->value();
      
      if(docName.substr(0, 4) == "User" || docName.substr(0,4) == "File") continue;

      int indexOfDoc = index->addDocument(docName);

      if(text){
         stringstream ss(text->value());
         
         while(!ss.eof()){
         	string word;
         	ss >> word;
            //if(/*!isStopWord(word) */!isXMLTag(word)){
				word[0] = tolower(word[0]);
            	Porter2Stemmer::stem(word);
            	index->addWordForDocument(indexOfDoc, word);
           	}
       }


         cout << num++ << docName << endl;
         

      //if(num > 30) break;
   }

   return true;
}

bool XMLParser::writeDocToIndex(IndexInterface* index){

	string token = "";
	while(token != "<page>" && !is.eof()) is >> token;
	
	if(is.eof()) return false;

	is >> token;

	string docName;
	getline(is, docName);

	docName = token + docName;
	docName = docName.substr(7, docName.size()-15);

	if(docName.substr(0, 4) == "User" || docName.substr(0,4) == "File") return false;

	vector<string> words;

	while(token != "</page>"){
		for(int i = 0; i < token.size(); i++)
			token[i] = tolower(token[i]);
		//if(isNotXMLTag(token) && !isStopWord(token)){
		Porter2Stemmer::stem(token);
		words.push_back(token);
		//}
		is >> token;
	}

	index->addDocument(docName, words);

	return true;
}