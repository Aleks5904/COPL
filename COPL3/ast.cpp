#include "ast.h"
#include <iostream>
#include "token.h"
using namespace std;

ASTree::ASTree(const string invoer) {
    input = invoer; // de doorgegeven invoer
    fout = false; positie = -1, haakje = 0; // start waarden
	tokenize();    
    maakBoom();
    // controleer of de boom afleidebaar is
    if (!fout && !isDerivable(treeRoot))
        fout = true;
    if(!fout){ // print de boom als er geen fout gevonden is
        printBoom(treeRoot);
        cout << endl;
    } // if
    // verwijder de boom en de vector
    leegVector();
    deleteSubtree(treeRoot);
} // ASTree::ASTree

bool ASTree::varCheck(const int i){
    if ((CharInSet(input[i], true) || isNUm(input[i]) || CharInSet(input[i], false)) 
        && input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' 
        && input[i+1] != '^' && input[i+1] != '\\' && input[i+1] != ':' && input[i+1] != '-')
            return true; 
return false;
} // ASTree::varCheck

bool ASTree::CharInSet(const char input, const bool lowerCase){
	if(input >= 'a' && input <= 'z' && lowerCase)
		return true; 
	else if(input >= 'A' && input <= 'Z' && !lowerCase)
		return true; 
	return false; // geen char/num
} // CharInSet

bool ASTree::isNUm(const char input){
    if (input >= '0' && input <= '9')
        return true;    
    return false;
} // ASTree::isNUm

void ASTree::tokenize(){
	int stringSize = input.size();
	for (int i = 0; i < stringSize; i++){
        Token* t;
		if (input[i] == '('){       // haakje-open
			t = new Token("(", Token::HAAKJEOPEN);
            tokens.push_back(t);
        } else if(input[i] == ')'){ // haakje-sluit
			t = new Token(")", Token::HAAKJESLUIT);
            tokens.push_back(t);
        } else if(input[i] == ':'){ // judgement
			t = new Token(":", Token::JUDG);
            tokens.push_back(t);
        } else if(input[i] == '\\'){// slash
			t = new Token("\\", Token::LAMBDA);	
            tokens.push_back(t);
        } else if(input[i] == '^'){ // type
            t = new Token("^", Token::TOO);
            tokens.push_back(t);
        } else if(input[i] == '-' && input[i+1] == '>'){ // pijl
            t = new Token("->", Token::ARROW);
            i++; tokens.push_back(t);
        } // elif
		else if(CharInSet(input[i], true)) { // lvar 
            string var = "";
            var += input[i];
            while (varCheck(i) && i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t = new Token(var, Token::LVAR);
            tokens.push_back(t);
		} // elif
        else if(CharInSet(input[i], false)) { // uvar 
            string var = "";
            var += input[i];
            while (varCheck(i) &&  i != stringSize-1){
				i++;
				var += input[i];
			} // check variabele op correctheid en sla aan
            t = new Token(var, Token::UVAR);
            tokens.push_back(t);
		} // elif
		else if(input[i] == ' ') // ga door als spatie
			continue;
	} // for
    Token* end = new Token("end", Token::END);
    tokens.push_back(end);
    size = tokens.size();
} // ASTree::tokenize

bool ASTree::maakBoom(){
    Token* huidig = nullptr;
	treeRoot = expr(nullptr); // start recursive descent
    huidig = peek(); // controleer op judgement
    if (huidig->type != Token::JUDG){
        cerr << "missende judgement" << endl;
        fout = true; return false;
    } // if
    Token* judg = new Token(":", Token::JUDG);
    judg->links = treeRoot; treeRoot = judg;
    judg->rechts = type(); // maak de rechter deel van de boom
    if (judg->rechts == nullptr){
        cerr << "missende type " << endl;
        fout = true; return false;
    } // if
    treeRoot = judg;
    return treeRoot;
} // ASTree::checkExpression

Token* ASTree::expr(Token* ingang){
    if(fout) return nullptr;
    Token* temp = lexpr(ingang); // check voor expressie
    if (temp == nullptr) {
        cerr << "geen expressie" << endl;
        fout = true; return nullptr;
    } // if
    ingang = expr1(temp); // check voor expressie`
    return ingang;
} // ASTree::expr

Token* ASTree::expr1(Token* ingang){
    if(fout) return nullptr;
    Token* temp = lexpr(ingang); // check voor lexpressie
    if (temp != nullptr) {
        ingang = expr1(temp); // check voor expressie`
        return ingang;
    } // if
    else
        return ingang;
} // ASTree::expr1

Token* ASTree::lexpr(Token* ingang){
    if(fout) return nullptr;
    positie++; // ga naar volgende token in vector
    Token* huidig = peek(); // krijg de momentele token uit vector
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        cerr << "geen opende haakje" << endl;
        fout = true; return nullptr;
    } // if
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        } // if
        // applicatie aanwezig
        Token* tok = new Token("@" ,Token::APPLICATIE);
        tok->links = ingang;
        tok->rechts = temp;
        return tok;
    } // if
    else if (huidig->type == Token::LAMBDA){
        // abstractie aanwezig
        positie++;
        huidig = peek(); // krijg de momentele token uit vector
        if (huidig->type == Token::LVAR){ // Lvar aanwezig
            Token* lambda = new Token("\\", Token::LAMBDA);
            Token* var = new Token(huidig->var, Token::LVAR);
            lambda->links = var;
            positie++;
            huidig = peek(); // check volgende token in string
            if (huidig->type != Token::TOO){
                deleteSubtree(lambda);
                cerr << "missende ^" << endl;
                fout = true; return nullptr;
            } // if
            temp = type(); // controleer het type
            if (temp == nullptr){
                cerr << "missende type in abstractie" << endl;
                fout = true; return nullptr;
            } // if
            lambda->links->links = temp;
            lambda->rechts = lexpr(nullptr);
            if (lambda->rechts == nullptr){
                deleteSubtree(lambda);
                cerr << "geen expressie in abstractie" << endl;
                fout = true; return nullptr;
            } // if
            if (ingang == nullptr)
                return lambda;
            else{ // applicatie aanwezig, creeër boom
                Token* tok = new Token("@", Token::APPLICATIE);
                tok->links = ingang;
                tok->rechts = lambda;
                return tok;
            } // else
        } // if
        else{
            cerr << "geen variabele in abstractie" << endl;
            fout = true; return nullptr;
        } // else
    } // elif
    else{
        return nullptr;
    } // else
} // ASTree::lexpr

Token* ASTree::pexpr() {
    if(fout) return nullptr;
    Token* huidig = peek(); // krijg de momentele token uit vector
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        cerr << "geen opende haakje" << endl;
        fout = true; return nullptr;
    } // if
    if(huidig->type == Token::LVAR){ // lvar aanwezig
        Token* var = new Token(huidig->var, Token::LVAR);
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
            cerr << "geen sluitende haakje" << endl;
            deleteSubtree(temp);
            fout = true; return nullptr;
        } // else
    } // elif
    else
        return nullptr;
} // ASTree::pexpr()

Token* ASTree::type(){
    if(fout) return nullptr;
    positie++;
    Token* t2 = nullptr;
    Token* t = peek(); // krijg de momentele token uit vector
    if (t->type == Token::UVAR){
        t2 = type1(); // check op < ->Type | ε >
        Token* uvar = new Token(t->var, t->type);
        if (t2 != nullptr){ // ->T  aanwezig, creeër boom
            Token* arrow = new Token("->", Token::ARROW);
            arrow->links = uvar;
            arrow->rechts = t2;
            return arrow;
        } // if
        return uvar;
    } // if
    else if(t->type == Token::HAAKJEOPEN){ // <'('Type')'>
        t2 = type();
        if (t2 != nullptr){
            positie++; t = peek();
            if (t->type != Token::HAAKJESLUIT){
                deleteSubtree(t2);
                cout << "geen sluitende haakje" << endl;
                fout = true; return nullptr;
            } // if
            Token* t3 = type1();  // check op < ->Type | ε >
            if (t3 != nullptr) {
                Token* arrow = new Token("->", Token::ARROW);
                arrow->links = t2;
                arrow->rechts = t3;
                return arrow;
            } // if
            else return t2;
        } // if
        else{
            cerr << "error (type)" << endl;
            fout = true; return nullptr;
        } // else
    } // elif
    return nullptr;
} // ASTree::type

Token* ASTree::type1(){
    if(fout) return nullptr;
    positie++;
    Token* t = peek(); // krijg de momentele token uit vector
    if(t->type == Token::ARROW){ // ->Type
        Token* t2 = type();
        if (t2 != nullptr)
            return t2;
        else{
            deleteSubtree(t2);
            cerr << "error (type1)" << endl;
            fout = true; return nullptr;
        } // else
    } // if
    else{ // leeg
        positie--;
        return nullptr;
    } // else
} // ASTree::type1

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::UVAR){
        cout <<  ingang->var;
        return;
    } // if
    else if (ingang->type == Token::JUDG){
        printBoom(ingang->links);
        cout << ingang->var;
        printBoom(ingang->rechts);
    } // elif
    else if(ingang->type == Token::LAMBDA){
        cout << "(";
        cout << ingang->var;
        printBoom(ingang->links);
        cout << " ";
        printBoom(ingang->rechts);
        cout << ")";
    } // elif
    else if(ingang->type == Token::APPLICATIE){
        cout << "(";
        printBoom(ingang->links);
        cout << " ";
        printBoom(ingang->rechts);
        cout << ")";
    } // elif
    else if (ingang->type == Token::ARROW){
        cout << "(";
        printBoom(ingang->links);
        cout << " " << ingang->var << " ";
        printBoom(ingang->rechts);
        cout << ")";
    } // elif
    else if(ingang->type == Token::LVAR && ingang->links != nullptr){
        cout << ingang->var << "^";
        printBoom(ingang->links);
    } // elif
    else if(ingang->type == Token::LVAR){
        cout <<  ingang->var;
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

void ASTree::leegPtrVector(vector<Token*> & bomen){
    for (int i = 0; i < int(bomen.size()); i++)
        deleteSubtree(bomen[i]);
} // ASTree::leegPtrVector

void ASTree::deleteSubtree(Token* ingang){
    if (ingang){
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
    
    Token* copy = new Token;   // knoop voor copy boom
    copy->var = ingang->var;   // kopieer de string var
    copy->type = ingang->type; // kopieer de type

    if (ingang->links != nullptr) { // ga naar linker kind
        copy->links = copySubtree(ingang->links);
    } // if

    if (ingang->rechts != nullptr) { // ga naar rechter kind
        copy->rechts = copySubtree(ingang->rechts);
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
    if(fout) return nullptr;

    if (root->type == Token::LAMBDA) // lamdaRegel
        return lambdaRule(root);
    else if(root->type == Token::APPLICATIE) // @ regel
        return applRule(root);
    else if(root->type == Token::LVAR) // var regel
        return varRule(root);

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
    cerr << "onbekend type" << endl;
    fout = true; return nullptr;
} // ASTree::varRule

Token* ASTree::applRule(Token* appl){
// maak de boom 
Token* applCopy = new Token("@", Token::APPLICATIE);
applCopy->links = getType(appl->links);
applCopy->rechts = getType(appl->rechts);

if(fout){ 
    deleteSubtree(applCopy);
    return nullptr;
} // if

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