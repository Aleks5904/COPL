#include "../include/AST.h"
using namespace std;

ASTree::ASTree(string invoer) {
    input = invoer; // de doorgegeven invoer
	tokenize();
    // default parameters
    fout = false; overflow = false;
    positie = -1; haakje = 0;

    // start recusrive descent
	treeRoot = expr(nullptr); 

    // print boom als geen foute expressie en geen β-overflow
    if(!fout){ 
        treeRoot = postOrder(treeRoot);
        if(!overflow){
            printBoom(treeRoot);
            cout << endl;
        } // if
    } // if
    deleteSubtree(getTreeRoot());
    leegVector();
} // ASTree::ASTree

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::VARIABELE){
        cout <<  ingang->var;
        return;
    } // if
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
} // ASTree::printBoom

Token* ASTree::postOrder(Token* ingang) {
    if(overflow) return nullptr;
    int limiet = 0; // houdt aantal β-reducties bij
    if (ingang == nullptr) 
        return ingang;  
    
    ingang->links = postOrder(ingang->links);
    ingang->rechts = postOrder(ingang->rechts);

    // check voor startplek (applicatie+abstractie)
    if (ingang->type == Token::APPLICATIE && ingang->links != nullptr
        && ingang->links->type == Token::LAMBDA) {
        while (ingang->type == Token::APPLICATIE && ingang->links != nullptr
            && ingang->links->type == Token::LAMBDA) {
            limiet++;
            if(limiet > 1000) { // over 1000 β-reducties
                cout << "teveel β-reducties" << endl;
                deleteSubtree(ingang);
                overflow = true; return nullptr;
            } // if
            ingang = betaReductie(ingang);
        } // while
    } // if

    return ingang;
} // ASTree::postOrder

void ASTree::leegVector(){
    for (int i = 0; i < size; i++){
            delete tokens[i];
            tokens[i] = nullptr; 
    } // for
} // ASTree::freeVector

void ASTree::deleteSubtree(Token* ingang){
    if (ingang){
        // ga naar linker kind
        if (ingang->links != nullptr) deleteSubtree(ingang->links);
        
        // ga naar rechter kind
        if(ingang->rechts != nullptr) deleteSubtree(ingang->rechts);   
    } // if
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
		} // if
		else if(input[i] == ')'){ // haakje-sluit
			t->type = Token::HAAKJESLUIT;
			t->var = ")";
			tokens.push_back(t);
		} // elif
		else if(input[i] == '\\'){ // LAMBDA
			t->type = Token::LAMBDA;
			t->var = "\\";
			tokens.push_back(t);	
		} // elif
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
		} // elif
		else if(input[i] == ' ' || CharInSet(input[i], false)){ // ga door als spatie
            delete t;
			continue;
		} // elif
	} // for
    Token* end = new Token();
    end->type = Token::END;
    tokens.push_back(end);
    size = tokens.size();
} // ASTree::tokenize

Token* ASTree::findLambda(Token* ingang) {
    if (ingang == nullptr) {
        return nullptr; 
    } // if
    if (ingang->type == Token::LAMBDA) { // return de kind van lambda
        return ingang->links;  
    } // if
    // zoek lambda in linker sub-boom
    Token* linksResult = findLambda(ingang->links);
    if (linksResult != nullptr) {
        return linksResult; 
    } // if
    // zoek lambda in rechter sub-boom
    Token* rechtsResult = findLambda(ingang->rechts);
    if (rechtsResult != nullptr) {
        return rechtsResult; 
    } // if
    return nullptr;
} // ASTree::findLambda

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
} // ASTree::findGivenVar

Token* ASTree::replaceSubtree(Token* ingang, Token* N, string variabel) {
    if (ingang == nullptr) { // lege knoop
        return nullptr; 
    }
    if (ingang->var == variabel) { // gevonden plek voor substitutie
        delete ingang;
        Token* nCopy = copySubtree(N); // maak copy van N
        ingang = nCopy;
        return ingang;
    }
    // zoek variabel in linker/rechter kind
    ingang->rechts = replaceSubtree(ingang->rechts, N, variabel);
    ingang->links = replaceSubtree(ingang->links, N, variabel);

    return ingang;  
} // ASTree::replaceSubtree

Token* ASTree::copySubtree(Token* ingang) {
    if (!ingang) return nullptr; // lege boom
    
    Token* copy = new Token; // knoop voor copy boom
    copy->var = ingang->var; // kopieer de string var
    copy->type = ingang->type; // kopieer de type

    if (ingang->links != nullptr) { // ga naar linker kind
        copy->links = copySubtree(ingang->links);
    } // if

    if (ingang -> rechts != nullptr) { // ga naar rechter kind
        copy->rechts = copySubtree(ingang->rechts);
    } // if
    return copy;
} // Tree::copySubtree

Token* ASTree::expr(Token* ingang){
    if(fout) return nullptr;
    Token* temp = lexpr(ingang);
    if (temp == nullptr) {
        fout = true;
        return nullptr;
    } // if
    ingang = expr1(temp);
    return ingang;
} // ASTree::expr

Token* ASTree::expr1(Token* ingang){
    if(fout) return nullptr;
    Token* temp = lexpr(ingang);
    if (temp != nullptr) {
        ingang = expr1(temp);
        return ingang;
    } // if
    return ingang;
} // ASTree::expr1

Token* ASTree::lexpr(Token* ingang){
    if(fout) return nullptr;
    positie++; // ga naar volgende token in vector
    Token* huidig = peek(); // krijg de momentele token uit vector
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        cerr << "geen opende haakje" << endl;
        fout = true;
        return nullptr;
    } // if
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        }
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
        if (huidig->type == Token::VARIABELE){
            Token* lambda = new Token("\\", Token::LAMBDA);
            Token* var = new Token(huidig->var, Token::VARIABELE);
            lambda->links = var;
            temp = lexpr(nullptr);
            if (temp == nullptr){
                cerr << "geen expressie in abstractie" << endl;
                deleteSubtree(lambda);
                fout = true;
                return nullptr;
            } // if
            lambda->rechts = temp;
            if (ingang == nullptr)
                return lambda;
            
            Token* tok = new Token("@", Token::APPLICATIE);
            tok->links = ingang;
            tok->rechts = lambda;
            return tok;
        }else{
            fout = true;
            cerr << "geen variabele in abstractie" << endl;
            return nullptr;
        } // else
    } // elif
    else
        return nullptr;
} // ASTree::lexpr

Token* ASTree::pexpr() {
    if(fout) return nullptr;
    Token* huidig = peek(); // krijg de momentele token uit vector
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        fout = true;
        cerr << "geen opende haakje" << endl;
        return nullptr; 
    } // if
    if(huidig->type == Token::VARIABELE){ // variabele aanwezig
        Token* var = new Token(huidig->var, Token::VARIABELE);
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
            return nullptr;
        } // else
    } // elif 
    else
        return nullptr;
} // ASTree::pexpr()

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

Token* ASTree::betaReductie(Token* ingang){
    string deltaX;      // kind van lambda
    bool extraStap = false;  // zorgt voor "return ingang" i.p.v. "return ingang->rechts"
    bool naarRechts = false; // houdt bij of er 1 extra stap naar rechts gedaan is
    Token* copy = nullptr;   // slaat de ingang op
    Token* temp = nullptr;   // slaat de ingang na betareductie op
    if (ingang != nullptr){
        Token* N = copySubtree(ingang->rechts);
        deleteSubtree(ingang->rechts);
        ingang->rechts = nullptr; // @->rechts = nullptr
        copy = ingang;
        ingang = ingang->links;
        deltaX = ingang->links->var;
        Token* alfa = findLambda(ingang->rechts);
        if (alfa != nullptr){ // mogelijke gebonden variabel
        
            bool bound = findGivenVar(N, alfa->var);
            if (bound) // α-conversie
                alfa->var += "'";
            
        } // if
        // zoek rechter kind = variabel
        if (ingang->rechts->type != Token::VARIABELE){
            ingang = ingang->rechts;
            naarRechts = true;
            if (ingang->rechts->type != Token::VARIABELE){
                ingang = replaceSubtree(ingang, N, deltaX);
                deleteSubtree(N);
                extraStap = true;
            } // if
        } // if
        if(!extraStap && !naarRechts){ // case #1 (vervang ingang->rechts)
            ingang->rechts = replaceSubtree(ingang->rechts, N, deltaX);
            deleteSubtree(N); 
        } // if
        else if (!extraStap){ // case #2 (vervang ingang)
            ingang = replaceSubtree(ingang, N, deltaX);
            deleteSubtree(N);
        } // elif
        if (naarRechts) // case #2 (return ingang)
            temp = copySubtree(ingang);
        else // case #1 (return ingang->rechts)
            temp = copySubtree(ingang->rechts);
        
        deleteSubtree(copy); // verwijder begin-boom
        ingang = temp; // ingang = gereduceerde boom
        return ingang;
    } // if
    return nullptr;
} // ASTree::betaReductie

