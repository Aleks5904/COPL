#ifndef TOKEN
#define TOKEN
#include <iostream>
using namespace std;

/**
 * Token: Token construeert een object om informatie op te slaan
 * over een individuele token.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (code)
 * @author Reyer Dix (code)
 * @file token.h
 * @date laatste verandering: 12/11/23
 **/


struct Token{
	enum{
		VARIABELE,
		HAAKJEOPEN,
		HAAKJESLUIT,
		LAMBDA,
		END	
	} type; // type token

	union 
	{
		string var; // informatie over de token
	};

    // default constructor
	Token() : type(VARIABELE), var("") {} 

	// destructor
	~Token(){}
    
}; // Token

#endif