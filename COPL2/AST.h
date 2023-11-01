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
        ASTree(string invoer);

        // @function tokenize()
        // @brief Loopt langs een string, maakt van elke char een token en
        // zet het in een vector.
        // @pre: Er bestaat een string om af te lezen.
        // @post: De vector met tokens is gecreeerd.
        void tokenize();

        // @function maakBoom()
        // @brief Roept expr() aan voor het started van het maken van een boom.
        // @return Returneert true als er een boom gemaakt is (anders false).
        // @pre: Er bestaat een vector gevuld met tokens.
        // @post: De corresponderende boom is gemaakt.
        bool maakBoom();

        // @function printBoom()
        // @brief Print het boom af door het correct langs te gaan.
        // @param ingang: De token waarbij het printen begint.
        // @pre: Er bestaat een correct geconstrueerde boom.
        // @post: De boom is geprint in voor van een string.
        void printBoom(Token* ingang);
        
        void preOrder(Token* ingnang);	
        void freeVector();
        void deleteSubtree(Token* ingang);
        Token* betaReduction(Token* ingang);
    private:
        Token* findLambda(Token* ingang);
        bool findGivenVar(Token* ingang, string variable);
        Token* startingPoint(Token* ingang);
        bool findAbstraction(Token* ingang);
        Token* alfaConversion(Token* ingang);
        Token* copySubtree(Token* ingang);
		Token* expr(Token*  ingang);
		Token* expr1(Token*  ingang);
		Token* lexpr(Token*  ingang);
		Token* pexpr();
        Token* peek();

        

        
}; // ASTree


#endif