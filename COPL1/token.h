#ifndef TOKEN
#define TOKEN
#include <iostream>
using namespace std;

/**
 * Token: Token construeert een object om informatie op te slaan
 * over een individuele token.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (s3522415)
 * @author Reyer Dix (s3333892)
 * @file token.h
 * @date laatste verandering: 20/11/23
 **/


struct Token{
	enum Type{
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
	Token(){} 

	Token(string var, Type tokenType) : type(tokenType), var(var) {}

	// destructor
	~Token(){}
    
}; // Token

#endif