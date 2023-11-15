#include <iostream>
#include <string>
#include <fstream>
#include "parser.h"
using namespace std;

/**
 * main: Roept de Parser-constructor aan en voert de expressie check uit.
 * @authors  @Aleksandr Petrov (s3459918)
 * @authors  @Reyer Dix (s3459918)
 * @authors  @Nour Hassan (s3612031)
 * @file main.cc
 * @date last change: 12/11/23
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
    
	string line;
	bool checkexpr;
	getline(input, line);
	Parser* temp = new Parser(line);
	checkexpr = temp->checkExpression();
	cout << "check \n" << checkexpr << endl;
	if(checkexpr) cout << "succes" << endl;
	temp->print();
    temp->leegVector();
    delete temp;
    return 0;
	}
}