#ifndef PARSER
#define PARSER
#include <iostream>
#include <vector>
#include "token.h"
using namespace std;

/**
 * Parser: Parser checkt de vector van tokens qua syntax
 * door gebruik te maken van LL-grammar.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (s3522415)
 * @author Reyer Dix (s3333892)
 * @file parser.h
 * @date laatste verandering: 26/11/23
 **/

class Parser{
	public:
        // default constructor 
		Parser(){}			  

        // constructor voor het opnemen van input
		Parser(string invoer); 

        // destructor
        ~Parser(){}

        // @function freeVector()
        // @brief Maakt de vector leeg, die de tokens 
        // voor expressie controle bevat.
        // @pre: Er bestaat een gevulde vector.
        // @post: De vector is leeg.    
        void leegVector();

        int getFout(){return fout;}

	private:
        bool fout = false;     // geeft aan of er een fout aangetreden is
        int positie;           // de positie in de token-vector
        int size;              // groote van de token vector
		int haakje;            // teller voor de haakjes
		vector<Token*> tokens; // vector voor het opslaan van individuele tokens
        string input;          // de invoer voor het parser constructor

        // @function print()
        // @brief Print elke token van de vector uit. 
        // @pre: Er is een token met Token-objecten.
        // @post: Voor elke token wordt informatie uitgeprint.  
		void print();

		// @function CharInSet()
        // @brief Controleert of het input een nummer of character is. 
        // @param input: Input die gecontroleerd wordt.
        // @param first: geeft aan of de input op "nummer" of "character" 
        // gecontroleerd moet worden.
        // @return return geeft terug of input een character of nummer is
        // @return (true= het is nummer/char, anders false).
        // @pre: Er is een correcte input doorgegeven.
        // @post: De input is qua type gecontroleerd.  
        bool CharInSet(const char input, const bool first) const;

        // @function tokenize()
        // @brief Slaat elke token met het correcte type en var aan. 
        // @pre: Er bestaat een input string .
        // @post: De input is tokenized in een vector.
        void tokenize();

        // @function expr()
        // @brief Checkt of de expressie een correcte lexpressie en expressie` bevat. 
        // @return Returneert de correctheid van lexpr en expr`. 
        // @return 1 = syntax correct, anders 0.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De expressie is gecontroleerd.  
		int expr();
			
		// @function expr1()
        // @brief Checkt of de expressie een correcte lexpr en expr` 
        // bevat of leeg is. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De expressie` is gecontroleerd.  
		void expr1();

		// @function lexpr()
        // @brief Checkt of de expressie een correcte pexpr / abstractie bevat. 
        // @return Returneert de correctheid van pexpr / abstractie. 
        // @return 1 = syntax correct, anders 0.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De lexpressie is gecontroleerd.  
		int lexpr();

		// @function pexpr()
        // @brief Checkt of de expressie een correcte variabele en expressie bevat. 
        // @return Returneert de correctheid van variabele en expressie. 
        // @return 1 = syntax correct, anders 0.
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De pexpressie is gecontroleerd.  
		int pexpr();
			
		// @function peek()
        // @brief Checkt wat voor type token op de gegevenn positie staat. 
        // @return Returneert de corresponderende token. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De token is returned.  
		Token* peek(); 

}; // Parser

#endif