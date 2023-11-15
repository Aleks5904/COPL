#include <iostream>
using namespace std;

#ifndef token
#define token

/**
 * Token: Token construeert een object om informatie op te slaan
 * over een individuele token.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (code)
 * @author Reyer Dix (code)
 * @file token.h
 * @date laatste verandering: 31/10/23
 **/

struct Token
{
    Token* links;
    Token* rechts;

    enum Type
    {
        APPLICATIE,
        VARIABELE,
        LAMBDA,
        HAAKJEOPEN,
        HAAKJESLUIT,
        END
    }
    type;
		string var; // informatie over de token

    Token(string var, Type tokenType) : links(nullptr), rechts(nullptr), 
        type(tokenType), var(var) {}
        
    // default constructor
    Token() : links(nullptr), rechts(nullptr) {}

    // destructor
    ~Token(){};
}; // Token

#endif