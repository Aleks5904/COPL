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
	int stringSize = input.size();
	std::cout << std::endl;
	std::cout << "input size: " << stringSize << std::endl;
	for (int i = 0; i < stringSize; i++){
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
        else if(input[i] == ':'){ // haakje-sluit
			t->type = Token::DUBBPUNPT;
			t->var = ":";
			tokens.push_back(t);
			std::cout << ": encountered at " << i  << std::endl;
		}
		else if(input[i] == '\\'){ // slash
			t->type = Token::LAMBDA;
			t->var = "\\";
			tokens.push_back(t);	
			std::cout << "\\ encountered at " << i  << std::endl;
		}
        else if(input[i] == '^'){
            t->type = Token::TOO;
            t->var = "^";
            tokens.push_back(t);
            std::cout << "^ encountered at " << i  << std::endl;
        }
        else if(input[i] == '-' && input[i+1] == '>'){
            t->type = Token::ARROW;
            t->var = "->";
            tokens.push_back(t);
            std::cout << "-> encountered at " << i  << std::endl;
        }
		else if(CharInSet(input[i], true)) { // lvar 
            string var = "";
            var += input[i];
            std::cout << "lvar encountered at " << i  << std::endl;
           while ((CharInSet(input[i], true) || isNUm(input[i])) && input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' 
                && input[i+1] != '^' && input[i+1] != '\\' && input[i+1] != ':' && i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t->type = Token::LVAR;
            t->var = var;
            tokens.push_back(t);
		}
        else if(CharInSet(input[i], false)) { // uvar 
            string var = "";
            var += input[i];
            std::cout << "uvar encountered at " << i  << std::endl;
            while ((CharInSet(input[i], false) || isNUm(input[i])) && input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' 
                && input[i+1] != '^' && input[i+1] != '\\' && input[i+1] != ':' && i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t->type = Token::UVAR;
            t->var = var;
            tokens.push_back(t);

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
    Token* huidig = nullptr;
    std::cout << input;
	positie = -1;
	haakje = 0;
    treeRoot = tokens[0];
	treeRoot = expr(nullptr);
    positie++;
    huidig = peek();
    if (huidig->type != Token::DUBBPUNPT)
    {
        std::cerr << "missende :" << std::endl;
        exit(1);
    }
    Token* dubbPunt = new Token(":", Token::DUBBPUNPT);
    dubbPunt->links = treeRoot;
    dubbPunt->rechts = type();
    if (dubbPunt->rechts == nullptr)
    {
        std::cerr << "missende type " << std::endl;
        exit(1);
    }
    treeRoot = dubbPunt;
    return treeRoot;
} // ASTree::checkExpression



Token* ASTree::expr(Token* ingang){
    std::cout << "expr" << std::endl;
    Token* temp = lexpr(ingang);
    if (temp == nullptr) {
        exit(1);
    }
    ingang = expr1(temp);
    return ingang;
} // ASTree::expr

Token* ASTree::expr1(Token* ingang){
    std::cout << "expr1" << std::endl;
    Token* temp = lexpr(ingang);
    if (temp != nullptr) {
        ingang = expr1(temp);
        return ingang;
    }
    return ingang;
} // ASTree::expr1

Token* ASTree::lexpr(Token* ingang){
    std::cout << "lexpr" << std::endl;
    positie++; // ga naar volgende token in vector
    Token* huidig = peek(); // krijg de momentele token uit vector
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        }
        // applicatie aanwezig
        Token* tok = new Token("@" ,Token::APPLICATIE);
        tok->links = ingang;
        tok->rechts = temp;
        return tok;
    }
    else if (huidig->type == Token::LAMBDA)
    {
        // abstractie aanwezig
        Token* lambda = new Token("\\", Token::LAMBDA);
        positie++;
        huidig = peek(); // krijg de momentele token uit vector
        if (huidig->type == Token::LVAR)
        {
           Token* var = new Token(huidig->var, Token::LVAR);
           lambda->links = var;
           positie++;
           huidig = peek();
           if (huidig->type != Token::TOO)
           {
                std::cerr << "missende ^" << std::endl;
                exit(1);
           }
           temp = type();
           if (temp == nullptr)
           {
                std::cerr << "missende type in abstractie" << std::endl;
                exit(1);
           }
           lambda->links->links = temp;
           lambda->rechts = lexpr(nullptr);
           if (lambda->rechts == nullptr)
           {
             std::cerr << "geen expressie in abstractie" << std::endl;
             exit(1);
           }
           lambda->rechts = temp;
           if (ingang == nullptr)
               return lambda;
           
            Token* tok = new Token("@", Token::APPLICATIE);
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
    
} // ASTree::lexpr

Token* ASTree::pexpr() {
    std::cout << "pexpr" << std::endl;
    Token* huidig = peek(); // krijg de momentele token uit vector
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(huidig->type == Token::LVAR){ // lvar aanwezig
        Token* var = new Token(huidig->var, Token::LVAR);
        return var;
    }
    else if(huidig->type == Token::HAAKJEOPEN){ // '('expr')' 
        haakje++;
        Token* temp = expr(nullptr);
        huidig = peek(); // krijg de momentele token uit vector
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
    
} // ASTree::pexpr()

Token* ASTree::type(){
    std::cout << "type" << std::endl;
    positie++;
    Token* t2 = nullptr;
    Token* t = peek();
    if (t->type == Token::UVAR)
    {
        std::cout << "type: " << t->var << std::endl;
        t2 = type1();
        Token* uvar = new Token(t->var, t->type);
        Token* arrow = new Token("->", Token::ARROW);
        if (t2 != nullptr)
        {
            arrow->links = uvar;
            arrow->rechts = t2;
            return arrow;
        }
        return uvar;
    }
    else if(t->type == Token::HAAKJEOPEN){
        t2 = type();
        if (t2 != nullptr)
        {
            positie++;
            t = peek();
            if (t->type != Token::HAAKJESLUIT)
            {
                std::cout << "no closing bracket (type)" << std::endl;
                exit(1);
            }
            Token* t3 = type1();
            if (t3 != nullptr)
            {
                Token* arrow = new Token("->", Token::ARROW);
                arrow->links = t2;
                arrow->rechts = t3;
                return arrow;
            }
            
        }
        else{
            std::cerr << "error (type)" << std::endl;
            exit(1);
        }
    }
    return nullptr;
} // ASTree::type

Token* ASTree::type1(){
    std::cout << "type1" << std::endl;
    positie++;
    Token* t = peek();
    if(t->type == Token::ARROW){
        Token* t2 = type();
        if (t2 != nullptr)
        {
            return t2;
        }
        else{
            std::cerr << "error (type1)" << std::endl;
            exit(1);
        }
        
    }
    else
        return nullptr;
} // ASTree::type1

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

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
    bool toTherechts = false;
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
            std::cout << "toTherechts" << std::endl;
            toTherechts = true;
            if (ingang->rechts->type != Token::VARIABELE)
            {
                std::cout << "extraStap" << std::endl;
                meerPlekken(ingang, deltaX);
                ingang->rechts = replaceSubtree(ingang->rechts, N, deltaX);
                if(morePlaces || !replaced) deleteSubtree(N);
                extraStap = true;
            }
            
        }
        if(!extraStap && !toTherechts){
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
        
        
        if (toTherechts){
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

