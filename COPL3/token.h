#include <iostream>
using namespace std;

#ifndef token
#define token

/**
 * Token: Token construeert een object om informatie op te slaan 
 * over een individuele knoop.
 * @author Aleksandr Petrov (s3459918)
 * @author Nour Hassan (code)
 * @author Reyer Dix (code)
 * @file token.h
 * @date last change: 31/10/23
 **/

struct Token
{
    Token* links = nullptr;
    Token* rechts = nullptr;

    enum Type
    {

        APPLICATIE,  // 0
        VARIABELE,   // 1
        LAMBDA,      // 2
        HAAKJEOPEN,  // 3
        HAAKJESLUIT, // 4
        UVAR,        // 5
        LVAR,        // 6
        ARROW,       // 7
        TOO,         // 8
        JUDG,        // 9
        END          // 10
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