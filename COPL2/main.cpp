#include <iostream>
#include "AST.h"
using namespace std;



// int main(){
//     string input;
// 	cout << "voer string in" << endl;
// 	getline(cin, input);
// 	// cout << input << endl;
// 	// Token* rootNode = new Token(input);
//     // ASTree* tree = new ASTree(input, rootNode);
//     // cout << "root info: " <<  tree->getRoot() << endl;
//     int i = 0;
//     // tree->parseExpression(input, i);
//     // std::cout << tree->treeRoot->left;
//     return 0;
// }

int main() {
    string expression = "(x y)";
    // string expression = "\\x.\\y.(x z)";
    ASTree parser;
    Token* result = parser.parse(expression);

    if (result) {
        cout << "Parsed Expression: " << result->expression << endl;
        // cout << "Parsed Expression: " << result->left->expression << endl;
        // cout << "Parsed Expression: " << result->right->expression << endl;
        // You can traverse and display the AST here if needed.
    } else {
        cout << "Error parsing input." << endl;
    }

    // Don't forget to free the allocated memory if you have not implemented
    // a destructor in your Token class.

    return 0;
}