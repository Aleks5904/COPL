#include <iostream>
#include <vector>
#include "token.h"
using namespace std;
#ifndef AST
#define AST

/**
 * Tree: Constructs the tree by calling the parser.
 * @authors  @Aleksandr Petrov (s3459918)
 * @authors  @Reyer Dix (s3459918)
 * @authors  @Nour Hassan (s3612031)
 * @file AST.h
 * @date last change: 25/10/23
 **/

class ASTree{
    public:
        Token* treeRoot;
        string input;
        vector<Token*> tokens;
        int size;
        int positie;
        int haakje;
        ASTree();

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
		Token* expr(Token*  ingang);
		Token* expr1(Token*  ingang);
		Token* lexpr(Token*  ingang);
		Token* pexpr();
        Token* peek();

        

        
}; // ASTree


#endif