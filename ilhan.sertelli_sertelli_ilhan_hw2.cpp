#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include "AVL.h"
#include "strutils.h"
using namespace std;

struct DocumentItem {
    string documentName;
    int count;

    DocumentItem(const string & name, const int & c): documentName(name), count(c) {}
};

struct WordItem {
    string word;
    AVLSearchTree<string, DocumentItem> docInfo; // Document info is stored in tree

    WordItem(const string& w) : word(w) {}
};

// Parsing the query into words and creating the list of the words
vector<string> parser(string query) {
    vector<string> words;
    int idx = 0;
    while (idx < query.length()) {
        string tempWord = "";
        if (query.at(idx) != ' ') {
            while (idx < query.length() && query.at(idx) != ' ') {
                tempWord += query.at(idx);
                idx++;
            }
        }
        else
            idx++;
        if (tempWord != "") {
            ToLower(tempWord);
            words.push_back(tempWord);
        }
    }
    return words;
}

int main() {
    int numFiles;
    vector<string> fileList; // The list of input files
    cout << "Enter number of input files: ";
    cin >> numFiles;

    // Adding the files into the list
    for (int i = 1; i <= numFiles; i++) {
        string fileName;
        cout << "Enter " << i << ". file name: ";
        cin >> fileName;
        fileList.push_back(fileName);
    }
    // Main tree
    AVLSearchTree<string, WordItem*> searchTree;
    for (int j = 0; j < fileList.size(); j++) {
        ifstream file;
        string word;
        file.open(fileList[j].c_str());
        if (!file.fail()) {
            // Getting all the words one by one
            while (file >> word) {
                ToLower(word); // Making words case-insensitive
                bool isAlpha = true;
                for (char k : word) {
                    // Check if the word consist of only alphabetical characters
                    if (!(('a' <= k && k <= 'z') || ('A' <= k && k <= 'Z'))) {
                        isAlpha = false;
                        break;
                    }
                }
                if (isAlpha) {
                    WordItem * item = new WordItem(word);
                    // Add the word to the tree if it does not exist
                    if (searchTree.find(word) == nullptr) {
                        DocumentItem doc(fileList[j], 1);
                        item->word = word;
                        item->docInfo.insert(fileList[j], doc);
                        searchTree.insert(word, item);
                    }
                        // If the word exists
                    else {
                        if (searchTree.find(word)->key == word) {
                            // Check if the file is already stored in the word node
                            if (searchTree.find(word)->value->docInfo.find(fileList[j]) == nullptr) {
                                DocumentItem doc(fileList[j], 1);
                                searchTree.find(word)->value->docInfo.insert(fileList[j], doc);
                            }
                                // If the file is already in the word node
                            else {
                                // Storing the count of the appearance of the word in the file
                                int newCount = searchTree.find(word)->value->docInfo.find(fileList[j])->value.count;
                                newCount++;
                                // Removing the old count-version and adding the +1 version to the tree (updating)
                                searchTree.find(word)->value->docInfo.remove(fileList[j], searchTree.find(word)->value->docInfo.find(fileList[j])->value);
                                DocumentItem newDoc(fileList[j], newCount);
                                searchTree.find(word)->value->docInfo.insert(fileList[j], newDoc);
                            }
                        }
                    }
                }
            }
        }
        file.close();
        cout << endl << endl;
    }
    string query;
    cout << "Enter queried words in one line: ";
    cin.ignore();
    getline(cin, query);
    // Enter a string query until "ENDOFINPUT" is written
    do {
        vector<string> parsedQuery = parser(query); // A list of words in the input query
        // Remove the word if the first word is "remove"
        if (UpperString(parsedQuery[0]) == "REMOVE") {
            for (int i = 1; i < parsedQuery.size(); i++) {
                searchTree.remove(parsedQuery[i], searchTree.find(parsedQuery[i])->value);
                cout << parsedQuery[i] << " has been REMOVED" << endl;
            }
        }
        else {
            bool none = true; // True -> The query does not exist in none of the files
            for (int i = 0; i < fileList.size(); i++) {
                bool inEach = true; // True -> The query words exist in the file
                for (int j = 0; j < parsedQuery.size(); j++) {
                    // If the word already exists in the tree
                    if (searchTree.find(parsedQuery[j]) != nullptr) {
                        // Checking if the file is stored in the node
                        if (searchTree.find(parsedQuery[j])->value->docInfo.find(fileList[i]) == nullptr)
                            inEach = false;
                    } else
                        inEach = false;
                }
                if (inEach) { // It means that every word in the query exists in at least one file
                    cout << "in Document " << fileList[i] << ", ";
                    for (int k = 0; k < parsedQuery.size(); k++) {
                        if (k != parsedQuery.size() - 1)
                            cout << parsedQuery[k] << " found "
                                 << searchTree.find(parsedQuery[k])->value->docInfo.find(fileList[i])->value.count
                                 << " times, ";
                        else
                            cout << parsedQuery[k] << " found "
                                 << searchTree.find(parsedQuery[k])->value->docInfo.find(fileList[i])->value.count
                                 << " times.";
                    }
                    cout << endl;
                    none = false;
                }
            }
            if (none) { // It means that none of the file contains every word in the query
                cout << "No document contains the given query" << endl;
            }
        }
        cout << endl;
        cout << "Enter queried words in one line: ";
        getline(cin, query);
        cin.clear();
    } while (query != "ENDOFINPUT");

    return 0;
}
