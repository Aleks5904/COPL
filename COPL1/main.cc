#include <iostream>
#include <string>
#include "parser.h"
using namespace std;

/**
 * main: Vraagt het input aan de user een roept Parser aan.
 * @Aleksandr Petrov (s3459918)
 * @Nour Hassan (code)
 * Reyer Dix (code)
 * main.cc
 * date last change: 16/10/23**/


int main(){
	string input;
	Parser * temp;
	bool checkexpr;
	cout << "voer string in" << endl;
	getline(cin, input);
	cout << input << endl;
	temp = new Parser(input);
	checkexpr = temp->checkExpression();
	cout << "check \n" << checkexpr << endl;
	if(checkexpr) cout << "succes" << endl;
	temp->print();
	return 0;
}//main
