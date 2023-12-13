#include <iostream>
#include <string>
#include <fstream>
#include "../include/parser.h"
using namespace std;

/**
 * main: Roept de Parser-constructor aan en voert de expressie check uit.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (s3522415)
 * @author Reyer Dix (s3333892)
 * @file main.cc
 * @date last change: 12/11/23
**/

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "geen file om vanuit te lezen" << endl;
        return 1;
    } else {
        const char* inputFile = argv[1];

        // Open the input file
        ifstream input(inputFile);

        if (!input.is_open()) {
            cerr << "gefaald om file te openen" << endl;
            return 1;
        } // if
    
	string line;
	getline(input, line);
	Parser temp(line);
    temp.leegVector();
    if(temp.getFout()) return 1; // ontdekte fout
    return 0;
	} // else
} // main