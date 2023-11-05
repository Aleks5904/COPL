#include <iostream>
#include <fstream>
#include "AST.h"
using namespace std;

/**
 * main: Roept de ASTree-constructor aan en voert beta reductie uit.
 * @authors  @Aleksandr Petrov (s3459918)
 * @authors  @Reyer Dix (s3459918)
 * @authors  @Nour Hassan (s3612031)
 * @file main.cc
 * @date last change: 20/10/23
**/


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "geen file om vanuit te lezen" << std::endl;
        return 1;
    } else {
        const char* inputFile = argv[1];

        // Open the input file
        std::ifstream input(inputFile);

        if (!input.is_open()) {
            std::cerr << "gefaald om file te openen" << std::endl;
            return 1;
        }

        std::string line;
        std::getline(input, line);
        ASTree parser(line);
        parser.printBoom(parser.treeRoot); 
        parser.treeRoot = parser.postOrder(parser.treeRoot); 
        std::cout << std::endl << "result main" << std::endl;
        parser.printBoom(parser.treeRoot);
        std::cout << std::endl << "result main" << std::endl;
        parser.deleteSubtree(parser.treeRoot);
        parser.leegVector();
            

    }

    return 0;
}


