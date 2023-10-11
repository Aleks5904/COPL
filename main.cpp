#include <iostream>
#include "AST.h"
using namespace std;



int main(){
    string input;
	cout << "voer string in" << endl;
	getline(cin, input);
	// cout << input << endl;
	Token* rootNode = new Token(input);
    ASTree* tree = new ASTree(input, rootNode);
    cout << "root info: " <<  tree->getRoot() << endl;
    return 0;
}