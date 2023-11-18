#include <iostream>
#include <fstream>
#include "ast.h"
using namespace std;

/**
 * main: Roeopt de parser aan voor boom-constructie en tyep-check.
 * @authors  @Aleksandr Petrov (s3459918)
 * @authors  @Reyer Dix (s3459918)
 * @authors  @Nour Hassan (s3612031)
 * @file main.cc
 * @date laatste veranderin: 18/11/23
**/


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "geen file om af te lezen" << std::endl;
        exit(1);
    } // if
    else {
        const char* inputFile = argv[1];

        // Open the input file
        std::ifstream input(inputFile);

        if (!input.is_open()) {
            std::cerr << "gefaald om file te openen" << std::endl;
            exit(1);
        } // if

        std::string line;
        std::getline(input, line); // lees de input van de file af
        ASTree parser(line);
    } // else

    return 0;
} // main


