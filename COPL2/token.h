#include <iostream>
using namespace std;

#ifndef token
#define token

/**
 * Token: Token construeert een object om informatie op te slaan
 * over een individuele token.
 * @author @Aleksandr Petrov (s3459918)
 * @author @Reyer Dix (s3333892)
 * @author @Nour Hassan (s3522415)
 * @file token.h
 * @date laatste verandering: 22/11/23
 **/

struct Token
{
    Token* links;   // linker kind
    Token* rechts;  // rechter kind

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