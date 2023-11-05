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
    Token* left;
    Token* right;

    enum Type
    {
        APPLICATION,
        LVAR,
        UVAR,
        VARIABELE,
        SLASH,
        HAAKJEOPEN,
        END,
        HAAKJESLUIT
    }
    type;
	string var; // informatie over de token

    // default constructor
    Token(){
        var = "";
        left = nullptr;
        right = nullptr;
    };
    // destructor
    ~Token(){};
}; // Token

#endif