#include <iostream>
#include "AST.h"
using namespace std;


int main() {
    // string expression = "(x y)";
    string expression = "x x))"; // works
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
    parser.printBoom(parser.treeRoot);

    return 0;
}

