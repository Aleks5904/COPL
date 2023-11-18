#include "ast.h"
#include <iostream>
#include "token.h"
using namespace std;

ASTree::ASTree(string invoer) {
    input = invoer; // de doorgegeven invoer
	tokenize();    
    maakBoom();
    if (isDerivable(treeRoot))
        std::cout << "match" << std::endl;
    else {
        std::cerr << "geen match" << std::endl;
        exit(1);
    } // else
    printBoom(treeRoot);
    leegVector();
    deleteSubtree(treeRoot);
} // ASTree::ASTree

bool ASTree::varCheck(int i){
    if ((CharInSet(input[i], true) || isNUm(input[i]) || CharInSet(input[i], false)) 
        && input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' 
        && input[i+1] != '^' && input[i+1] != '\\' && input[i+1] != ':' && input[i+1] != '-')
            return true; 
return false;
} // ASTree::varCheck

bool ASTree::CharInSet(char input, bool lowerCase){
	if(input >= 'a' && input <= 'z' && lowerCase)
		return true; 
	if(input >= 'A' && input <= 'Z' && !lowerCase)
		return true; 
	return false; // geen char/num
} // CharInSet

bool ASTree::isNUm(char input){
    if (input >= '0' && input <= '9')
        return true;    
    return false;
} // ASTree::isNUm

void ASTree::tokenize(){
	std::cout << input;
	int stringSize = input.size();
	for (int i = 0; i < stringSize; i++){
		Token* t = new Token();
		if (input[i] == '('){ // haakje-open
			t->type = Token::HAAKJEOPEN;
			t->var = "(";
			tokens.push_back(t);
		} // if
		else if(input[i] == ')'){ // haakje-sluit
			t->type = Token::HAAKJESLUIT;
			t->var = ")";
			tokens.push_back(t);
		} // elif
        else if(input[i] == ':'){ // haakje-sluit
			t->type = Token::JUDG;
			t->var = ":";
			tokens.push_back(t);
		} // elif
		else if(input[i] == '\\'){ // slash
			t->type = Token::LAMBDA;
			t->var = "\\";
			tokens.push_back(t);	
		} // elif
        else if(input[i] == '^'){ // type
            t->type = Token::TOO;
            t->var = "^";
            tokens.push_back(t);
        } // elif
        else if(input[i] == '-' && input[i+1] == '>'){ // pijl
            t->type = Token::ARROW;
            t->var = "->";
            tokens.push_back(t);
            i++;
        } // elif
		else if(CharInSet(input[i], true)) { // lvar 
            string var = "";
            var += input[i];
            while (varCheck(i) && i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t->type = Token::LVAR;
            t->var = var;
            tokens.push_back(t);
		} // elif
        else if(CharInSet(input[i], false)) { // uvar 
            string var = "";
            var += input[i];
            while (varCheck(i) &&  i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t->type = Token::UVAR;
            t->var = var;
            tokens.push_back(t);

		} // elif
		else if(input[i] == ' '){ // ga door als spatie
            delete t;
			continue;
		} // elif
		// else continue;
	} // for
    Token* end = new Token();
    end->type = Token::END;
    tokens.push_back(end);
    size = tokens.size();
} // ASTree::tokenize

bool ASTree::maakBoom(){
    Token* huidig = nullptr;
	positie = -1, haakje = 0; // start waarden
	treeRoot = expr(nullptr); // start recursive descent
    huidig = peek(); // controleer op judgement
    if (huidig->type != Token::JUDG){
        std::cerr << "missende judgement" << std::endl;
        exit(1);
    } // if
    Token* judg = new Token(":", Token::JUDG);
    judg->links = treeRoot;
    judg->rechts = type(); // maak de rechter deel van de boom
    if (judg->rechts == nullptr){
        std::cerr << "missende type " << std::endl;
        exit(1);
    } // if
    treeRoot = judg;
    return treeRoot;
} // ASTree::checkExpression

Token* ASTree::expr(Token* ingang){
    std::cout << "expr" << std::endl;
    std::cout << "positie: " << positie << std::endl;
    Token* temp = lexpr(ingang); // check voor expressie
    if (temp == nullptr) {
        std::cerr << "expr leeg" << std::endl;
        exit(1);
    } // if
    ingang = expr1(temp); // check voor expressie`
    return ingang;
} // ASTree::expr

Token* ASTree::expr1(Token* ingang){
    std::cout << "expr1" << std::endl;
    std::cout << "positie: " << positie << std::endl;
    Token* temp = lexpr(ingang); // check voor lexpressie
    if (temp != nullptr) {
        ingang = expr1(temp); // check voor expressie`
        return ingang;
    } // if
    else{ 
        std::cout << "returning nullptr (expr1)" << std::endl;
        return ingang;
    } // else
} // ASTree::expr1

Token* ASTree::lexpr(Token* ingang){
    std::cout << "lexpr" << std::endl;
    positie++; // ga naar volgende token in vector
    std::cout << "positie: " << positie << std::endl;
    Token* huidig = peek(); // krijg de momentele token uit vector
    std::cout << "lexpr: " << huidig->var << std::endl;
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    } // if
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        } // if
        std::cout << "applicatie in lexpr" << std::endl;
        // applicatie aanwezig
        Token* tok = new Token("@" ,Token::APPLICATIE);
        tok->links = ingang;
        tok->rechts = temp;
        return tok;
    } // if
    else if (huidig->type == Token::LAMBDA){
        std::cout << "lambda in lexpr" << std::endl;
        // abstractie aanwezig
        Token* lambda = new Token("\\", Token::LAMBDA);
        positie++;
        huidig = peek(); // krijg de momentele token uit vector
        if (huidig->type == Token::LVAR){ // Lvar aanwezig
            std::cout << "lvar in lexpr" << std::endl;
            Token* var = new Token(huidig->var, Token::LVAR);
            lambda->links = var;
            positie++;
            huidig = peek(); // check volgende token in string
           if (huidig->type != Token::TOO){
                std::cerr << "missende ^" << std::endl;
                exit(1);
           } // if
           std::cout << "^ in lexpr" << std::endl; 
           temp = type(); // controleer het type
           if (temp == nullptr){
                std::cerr << "missende type in abstractie" << std::endl;
                exit(1);
           } // if
           lambda->links->links = temp;
           lambda->rechts = lexpr(nullptr);
           if (lambda->rechts == nullptr){
             std::cerr << "geen expressie in abstractie" << std::endl;
             exit(1);
           } // if
           if (ingang == nullptr){
                std::cout << "returning lambda in lexpr" << std::endl;
               return lambda;
           } // if
           else{ // applicatie aanwezig, creeër boom
            Token* tok = new Token("@", Token::APPLICATIE);
            tok->links = ingang;
            tok->rechts = lambda;
            return tok;
           } // else
        } // if
        else{
            std::cerr << "geen variabele in abstractie" << std::endl;
            exit(1);
        } // else
    } // elif
    else{
        std::cout << "returning nullptr (lexpr)" << std::endl;
        return nullptr;
    } // else
} // ASTree::lexpr

Token* ASTree::pexpr() {
    std::cout << "pexpr" << std::endl;
    std::cout << "positie: " << positie << std::endl;
    Token* huidig = peek(); // krijg de momentele token uit vector
    std::cout << "pexpr: " << huidig->var << std::endl;
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        exit(1);
    } // if
    if(huidig->type == Token::LVAR){ // lvar aanwezig
        Token* var = new Token(huidig->var, Token::LVAR);
        std::cout << "lvar in pexpr" << std::endl;
        return var;
    } // if
    else if(huidig->type == Token::HAAKJEOPEN){ // '('expr')' 
        haakje++;
        Token* temp = expr(nullptr);
        huidig = peek(); // krijg de momentele token uit vector
        if(huidig->type == Token::HAAKJESLUIT){
            haakje--;
            return temp;
        } // if
        else{
            std::cerr << "geen sluitende haakje" << std::endl;
            deleteSubtree(temp);
            exit(1);
        } // else
        
    } // elif
    else{
        std::cout << "returning nullptr (pexpr)" << std::endl;
        return nullptr;
    } // else
} // ASTree::pexpr()

Token* ASTree::type(){
    std::cout << "type" << std::endl;
    positie++;
    std::cout << "positie: " << positie << std::endl;
    Token* t2 = nullptr;
    Token* t = peek(); // krijg de momentele token uit vector
    std::cout << "type: " << t->var << std::endl;
    if (t->type == Token::UVAR)
    {
        std::cout << "UVAR in type: " << t->var << std::endl;
        t2 = type1(); // check op < ->Type | ε >
        Token* uvar = new Token(t->var, t->type);
        if (t2 != nullptr) // ->T  aanwezig, creeër boom
        {
            std::cout << "type niet leeg" << std::endl;
            Token* arrow = new Token("->", Token::ARROW);
            arrow->links = uvar;
            arrow->rechts = t2;
            return arrow;
        } // if
        std::cout << "return uvar (type)" << std::endl;
        return uvar;
    } // if
    else if(t->type == Token::HAAKJEOPEN){ // <'('Type')'>
        t2 = type();
        if (t2 != nullptr)
        {
            std::cout << "t2 != nullptr in type" << std::endl;
            positie++;
            t = peek();
            if (t->type != Token::HAAKJESLUIT)
            {
                std::cout << "no closing bracket (type)" << std::endl;
                exit(1);
            } // if
            Token* t3 = type1();  // check op < ->Type | ε >
            if (t3 != nullptr) 
            {
                std::cout << "arrow abstractie (type)" << std::endl;
                Token* arrow = new Token("->", Token::ARROW);
                arrow->links = t2;
                arrow->rechts = t3;
                return arrow;
            } // if
            else return t2;
            
        } // if
        else{
            std::cerr << "error (type)" << std::endl;
            exit(1);
        } // else
    } // elif
    return nullptr;
} // ASTree::type

Token* ASTree::type1(){
    std::cout << "type1" << std::endl;
    positie++;
    std::cout << "positie: " << positie << std::endl;
    Token* t = peek(); // krijg de momentele token uit vector
    std::cout << "type1: " << t->var << std::endl;
    if(t->type == Token::ARROW){ // ->Type
        Token* t2 = type();
        if (t2 != nullptr)
        {
            std::cout << "returning t2 in type1: " <<  t2->var << std::endl;
            return t2;
        } // if
        else{
            std::cerr << "error (type1)" << std::endl;
            exit(1);
        } // else
        
    } // if
    else{ // leeg
        positie--;
        std::cout << "returning nullptr in type1" << std::endl;
        return nullptr;
    } // else
} // ASTree::type1

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::UVAR){
        std::cout <<  ingang->var;
        return;
    } // if
    else if (ingang->type == Token::JUDG){
        printBoom(ingang->links);
        std::cout << ingang->var;
        printBoom(ingang->rechts);
    } // elif
    else if(ingang->type == Token::LAMBDA){
        std::cout << "(";
        std::cout << ingang->var;
        printBoom(ingang->links);
        std::cout << " ";
        printBoom(ingang->rechts);
        std::cout << ")";
    } // elif
    else if(ingang->type == Token::APPLICATIE){
        std::cout << "(";
        printBoom(ingang->links);
        std::cout << " ";
        printBoom(ingang->rechts);
        std::cout << ")";
    } // elif
    else if (ingang->type == Token::ARROW){
        std::cout << "(";
        printBoom(ingang->links);
        std::cout << " " << ingang->var << " ";
        printBoom(ingang->rechts);
        std::cout << ")";
    } // elif
    else if(ingang->type == Token::LVAR && ingang->links != nullptr){
        std::cout << ingang->var << "^";
        printBoom(ingang->links);
    } // elif
    else if(ingang->type == Token::LVAR){
        std::cout <<  ingang->var;
        return;
    } // elif
} // ASTree::printBoom

void ASTree::leegVector(){
    for (int i = 0; i < size; i++){
        // alleen variabelen staan in de boom
        if (tokens[i]->type != Token::VARIABELE){ 
            delete tokens[i];
            tokens[i] = nullptr; 
        } // if
    } // for
} // ASTree::freeVector

void ASTree::leegPtrVector(std::vector<Token*> bomen){
    for (int i = 0; i < int(gamma.size()); i++)
        deleteSubtree(gamma[i]);
} // ASTree::leegPtrVector

void ASTree::deleteSubtree(Token* ingang){
    if (ingang)
    {
        // ga naar linker kind
        if (ingang->links != nullptr) deleteSubtree(ingang->links);
        
        // ga naar rechter kind
        if(ingang->rechts != nullptr) deleteSubtree(ingang->rechts);   
    } // if
    delete ingang;
    ingang = nullptr;
} //ASTree::deleteSubtree

Token* ASTree::copySubtree(Token* ingang) {
    if (!ingang) return nullptr; // lege boom
    
    Token* copy = new Token; // knoop voor copy boom
    copy -> var = ingang -> var; // kopieer de string var
    copy -> type = ingang -> type; // kopieer de type

    if (ingang -> links != nullptr) { // ga naar linker kind
        copy -> links = copySubtree(ingang -> links);
    } // if

    if (ingang -> rechts != nullptr) { // ga naar rechter kind
        copy -> rechts = copySubtree(ingang -> rechts);
    } // if
    return copy;
} // ASTree::copySubtree

bool ASTree::isDerivable(Token* judgement){
    if(judgement->type != Token::JUDG) return false;

    // construeer de type
    Token* judgementLeft = getType(judgement->links);
    // controleer beide kanten op gelijkheid
    bool gelijk = treeEq(judgementLeft, judgement->rechts);

    // dealloceer memory
    deleteSubtree(judgementLeft);
    leegPtrVector(gamma);

    if (gelijk) // match
        return true;
    return false; // geen match
} // ASTree::isDerivable

Token* ASTree::getType(Token* root){
    if (root->type == Token::LAMBDA){ // lamdaRegel
        std::cout << "\\ rule" << std::endl;
        return lambdaRule(root);
    } // if
    else if(root->type == Token::APPLICATIE){ // @ regel
        std::cout << "application rule" << std::endl;
        return applRule(root);
    } // elif
    else if(root->type == Token::LVAR){ // var regel
        std::cout << "var rule" << std::endl;
        return varRule(root);
    } // elif
    return getType(root);
} // ASTree::getType

Token* ASTree::varRule(Token* Lvar){
    Token* T1  = nullptr;
    // ga de vector af en zoek voor matchende x
    for (int i = gamma.size()-1; i >= 0; i--){
        if (gamma[i]->links->var == Lvar->var){
            T1 = copySubtree(gamma[i]->rechts);
            return T1;
        } // if
    } // for
    std::cerr << "onbekend type" << std::endl;
    exit(1);
    return nullptr;
} // ASTree::varRule

Token* ASTree::applRule(Token* appl){
// maak de boom 
Token* applCopy = new Token("@", Token::APPLICATIE);
applCopy->links = getType(appl->links);
applCopy->rechts = getType(appl->rechts);

// sla de pointer naar de root op voor applCopy
Token* copy = applCopy;

// kopieer T`
applCopy = copySubtree(applCopy->links->rechts);

// verwijder de oude boom
deleteSubtree(copy);
return applCopy;
} // ASTree::applRule

Token* ASTree::lambdaRule(Token* lambda){
    // maak de boom
    Token* judgement = new Token(":", Token::JUDG);
    Token* X = copySubtree(lambda->links);
    Token* T = copySubtree(lambda->links->links);
    judgement->links = X; judgement->rechts = T;

    // stop de boom in gamma
    gamma.push_back(judgement);

    // (T->T`)
    Token* T1 = getType(lambda->rechts);
    Token* arrow = new Token("->", Token::ARROW);
    arrow->links = copySubtree(lambda->links->links);
    arrow->rechts = T1;
    return arrow;
} // ASTree::lambdaRule

bool ASTree::treeEq(Token* boom1, Token* boom2){
    if(boom1 == nullptr || boom2 == nullptr)
        return (boom1 == nullptr && boom2 == nullptr);
    
    if(boom1->type != boom2->type) // ongelijke types
        return false;
      
    // ongelijke strings
    if(boom1->type == Token::UVAR && boom1->var != boom2->var)
        return false;
         
    if(treeEq(boom1->links, boom2->links))
        return treeEq(boom1->rechts, boom2->rechts);
    
    return false;
} // ASTree::treeEq