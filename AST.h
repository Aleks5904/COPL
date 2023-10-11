#include <iostream>
using namespace std;
#pragma once
#ifndef AST
#define AST


struct Token
{
    string expressie;
    Token* left;
    Token* right;
    
    Token(string s);

};

class ASTree{
    public:
        Token* treeRoot;
        string input;
        ASTree(const string& i, Token* newRoot);
        ASTree();
        ~ASTree();

        string getRoot();
    private:
};

#endif