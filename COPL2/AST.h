#include <iostream>
#include <vector>
using namespace std;
// #pragma once
#ifndef AST
#define AST

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
    // Token(Type t, const string& s);

    // default constructor
    Token(){
        left = nullptr;
        right = nullptr;
    };
    ~Token(){};
};


class ASTree{
    public:
        Token* treeRoot;
        string input;
        vector<Token*> tokens;
        int size;
        int positie;
        int haakje;
        // ASTree(const string& i, Token* newRoot);
        ASTree();
        // ~ASTree();

        // string getRoot();
        // Token* parseExpression(string input, int& i);
        void parse(string invoer);
        void tokenize();
        bool maakBoom();
        void printBoom(Token* ingang);
        Token* wrapper(Token* ingnang);	
        void freeVector();
        void deleteSubtree(Token* ingang);
    private:
        Token* startingPoint(Token* ingang);

        bool findAbstraction(Token* ingang);

        Token* alfaConversion(Token* ingang);

        Token* betaReduction(Token* ingang);

        Token* copySubtree(Token* ingang);
		//Step 1 check if there is a expression and if it is there call lexpr and expr1
		Token* expr(Token*  ingang);
			
		//step 2 check if there is another expression or if it is empty. If not empty call itself and lexpr
		Token* expr1(Token*  ingang);

		//step 3 check if there is a \ or labda. If there is call var and lexpr. if not call p expr
		Token* lexpr(Token*  ingang);

		//step 4 check if there is a variable. If not call expr and store depth of the brackets.
		Token* pexpr();

		//check if the var is allowed

        Token* peek();

        

        
};


#endif