#include <iostream>
using namespace std;

#ifndef token
#define token

/**
 * Token: Token constructs an object to store information
 * about a single Node.
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
        VARIABELE,
        SLASH,
        HAAKJEOPEN,
        END,
        HAAKJESLUIT
    }
    type;
    // union 
	// {
		string var; // informatie over de token
	// };

    // default constructor
    Token(){
        left = nullptr;
        right = nullptr;
    };
    ~Token(){};
}; // Token

#endif