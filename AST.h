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
        Token* parseExpression(string input, int& i);
        Token* parse(const string& input);

    private:
        Token* parseExpression(string input, int& i);
        Token* parseVariable(string input, int& i);
        Token* parseLambda(string input, int& i);
        		//Recursive descent functions
			
		//Step 1 check if there is a expression and if it is there call lexpr and expr1
		Token* expr();
			
		//step 2 check if there is another expression or if it is empty. If not empty call itself and lexpr
		Token* expr1();

		//step 3 check if there is a \ or labda. If there is call var and lexpr. if not call p expr
		Token* lexpr();

		//step 4 check if there is a variable. If not call expr and store depth of the brackets.
		Token* pexpr();

		//check if the var is allowed
		Token* var();
};

#endif