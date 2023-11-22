#include <iostream>
#include <vector>
#include "token.h"
using namespace std;
#ifndef AST
#define AST

/**
 * ASTree: Construeert boom door ASTree aan te roepen.
 * @author @Aleksandr Petrov (s3459918)
 * @author @Reyer Dix (s3333892)
 * @author @Nour Hassan (s3522415)
 * @file AST.h
 * @date laatste verandering: 22/11/23
 **/

class ASTree{
    public:
        // default constructor
        ASTree(string invoer);

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

        // getter voor overflow parameter
        bool getOverflow(){return overflow;}

        // getter voor fout parameter
        bool getFout(){return fout;}

        // getter voor treeRoot parameter
        Token* getTreeRoot(){return treeRoot;}
        
    private:
        bool overflow;          // true = over 1000 β-reducties, anders false;
        bool fout;              // geeft aan of er een fout in de expressie zit
        Token* treeRoot;        // de root van de boom
        string input;           // de doorgegeven string
        vector<Token*> tokens;  // vector met tokens
        int size;               // grootte van de vector
        int positie;            // momentele positie in vector
        int haakje;             // teller voor opende- en sluitende haakjes

    	// @function CharInSet()
        // @brief Controleert of het input een nummer of character is. 
        // @param input: Input die gecontroleerd wordt.
        // @param first: geeft aan of de input op "nummer" of "character" 
        // gecontroleerd moet worden.
        // @return return geeft terug of input een character of nummer is
        // @return (true= het is nummer/char, anders false).
        // @pre: Er is een correcte input doorgegeven.
        // @post: De input is qua type gecontroleerd.  
        bool CharInSet(char input, bool first);

        // @function tokenize()
        // @brief Loopt langs een string, maakt van elke char een token en
        // zet het in een vector.
        // @pre: Er bestaat een string om af te lezen.
        // @post: De vector met tokens is gecreeerd.
        void tokenize();

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
        bool findGivenVar(Token* ingang, string variable);

        // @function replaceSubtree()
        // @brief Voert substitutie uit voor gegeven variabel en boom.
        // @param ingang: De start-knoop van de (sub-)boom.
        // @param variable: De variabele waarvoor gezocht wordt.
        // @param N: De (sub-)boom die gesubstitueerd wordt.
        // @param variabel: De variabel die door N vervangen wordt
        // @pre: Er is een correct geconstrueerde boom doorgegeven.
        // @post: Alle substituties zijn uitgevoerd.       
        Token* replaceSubtree(Token* ingang, Token* N, std::string variabel);

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
    
		// @function peek()
        // @brief Checkt wat voor type token op de gegeven positie staat. 
        // @return Returneert de corresponderende token. 
        // @pre: Er bestaat een vector individuele tokens.
        // @post: De token is terug-gegeven.  
        Token* peek();

        // @function betaReduction()
        // @brief Voert beta-eductie uit op de gegeven (sub-)boom.
        // @param ingang: De ingang van de gegeven boom.
        // @return Retourneert beta-gereduceerde boom.
        // @pre: Er is een correct geconstrueerde boom doorgegeven.
        // @post: Beta-reductie is uitgevoerd.
        Token* betaReductie(Token* ingang);
        
}; // ASTree


#endif