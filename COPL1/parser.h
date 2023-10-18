#ifndef PARSER
#define PARSER
#include <iostream>
#include <vector>
#include "token.h"
using namespace std;

/**
 * Parser: Parser checkt de vector van token qua syntax
 * door gebruik te maken van LL-grammar.
 * @Aleksandr Petrov (s3459918)
 * @Nour Hassan (code)
 * @Reyer Dix (code)
 * parser.h
 * datum laatste verandering: 16/10/23
 **/

class Parser{
	public:
		Parser();				// default constructor 
		Parser(string invoer);	// constructor voor het opnemen van input

		// @function checkExpression()
        // @abstract Roept expr() aan voor het start van syntax check. 
        // @return Returneert true alls alles goed uitpakt.
        // @pre: Er is een correcte input doorgegeven.
        // @post: Het input wordt ge-evalueert.  
		bool checkExpression();

        // @function print()
        // @abstract Print elke token van de vector uit. 
        // @pre: Er is een token met Token-objecten.
        // @post: Voor elke token wordt informatie uitgeprint.  
		void print();

	private:
			
        // @function expr()
        // @abstract Checkt of de expressie een correcte lexpressie en expressie` bevat. 
        // @return Returneert de correctheid van lexpressie en expressie`. 
        // @return true = syntax correct, anders false.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De expressie is gecontroleerd.  
		int expr();
			
		// @function expr1()
        // @abstract Checkt of de expressie een correcte expressie bevat of leeg is. 
        // @return Returneert de correctheid van expressie. 
        // @return true = syntax correct, anders false.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De expressie` is gecontroleerd.  
		int expr1();

		// @function lexpr()
        // @abstract Checkt of de expressie een correcte variabele en expressie bevat. 
        // @return Returneert de correctheid van variabele en expressie. 
        // @return true = syntax correct, anders false.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De lexpressie` is gecontroleerd.  
		int lexpr();

		// @function pexpr()
        // @abstract Checkt of de expressie een correcte variabele en expressie bevat. 
        // @return Returneert de correctheid van variabele en expressie. 
        // @return true = syntax correct, anders false.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De pexpressie is gecontroleerd.  
		int pexpr();

		// @function var()
        // @abstract Checkt of de variabele de correcte syntax bevat. 
        // @return Returneert de correctheid van variabele. 
        // @return true = syntax correct, anders false.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De variabele is gecontroleerd.  
		int var();
			
		string input; // de invoer van het parser constructor
		// size_t index; 
		
		int positie; //  de positie in de token-vector
        int size;    //groote van de token vector
		int haakje;  // teller voor de haakjes
		//A vector class of token pointers used to store all the tokens we find.
		vector<Token*> tokens; // vector voor het opslaan van het van individuele tokens

		// @function peek()
        // @abstract Checkt wat voor type token op de gegevenn positie staat. 
        // @return Returneert de corresponderende token. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De token is terug-gegeven.  
		Token* peek(); 

        // @function tokenize()
        // @abstract Slaat elke token met het correcte type aan. 
        // @pre: Er is een string input doorgegeven.
        // @post: De input is doorgegeven in een vector van tokens.
        void tokenize();


}; // Parser

#endif