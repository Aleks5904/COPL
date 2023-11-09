#include <iostream>
#include <fstream>
#include "ast.h"
using namespace std;

/**
 * main: Calls the parser to contruct a tree and perform beta-reduction and alfa-conversion.
 * @authors  @Aleksandr Petrov (s3459918)
 * @authors  @Reyer Dix (s3459918)
 * @authors  @Nour Hassan (s3612031)
 * @file main.cc
 * @date last change: 20/10/23
**/


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "no file to read from" << std::endl;
        exit(1);
    } else {
        const char* inputFile = argv[1];

        // Open the input file
        std::ifstream input(inputFile);

        if (!input.is_open()) {
            std::cerr << "Failed to open the input file." << std::endl;
            exit(1);
        }

        
        std::string line;
        std::getline(input, line);
        ASTree parser(line);
        for (int i = 0; i < parser.tokens.size(); i++)
        {
           std::cout << parser.tokens[i]->var << std::endl;
        }
        

        parser.maakBoom();
        // parser.printBoom(parser.treeRoot); 
            

    }

    return 0;
}


