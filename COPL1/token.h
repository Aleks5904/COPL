#ifndef TOKEN
#define TOKEN
#include <iostream>
using namespace std;

/**
 * Token: Token construeert een object om informatie 
 * op te slaan van een individuele token.
 * @Aleksandr Petrov (s3459918)
 * @Nour Hassan (code)
 * @Reyer Dix (code)
 * token.h
 * datum laatste verandering: 16/10/23
 **/

struct Token{
	enum{
		VARIABELE,
		HAAKJEOPEN,
		HAAKJESLUIT,
		SLASH	
	} type; // type token

	union 
	{
		string var; // informatie over de token
	};

    // default constructor
	Token() : type(VARIABELE), var("") {} 
    
}; // Token

#endif