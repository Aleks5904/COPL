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
        bool morePlaces, replaced;
        Token* treeRoot;
        string input;
        vector<Token*> tokens;
        int size;
        int positie;
        int haakje;
        std::vector<Token*> gamma;
        ASTree(string invoer);

        // @function tokenize()
        // @brief Loopt langs een string, maakt van elke char een token en
        // zet het in een vector.
        // @pre: Er bestaat een string om af te lezen.
        // @post: De vector met tokens is gecreeerd.
        void tokenize();

        // @function maakBoom()
        // @brief Roept expr() aan voor het started van het maken van een boom.
        // @return Retourneert true als er een boom gemaakt is (anders false).
        // @pre: Er bestaat een vector gevuld met tokens.
        // @post: De corresponderende boom is gemaakt.
        bool maakBoom();

        // @function printBoom()
        // @brief Print het boom af door het correct langs te gaan.
        // @param ingang: De token waarbij het printen begint.
        // @pre: Er bestaat een correct geconstrueerde boom.
        // @post: De boom is geprint in voor van een string.
        void printBoom(Token* ingang);
        
        // @function postOrder()
        // @brief Gaat door het boom en past beta-reductie toe 
        // @param ingang: De knoop waarin het lopen door het boom gestart wordt.
        // @return Retourneert beta-gereduceerde boom.
        // @pre: Er is een correct geparste boom.
        // @post: De gereduceerde boom is returned.
        Token* postOrder(Token* ingang);	
        
        // @function freeVector()
        // @brief Maakt de vector leeg, die voor de boom constructie zorgt.
        // @pre: Er bestaat een gevulde vector.
        // @post: De vector is leeg.
        void leegVector();

        // @function deleteSubtree()
        // @brief Verwijdert de (sub-)boom met de gegeven ingang.
        // @param ingang: De knoop waarin het verwijderen start.
        // @pre: Er bestaat een correct geconstrueerde boom.
        // @post: De boom is verwijdert.
        void deleteSubtree(Token* ingang);

        void meerPlekken(Token* ingang, std::string variable);

        // @function betaReduction()
        // @brief Voert beta-eductie uit op de gegeven (sub-)boom.
        // @param ingang: De ingang van de gegeven boom.
        // @return Retourneert beta-gereduceerde boom.
        // @pre: Er is een correct geconstrueerde boom doorgegeven.
        // @post: Beta-reductie is uitgevoerd.
        Token* betaReductie(Token* ingang);

    private:
        bool CharInSet(char input, bool lowerCase);

        bool isNUm(char input);

        bool varCheck(bool lowerCase);

        // @function findLambda()
        // @brief Vind een knoop met type Token::SLASH (lambda)
        // voor de gegeven boom
        // @param ingang: De start van de gegeven boom.
        // @return Retourneert de lambda-knoop (als gevonden), anders nullptr.
        // @pre: Er is een correct geconstrueerde boom doorgegeven.
        // @post: er is gezocht voor lambda.
        Token* findLambda(Token* ingang);

        // @function findGivenVar()
        // @brief Zoekt de boom voor een knoop met de gegeven variabele.
        // @param ingang: De start-knoop van de (sub-)boom.
        // @param variable: De variabele waarvoor gezocht wordt.
        // @return Retourneert true als er een knoop gevonden is 
        // met gegeven variabele, ander false.
        // @pre: Er is een correct geconstrueerde boom doorgegeven.
        // @post: De corresponderende boolean is returned.
        bool findGivenVar(Token* ingang, string variabel);

        Token* replaceSubtree(Token* ingang, Token* N, std::string variable);

        // @function copySubtree()
        // @brief Kopieert de structuur van de gegeven boom 
        // vanaf het begin (ingang).
        // @param StartingNode: het knooppunt waarin het recursieve kopiëren begint.
        // @return Retourneert de kopie van de gegeven boom.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De corresponderende boom is returned.
        Token* copySubtree(Token* ingang);

        // @function expr()
        // @brief Checkt of de expressie een correcte lexpressie en expressie` bevat.
        // @param ingang: De knoop die in de pointer-constructie wordt toegevoegd.
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk).  
		Token* expr(Token*  ingang);

        // @function expr1()
        // @brief Checkt of de expressie een correcte expressie bevat of leeg is
        // @param ingang: De knoop die in de pointer-constructie wordt toegevoegd.
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk). 
		Token* expr1(Token*  ingang);

        // @function lexpr()
        // @brief Bouwt de "applicatie en abstractie"-deel van de boom, roept pexpr aan. 
        // @param ingang: De knoop die in de pointer-constructie wordt toegevoegd.
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk). 
		Token* lexpr(Token*  ingang);

        // @function pexpr()
        // @brief Bouwt de "variabele"-deel van de boom, roept expr aan. 
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk).         
		Token* pexpr();

        Token* type();

        Token* type1();

		// @function peek()
        // @brief Checkt wat voor type token op de gegeven positie staat. 
        // @return Returneert de corresponderende token. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De token is terug-gegeven.  
        Token* peek();

        bool isDerivable(Token* root);

        Token* ASTree::getType(Token* root);
        Token* varRule(Token* root);
        Token* applRule(Token* root);
        Token* lambdaRule(Token* root);
        bool treeEq(Token* root1, Token* root2);
}; // ASTree


#endif