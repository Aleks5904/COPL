#include "ast.h"
#include <iostream>
#include "token.h"
using namespace std;

ASTree::ASTree(string invoer) {
    input = invoer; // de doorgegevn invoer
	tokenize();    
}

bool CharInSet(char input, bool lowerCase){
	if(input >= 'a' && input <= 'z' && lowerCase)
		return true; 
	if(input >= 'A' && input <= 'Z' && !lowerCase)
		return true; 
	return false; // geen char/num
} // CharInSet

bool isNUm(char input){
    if (input >= '0' && input <= '9')
        return true;    
    return false;
}

void ASTree::tokenize(){
	std::cout << input;
	int StringSize = input.size();
	std::cout << std::endl;
	std::cout << "input size: " << StringSize << std::endl;
	for (int i = 0; i < StringSize; i++){
		std::cout << "current i: "  << i << std::endl;
		Token* t = new Token();
		if (input[i] == '('){ // haakje-open
			t->type = Token::HAAKJEOPEN;
			t->var = "(";
			tokens.push_back(t);
			std::cout << "( encountered at " << i << std::endl;
		}
		else if(input[i] == ')'){ // haakje-sluit
			t->type = Token::HAAKJESLUIT;
			t->var = ")";
			tokens.push_back(t);
			std::cout << ") encountered at " << i  << std::endl;
		}
		else if(input[i] == '\\'){ // slash
			t->type = Token::SLASH;
			t->var = "\\";
			tokens.push_back(t);	
			std::cout << "\\ encountered at " << i  << std::endl;
		}
		else if(CharInSet(input[i], true)) { // lvar 
            string var = "";
            var += input[i];
            std::cout << "lvar encountered at " << i  << std::endl;
            while (CharInSet(input[i], true) || isNUm(input[i])){
                var += input[i];
                std::cout << "test1";
                i++;
            }
            t->type = Token::LVAR;
            t->var = var;
		}
        else if(CharInSet(input[i], false)) { // uvar 
            string var = "";
            var += input[i];
            std::cout << "uvar encountered at " << i  << std::endl;
            while (CharInSet(input[i], false) || isNUm(input[i])){
                var += input[i];
                i++;
            }
            t->type = Token::UVAR;
            t->var = var;

		}
		else if(input[i] == ' '){ // ga door als spatie
            delete t;
			std::cout << "' ' encountered at " << i  << std::endl;
			continue;
		}
		// else continue;
	} // for
    Token* end = new Token();
    end->type = Token::END;
    tokens.push_back(end);
    size = tokens.size();
    std::cout << " size: " << size << std::endl;
} // ASTree::tokenize

bool ASTree::maakBoom(){
    std::cout << input;
	positie = -1;
	haakje = 0;
    treeRoot = tokens[0];
	treeRoot = expr(nullptr);
    return treeRoot;
} // ASTree::checkExpression

Token* ASTree::expr(Token* ingang){
    std::cout << "expr" << std::endl;
    std::cout << positie << std::endl;
    Token* temp = lexpr(ingang);
    if (temp == nullptr) {
        std::cout << "lexpr is leeg" << std::endl;
        exit(1);
    }
    ingang = expr1(temp);
    return ingang;
}

Token* ASTree::expr1(Token* ingang){
    std::cout << "expr1" << std::endl;
    std::cout << positie << std::endl;
    Token* temp = lexpr(ingang);
    if (temp != nullptr) {
        ingang = expr1(temp);
        return ingang;
    }
    return ingang;
}

Token* ASTree::lexpr(Token* ingang){
    std::cout << "lexpr" << std::endl;
    std::cout << positie << std::endl;
    positie++;
    Token* huidig = peek();
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cout << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        }
        Token* tok = new Token();
        tok->type = Token::APPLICATION;
        tok->var = "@";
        tok->left = ingang;
        tok->right = temp;
        return tok;
    }
    else if (huidig->type == Token::SLASH)
    {
        Token* lambda = new Token();
        lambda->type = Token::SLASH;
        lambda->var = "\\";
        positie++;
        huidig = peek();
        if (huidig->type == Token::LVAR)
        {
           lambda->left = huidig;
           temp = lexpr(nullptr);
           if (temp == nullptr)
           {
             std::cout << "no expr in lambda abstraction" << std::endl;
             exit(1);
           }
           lambda->right = temp;
           if (ingang == nullptr)
           {
               return lambda;
           }
            // deleteSubtree(lambda);
            Token* tok = new Token();
            tok->type = Token::APPLICATION;
            tok->var = "@";
            tok->left = ingang;
            tok->right = lambda;
            return tok;
           
        }
        else
        {
            std::cout << "no var in lambda abstraction" << std::endl;
            exit(1);
        }
    }
    else
    {
        return nullptr;
    }
}

Token* ASTree::pexpr() {
    std::cout << "pexpr" << std::endl;
    std::cout << positie << std::endl;
    Token* huidig = peek();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cout << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(huidig->type == Token::LVAR) return huidig;
    else if(huidig->type == Token::HAAKJEOPEN){
        haakje++;
        Token* temp = expr(nullptr);
        huidig = peek();
        if(huidig->type == Token::HAAKJESLUIT){
            haakje--;
            return temp;
        }
        else
        {
            std::cout << "geen sluitende haakje" << std::endl;
            deleteSubtree(temp);
            exit(1);
        }
        
    } else{
        return nullptr;
    }
}

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::VARIABELE)
    {
        std::cout <<  ingang->var;
        return;
    }
    else if(ingang->type == Token::SLASH){
         std::cout << "(";
        std::cout << ingang->var;
        printBoom(ingang->left);
        std::cout << " ";
        printBoom(ingang->right);
        std::cout << ")";
    }
    else if(ingang->type == Token::APPLICATION){
        std::cout << "(";
        printBoom(ingang->left);
        std::cout << " ";
        printBoom(ingang->right);
        std::cout << ")";
    }
}

void ASTree::leegVector(){
    for (int i = 0; i < tokens.size(); i++){
        // alleen variabelen staan in de boom
        if (tokens[i]->type != Token::VARIABELE){ 
            delete tokens[i];
            tokens[i] = nullptr; 
        }
    }
} // ASTree::freeVector

void ASTree::deleteSubtree(Token* ingang){
    if (ingang)
    {
        // ga naar linker kind
        if (ingang->left != nullptr) deleteSubtree(ingang->left);
        
        // ga naar rechter kind
        if(ingang->right != nullptr) deleteSubtree(ingang->right);   
    }    
    delete ingang;
    ingang = nullptr;
} // Tree::deleteSubtree

Token* ASTree::copySubtree(Token* ingang) {
    if (!ingang) return nullptr; // lege boom
    
    Token* copy = new Token; // knoop voor copy boom
    copy -> var = ingang -> var; // kopieer de string var
    copy -> type = ingang -> type; // kopieer de type

    if (ingang -> left != nullptr) { // ga naar linker kind
        copy -> left = copySubtree(ingang -> left);
        }

    if (ingang -> right != nullptr) { // ga naar rechter kind
        copy -> right = copySubtree(ingang -> right);
        }
    return copy;
} // Tree::copySubtree

bool ASTree::findGivenVar(Token* ingang, string variabel) {
    if (ingang == nullptr) // lege ingang
        return false; 

    if (ingang->var == variabel) // variabel aanwezig
        return true; 

    bool leftResult = findGivenVar(ingang->left, variabel);
    if (leftResult) // controleer linker kind
        return true; 

    bool rightResult = findGivenVar(ingang->right, variabel);
    if (rightResult) // controleer rechter kind
        return true;

    return false;
}

Token* ASTree::replaceSubtree(Token* ingang, Token* N, std::string variable) {
    if (ingang == nullptr) { // lege knoop
        return nullptr; 
    }
    if (ingang->var == variable) { // gevonden plek voor substitutie
        delete ingang;
        std::cout << "replacing with N" << std::endl;
        if (morePlaces) // meerdere plekken voor substitutie
        {
            std::cout << "more placing possible"<< std::endl;
            Token* nCopy = copySubtree(N);
            ingang = nCopy;
        }
        else ingang = N;
        replaced = true;
        return ingang;
    }
    std::cout << "boom: " << std::endl;
    printBoom(ingang);
    ingang->right = replaceSubtree(ingang->right, N, variable);
    ingang->left = replaceSubtree(ingang->left, N, variable);

    return ingang;  
}

Token* ASTree::findLambda(Token* ingang) {
    if (ingang == nullptr) {
        return nullptr; 
    }

    if (ingang->type == Token::SLASH) {
        return ingang->left;  
    }

    Token* leftResult = findLambda(ingang->left);
    if (leftResult != nullptr) {
        return leftResult; 
    }

    Token* rightResult = findLambda(ingang->right);
    if (rightResult != nullptr) {
        return rightResult; 
    }  
    return nullptr;
}

Token* ASTree::postOrder(Token* ingang) {
    int limit = 0; // houdt aantal beta-reducties bij
    if (ingang == nullptr) 
        return ingang;  
    
    ingang->left = postOrder(ingang->left);
    ingang->right = postOrder(ingang->right);

    // check voor startplek (applicatie+abstractie)
    if (ingang->type == Token::APPLICATION && ingang->left != nullptr
        && ingang->left->type == Token::SLASH) {
        while (ingang->type == Token::APPLICATION && ingang->left != nullptr
            && ingang->left->type == Token::SLASH) {
            limit++;
            if(limit > 1000) exit(2); // over 1000 beta-reducties
            ingang = betaReductie(ingang);
        }
    }

    return ingang;

}

void ASTree::meerPlekken(Token* ingang, std::string variable){
    if (ingang != nullptr)
    {
        if (ingang->left->var == variable 
        && ingang->right->var == variable)
            morePlaces = true;
        else morePlaces = false;
    }
 }

Token* ASTree::betaReductie(Token* ingang){
    morePlaces = false;
    replaced = false;
    std::string deltaX;
    bool extraStap = false;
    std::cout << "beta found" << std::endl;
    printBoom(ingang);
    bool toTheRight = false;
    Token* copy = nullptr;
    Token* ingang2 = nullptr;
    if (ingang != nullptr)
    {
        Token* N = copySubtree(ingang->right);
        deleteSubtree(ingang->right);
        ingang->right = nullptr;
        copy = ingang;
        ingang = ingang->left;
        deltaX = ingang->left->var;
        Token* alfa = findLambda(ingang->right);
        if (alfa != nullptr) // possible bound variable
        {
            bool bound = findGivenVar(N, alfa->var);
            if (bound)
            {
                alfa->var += "'";
            }
        }
        if (ingang->right->type != Token::VARIABELE)
        {
            ingang = ingang->right;
            std::cout << "toTheRight" << std::endl;
            toTheRight = true;
            if (ingang->right->type != Token::VARIABELE)
            {
                std::cout << "extraStap" << std::endl;
                meerPlekken(ingang, deltaX);
                ingang->right = replaceSubtree(ingang->right, N, deltaX);
                if(morePlaces || !replaced) deleteSubtree(N);
                extraStap = true;
            }
            
        }
        if(!extraStap && !toTheRight){
            std::cout << "test0";
            ingang->right = replaceSubtree(ingang->right, N, deltaX);
            if(!replaced) deleteSubtree(N); 
        }
        else if (!extraStap)
        {
            meerPlekken(ingang, deltaX);
            ingang = replaceSubtree(ingang, N, deltaX);
            if(morePlaces) deleteSubtree(N);
            std::cout << "test1" << std::endl;
        }
        
        
        if (toTheRight){
            std::cout << "return #1" << std::endl;
            ingang2 = copySubtree(ingang);
        }

        else {
            std::cout << "return #3" << std::endl;
            ingang2 = copySubtree(ingang->right);
        }
        deleteSubtree(copy);
        ingang = ingang2;
        return ingang;
    }
    return nullptr;

} // ASTree::betaReductie

