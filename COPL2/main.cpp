#include <iostream>
#include "AST.h"
using namespace std;


int main() {
    // string expression = "(x y)";
    string expression = "(x y)"; // works
    std::string line;
    std::getline(std::cin, line);
    // string expression = "(x y)  \\x (y x)";  // doesnt work 
    ASTree parser;
    parser.parse(line);
    for (int i = 0; i < parser.size; i++)
    {
        std::cout << parser.tokens[i]->type;
        std::cout << parser.tokens[i]->var;
        std::cout << std::endl;
    }
    parser.maakBoom();
    parser.printBoom(parser.treeRoot);
    // Token* reduced = parser.wrapper(parser.treeRoot);

    std::cout << std::endl << "result main" << std::endl;
    parser.printBoom(parser.treeRoot);
    std::cout << std::endl << "result main" << std::endl;
    parser.deleteSubtree(parser.treeRoot);
    parser.freeVector();
    return 0;
}

