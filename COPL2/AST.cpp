#include "AST.h"
#include <iostream>
#include "token.h"
using namespace std;

ASTree::ASTree(string invoer) {
    input = invoer; // de doorgegevn invoer
	tokenize();
    positie = -1;
	haakje = 0;
	treeRoot = expr(nullptr); 
}

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::VARIABELE)
    {
        std::cout <<  ingang->var;
        return;
    }
    else if(ingang->type == Token::LAMBDA){
         std::cout << "(";
        std::cout << ingang->var;
        printBoom(ingang->links);
        std::cout << " ";
        printBoom(ingang->rechts);
        std::cout << ")";
    }
    else if(ingang->type == Token::APPLICATIE){
        std::cout << "(";
        printBoom(ingang->links);
        std::cout << " ";
        printBoom(ingang->rechts);
        std::cout << ")";
    }
}

Token* ASTree::postOrder(Token* ingang) {
    int limit = 0; // houdt aantal beta-reducties bij
    if (ingang == nullptr) 
        return ingang;  
    
    ingang->links = postOrder(ingang->links);
    ingang->rechts = postOrder(ingang->rechts);

    // check voor startplek (applicatie+abstractie)
    if (ingang->type == Token::APPLICATIE && ingang->links != nullptr
        && ingang->links->type == Token::LAMBDA) {
        while (ingang->type == Token::APPLICATIE && ingang->links != nullptr
            && ingang->links->type == Token::LAMBDA) {
            limit++;
            if(limit > 1000) exit(2); // over 1000 beta-reducties
            ingang = betaReductie(ingang);
        }
    }

    return ingang;

}

void ASTree::leegVector(){
    for (int i = 0; i < size; i++){
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
        if (ingang->links != nullptr) deleteSubtree(ingang->links);
        
        // ga naar rechter kind
        if(ingang->rechts != nullptr) deleteSubtree(ingang->rechts);   
    }    
    delete ingang;
    ingang = nullptr;
} // Tree::deleteSubtree

bool ASTree::CharInSet(char input, bool first){
	if(input >= 'a' && input <= 'z')
		return true; // charcter
	if(input >= 'A' && input <= 'Z')
		return true; // character
	if(input >= '0' && input <= '9' && !first)
		return true; // nummer
	return false; // geen char/num
} // CharInSet

void ASTree::tokenize(){
    int stringSize = input.size();
	for (int i = 0; i < stringSize; i++){
		Token* t = new Token();
		if (input[i] == '('){ // haakje-open
			t->type = Token::HAAKJEOPEN;
			t->var = "(";
			tokens.push_back(t);
		}
		else if(input[i] == ')'){ // haakje-sluit
			t->type = Token::HAAKJESLUIT;
			t->var = ")";
			tokens.push_back(t);
		}
		else if(input[i] == '\\'){ // LAMBDA
			t->type = Token::LAMBDA;
			t->var = "\\";
			tokens.push_back(t);	
		}
		else if(input[i] != ' ' && CharInSet(input[i], true)) { // variabele
			string variable = "";
			t->type = Token::VARIABELE;
			variable += input[i];
			while (input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' && input[i+1] != '\\' && i != stringSize-1){
				i++;
				variable += input[i];
			} // check variabele op correctheid en sla aan
			t->var = variable;
			tokens.push_back(t);
		}
		else if(input[i] == ' ' || CharInSet(input[i], false)){ // ga door als spatie
            delete t;
			continue;
		}
	} // for
    Token* end = new Token();
    end->type = Token::END;
    tokens.push_back(end);
    size = tokens.size();
} // ASTree::tokenize

Token* ASTree::findLambda(Token* ingang) {
    if (ingang == nullptr) {
        return nullptr; 
    }

    if (ingang->type == Token::LAMBDA) {
        return ingang->links;  
    }

    Token* linksResult = findLambda(ingang->links);
    if (linksResult != nullptr) {
        return linksResult; 
    }

    Token* rechtsResult = findLambda(ingang->rechts);
    if (rechtsResult != nullptr) {
        return rechtsResult; 
    }  
    return nullptr;
}

bool ASTree::findGivenVar(Token* ingang, string variabel) {
    if (ingang == nullptr) // lege ingang
        return false; 

    if (ingang->var == variabel) // variabel aanwezig
        return true; 

    bool linksResult = findGivenVar(ingang->links, variabel);
    if (linksResult) // controleer linker kind
        return true; 

    bool rechtsResult = findGivenVar(ingang->rechts, variabel);
    if (rechtsResult) // controleer rechter kind
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
    ingang->rechts = replaceSubtree(ingang->rechts, N, variable);
    ingang->links = replaceSubtree(ingang->links, N, variable);

    return ingang;  
}

Token* ASTree::copySubtree(Token* ingang) {
    if (!ingang) return nullptr; // lege boom
    
    Token* copy = new Token; // knoop voor copy boom
    copy -> var = ingang -> var; // kopieer de string var
    copy -> type = ingang -> type; // kopieer de type

    if (ingang -> links != nullptr) { // ga naar linker kind
        copy -> links = copySubtree(ingang -> links);
        }

    if (ingang -> rechts != nullptr) { // ga naar rechter kind
        copy -> rechts = copySubtree(ingang -> rechts);
        }
    return copy;
} // Tree::copySubtree

Token* ASTree::expr(Token* ingang){
    Token* temp = lexpr(ingang);
    if (temp == nullptr) {
        exit(1);
    }
    ingang = expr1(temp);
    return ingang;
}

Token* ASTree::expr1(Token* ingang){
    Token* temp = lexpr(ingang);
    if (temp != nullptr) {
        ingang = expr1(temp);
        return ingang;
    }
    return ingang;
}

Token* ASTree::lexpr(Token* ingang){
    positie++;
    Token* huidig = peek();
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        }
        Token* tok = new Token();
        tok->type = Token::APPLICATIE;
        tok->var = "@";
        tok->links = ingang;
        tok->rechts = temp;
        return tok;
    }
    else if (huidig->type == Token::LAMBDA)
    {
        Token* lambda = new Token();
        lambda->type = Token::LAMBDA;
        lambda->var = "\\";
        positie++;
        huidig = peek();
        if (huidig->type == Token::VARIABELE)
        {
           lambda->links = huidig;
           temp = lexpr(nullptr);
           if (temp == nullptr)
           {
             std::cerr << "geen expressie in abstractie" << std::endl;
             exit(1);
           }
           lambda->rechts = temp;
           if (ingang == nullptr)
               return lambda;
           
            Token* tok = new Token();
            tok->type = Token::APPLICATIE;
            tok->var = "@";
            tok->links = ingang;
            tok->rechts = lambda;
            return tok;
           
        }
        else
        {
            std::cerr << "geen variabele in abstractie" << std::endl;
            exit(1);
        }
    }
    else
        return nullptr;
    
}

Token* ASTree::pexpr() {
    Token* huidig = peek();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(huidig->type == Token::VARIABELE) return huidig;
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
            std::cerr << "geen sluitende haakje" << std::endl;
            deleteSubtree(temp);
            exit(1);
        }
        
    } else
        return nullptr;
    
}

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

void ASTree::meerPlekken(Token* ingang, std::string variable){
    if (ingang != nullptr)
    {
        if (ingang->links->var == variable 
        && ingang->rechts->var == variable)
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
    bool naarRechts = false;
    Token* copy = nullptr;
    Token* ingang2 = nullptr;
    if (ingang != nullptr)
    {
        Token* N = copySubtree(ingang->rechts);
        deleteSubtree(ingang->rechts);
        ingang->rechts = nullptr;
        copy = ingang;
        ingang = ingang->links;
        deltaX = ingang->links->var;
        Token* alfa = findLambda(ingang->rechts);
        if (alfa != nullptr) // possible bound variable
        {
            bool bound = findGivenVar(N, alfa->var);
            if (bound)
            {
                alfa->var += "'";
            }
        }
        if (ingang->rechts->type != Token::VARIABELE)
        {
            ingang = ingang->rechts;
            std::cout << "naarRechts" << std::endl;
            naarRechts = true;
            if (ingang->rechts->type != Token::VARIABELE)
            {
                std::cout << "extraStap" << std::endl;
                meerPlekken(ingang, deltaX);
                ingang->rechts = replaceSubtree(ingang->rechts, N, deltaX);
                if(morePlaces || !replaced) deleteSubtree(N);
                extraStap = true;
            }
            
        }
        if(!extraStap && !naarRechts){
            std::cout << "test0";
            ingang->rechts = replaceSubtree(ingang->rechts, N, deltaX);
            if(!replaced) deleteSubtree(N); 
        }
        else if (!extraStap)
        {
            meerPlekken(ingang, deltaX);
            ingang = replaceSubtree(ingang, N, deltaX);
            if(morePlaces) deleteSubtree(N);
            std::cout << "test1" << std::endl;
        }
        
        
        if (naarRechts){
            std::cout << "return #1" << std::endl;
            ingang2 = copySubtree(ingang);
        }

        else {
            std::cout << "return #3" << std::endl;
            ingang2 = copySubtree(ingang->rechts);
        }
        deleteSubtree(copy);
        ingang = ingang2;
        return ingang;
    }
    return nullptr;

} // ASTree::betaReductie

