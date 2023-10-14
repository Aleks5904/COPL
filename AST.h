#include <iostream>
using namespace std;
#pragma once
#ifndef AST
#define AST


struct Token
{
    string expression;
    Token* left;
    Token* right;
    
    enum Type {
        EXPRESSION,
        VARIABLE,
        LAMBDA,
        OPEN_PAREN,
        CLOSE_PAREN        
    };
    Type type;
    Token(Type t, const string& s);
};

class ASTree{
    public:
        Token* treeRoot;
        string input;
        ASTree(const string& i, Token* newRoot);
        ASTree();
        // ~ASTree();

        string getRoot();
        // Token* parseExpression(string input, int& i);
        Token* parse(const string& input);

    private:
        Token* parseExpression(string input, int& i);
        Token* parseVariable(string input, int& i);
        Token* parseLambda(string input, int& i);
};

#endif