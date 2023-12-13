#include <iostream>
#include <fstream>
#include "AST.h"
using namespace std;

/**
 * main: Roept de ASTree-constructor aan en voert beta reductie uit.
 * @author @Aleksandr Petrov (s3459918)
 * @author @Reyer Dix (s3333892)
 * @author @Nour Hassan (s3522415)
 * @file main.cc
 * @date last change: 20/10/23
**/


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "geen file om vanuit te lezen" << std::endl;
        return 1;
    } else {
        const char* inputFile = argv[1];

        // open de input file
        std::ifstream input(inputFile);

        if (!input.is_open()) {
            std::cerr << "gefaald om file te openen" << std::endl;
            return 1;
        } // if

        std::string line;
        std::getline(input, line);
        ASTree parser(line);

        // verwijder boom en leeg vector
        parser.deleteSubtree(parser.getTreeRoot());
        parser.leegVector();

        // handel de exit status correct af
        if(parser.getFout()) return 1;
        else if(parser.getOverflow()) return 2;

    } // if

    return 0;
} // main


