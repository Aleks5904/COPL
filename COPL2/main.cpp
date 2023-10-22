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
    // string expression = "(x y)";
    string expression = "(x y)"; // works
    // string expression = "(x y)  \\x (y x)";  // doesnt work 
    ASTree parser;
    parser.parse(expression);
    for (int i = 0; i < parser.size; i++)
    {
        std::cout << parser.tokens[i]->type;
        std::cout << parser.tokens[i]->var;
        std::cout << std::endl;
    }
    parser.maakBoom();
    
    if (parser.treeRoot != nullptr)
    {
        std::cout << parser.treeRoot->var;
    }
    if (parser.treeRoot->left != nullptr)
    {
        std::cout << parser.treeRoot->left->var;
    }
    if (parser.treeRoot->left->right != nullptr)
    {
        std::cout << parser.treeRoot->left->right->var;
    }
    // if (result) {
        // cout << "Parsed Expression: " << result->left->expression << endl;
        // cout << "Parsed Expression: " << result->right->expression << endl;
        // You can traverse and display the AST here if needed.
    // } else {
        // cout << "Error parsing input." << endl;
    // }

    // Don't forget to free the allocated memory if you have not implemented
    // a destructor in your Token class.

    return 0;
}

