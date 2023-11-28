#include <iostream>
#include <vector>
#include "token.h"
using namespace std;
#ifndef AST
#define AST

/**
 * ASTree: Construeert boom vanuit de string.
 * @author @Aleksandr Petrov (s3459918)
 * @author @Reyer Dix (s3333892)
 * @author @Nour Hassan (s3522415)
 * @file AST.h
 * @date laatste verandering: 26/11/23
 **/

class ASTree{
    public:
        // defalut constructor
        ASTree(const string invoer);

        // getter voor fout parameter
        bool getFout(){return fout;}
        
    private:
        bool fout;                  // geeft aan of er een fout in de expressie zit
        Token* treeRoot;            // de root van de boom
        string input;               // de doorgegeven string
        vector<Token*> tokens;      // vector met tokens
        int size;                   // grootte van de vector
        int positie;                // momentele positie in vector
        int haakje;                 // teller voor opende- en sluitende haakjes
        std::vector<Token*> gamma;  // slaat mini-bomen op voor het opbouwen type

        // @function varCheck()
        // @brief Algemene controle voor de character op positie i.
        // isNum?, CharInSet?, (?, )?...
        // true = lVar, false = uVar
        // @return Retourneert true als input een variabele is, anders false.
        // @pre: Er is correct aangegeven of input lVar/uVar is.
        // @post: input is gecontroleerd.       
        bool varCheck(const int i);

        // @function CharInSet()
        // @brief Controleert of de input een uVar/lVar.
        // @param input: De input die gecontroleerd wordt.
        // @param lowerCase: Geeft aan of de input op uVar/lVar
        // gecontroleerd wordt, true == lvar, false = uVar.
        // @return Returneert true als de input een 
        // lVar/uVar is, ander false. 
        // @pre: -
        // @post: input is gecontroleerd.
        bool CharInSet(const char input, const bool lowerCase);

        // @function isNUm()
        // @brief Controleert of de input een nummer is
        // @param input: De input die gecontroleerd wordt.
        // @return Retourneert true als input = nummer, ander false.
        // @pre: -
        // @post: input is gecontroleerd.     
        bool isNUm(const char input);

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

        // @function type()
        // @brief Controleert Type op <uVar> <Type`>| <'('Type')'> <Type`>.
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk).  
        Token* type();

        // @function type1()
        // @brief Controleert Type` op <'->Type'> | leeg.
        // @return Retourneert (deels-)opgebouwde boom. 
        // @pre: Er bestaat een vector met knopen.
        // @post: De boom is opgebouwd (zover mogelijk).  
        Token* type1();

		// @function peek()
        // @brief Checkt wat voor type token op de gegeven positie staat. 
        // @return Returneert de corresponderende token. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De token is terug-gegeven.  
        Token* peek();

        // @function printBoom()
        // @brief Print het boom af door het correct langs te gaan.
        // @param ingang: De token waarbij het printen begint.
        // @pre: Er bestaat een correct geconstrueerde boom.
        // @post: De boom is geprint in vorm van een string.
        void printBoom(Token* ingang);
                
        // @function freeVector()
        // @brief Maakt de vector leeg, die voor de boom constructie zorgt.
        // @pre: Er bestaat een gevulde vector.
        // @post: De vector is leeg.
        void leegVector();

        // @function leegPtrVector()
        // @brief Maakt de vector leeg van de opgeslagen pointer-bomen, 
        // @param bomen: De vector die wordt leeg gehaald.
        // @pre: Er bestaat een gevulde vector.
        // @post: De vector is leeg.
        void leegPtrVector(std::vector<Token*> & bomen);

        // @function deleteSubtree()
        // @brief Verwijdert de (sub-)boom met de gegeven ingang.
        // @param ingang: De knoop waarin het verwijderen start.
        // @pre: Er bestaat een correct geconstrueerde boom.
        // @post: De boom is verwijdert.
        void deleteSubtree(Token* ingang);

        // @function copySubtree()
        // @brief Kopieert de structuur van de gegeven boom 
        // vanaf het begin (ingang).
        // @param StartingNode: het knooppunt waarin het recursieve kopiëren begint.
        // @return Retourneert de kopie van de gegeven boom.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De corresponderende boom is returned.
        Token* copySubtree(Token* ingang);

        // @function isDerivable()
        // @brief Controleert of de boom van hetzelfde type is.
        // @param judgement: het start van de boom die gecontoleert wordt.
        // @return true als boom van hetzelfde type is, anders false.
        // @pre: Een correct opgebouwde boom is doorgegeven.
        // @post: De boom is gecontroleerd op type.
        bool isDerivable(Token* judgement);
        
        // @function getType()
        // @brief Controleert recursief het type van de boom door
        // drie regels te gebruiken: lambdaRule, applRule, varRule.
        // @param root: het knooppunt waarin het recursieve controle begint.
        // @return returneert (deels-)opgebouwde boom-type.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De type van de boom is returned.       
        Token* getType(Token* root);

        // @function varRule()
        // @brief Controleert of de gegeven Lvar van type T is. 
        // @param Lvar: token die op type wordt gecontroleerd .
        // @return returneert (tot-zover) opgebouwde type.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De type van de Lvar is returned.  
        Token* varRule(Token* Lvar);

        // @function applRule()
        // @brief Controleert applicatie op type en returneert het type daarvan. 
        // @param appl: start van applicatie die gecontroleerd wordt.
        // @return returneert (tot-zover) opgebouwde type van applicatie.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De type van de applicatie is returned.  
        Token* applRule(Token* appl);

        // @function lambdaRule()
        // @brief Controleert abstractie op type en returneert het type daarvan. 
        // @param appl: start van abstractie die gecontroleerd wordt.
        // @return returneert (tot-zover) opgebouwde type van abstractie.
        // @pre: Er is een correct opgebouwde boom.
        // @post: De type van de abstractie is returned.  
        Token* lambdaRule(Token* lambda);

        // @function treeEq()
        // @brief Controleert twee doorgegeven bomen op gelijkheid. 
        // @param boom1: wortel boom 1.
        // @param boom2: wortel boom 2.
        // @pre: Er zijn twee correct opgebouwde bomen doorgegeven.
        // @post: De bomen zijn gecontroleerd op gelijkheid.  
        bool treeEq(Token* boom1, Token* boom2);

}; // ASTree


#endif