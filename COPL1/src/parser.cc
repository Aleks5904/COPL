#include "../include/parser.h"
#include "../include/token.h"


bool Parser::CharInSet(char input, bool first){
	if(input >= 'a' && input <= 'z')
		return true; // charcter
	if(input >= 'A' && input <= 'Z')
		return true; // character
	if(input >= '0' && input <= '9' && !first)
		return true; // nummer
	return false; // geen char/num
} // Parser::CharInSet

Parser::Parser(string invoer){
	input = invoer; // de doorgegevn invoer
	tokenize();
    positie = -1;
	haakje = 0;
	expr();
    print();
    leegVector();
} // Parser::Parser

void Parser::tokenize(){
	int StringSize = input.size();
	for (int i = 0; i < StringSize; i++){
		if (input[i] == '('){ // haakje-open
			Token* t = new Token("(", Token::HAAKJEOPEN);
			tokens.push_back(t);
		} // if
		else if(input[i] == ')'){ // haakje-sluit
			Token* t = new Token(")", Token::HAAKJESLUIT);
			tokens.push_back(t);
		} // elif
		else if(input[i] == '\\'){ // slash
			Token* t = new Token("\\", Token::LAMBDA);
			tokens.push_back(t);	
		} // elif
		else if(CharInSet(input[i], true)){ // variabele
			string variable = "";
			variable.push_back(input[i]);
			while (input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' && input[i+1] != '\\' && i != StringSize-1){
				i++;
				variable.push_back(input[i]);
			} // check variabele op correctheid en sla aan
			Token* t = new Token(variable, Token::VARIABELE);
			tokens.push_back(t);
		} // elif
		else if(input[i] == ' ' || CharInSet(input[i], false)){ // ga door als spatie
			continue;
		} // elif
	} // for
    Token* end = new Token("end", Token::END);
    tokens.push_back(end);
    size = tokens.size();
} // Parser::tokenize

int Parser::expr(){
    cout << "expr" << endl;
    cout << "positie:" << positie << endl;
    bool check1 = lexpr();
    if (!check1) {
        cout << "expr error" << std::endl;
        leegVector();
        exit(1);
    } // if
    expr1();
    return check1;
} // Parser::expr

void Parser::expr1(){
    cout << "expr1" << endl;
    cout << "positie:" << positie << endl;
    bool check1 = lexpr();
    if (check1) 
        expr1();
} // Parser::expr1

int Parser::lexpr(){
    cout << "lexpr" << endl;
    positie++; // ga naar volgende token in vector
    cout << "positie:" << positie << endl;
    Token* huidig = peek(); // krijg de momentele token uit vector
    if (pexpr()) {
        cout << "lexpr 1 return" << endl;
        return 1;
    } // if
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        leegVector();
        exit(1);
    } // if
    else if (huidig->type == Token::LAMBDA){
        cout << "lambda lexpr" << endl;
        positie++;
        huidig = peek(); // krijg de momentele token uit vector
        if (huidig->type != Token::VARIABELE){
            cout << "missende variabel" << endl;
            leegVector();
            exit(1);
        } // if
           if (!lexpr()){
             std::cerr << "geen expressie in abstractie" << std::endl;
             leegVector();
             exit(1);
           } // if
        
        return 1;
    } // elif
    cout << "lexpr 0 return" << endl;
    return 0;
} // Parser::lexpr

int Parser::pexpr() {
    cout << "pexpr" << endl;
    cout << "positie:" << positie << endl;
    Token* huidig = peek(); // krijg de momentele token uit vector
    std::cout << "var pexpr: " << huidig->var << endl;
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cerr << "geen opende haakje" << std::endl;
        leegVector();
        exit(1);
    } // if
    if(huidig->type == Token::VARIABELE) // variabele aanwezig
        return 1;
    else if(huidig->type == Token::HAAKJEOPEN){ // '('expr')' 
        cout << "haakjeopen (pexpr)" << endl;
        haakje++;
        // Token* temp = expr(nullptr);
        expr();
        huidig = peek(); // krijg de momentele token uit vector
        if(huidig->type == Token::HAAKJESLUIT){
            haakje--;
            return 1;
        } // if
        else{
            std::cerr << "geen sluitende haakje" << std::endl;
            leegVector();
            exit(1);
        } // else
        
    } // elif
    cout << "pexpr return 0" <<endl;
    return 0;
    
} // Parser::pexpr()

Token* Parser::peek(){
	return tokens[positie];
}; // Parser::peek

void Parser::print(){
	int size = tokens.size();
	Token* temp; // hulp variabele voor het oplsaan van tokens
	for (int i = 0; i < size; i++){
		temp = tokens[i];
		switch (temp->type){
		case Token::HAAKJEOPEN:
			cout << "(";
			break;
		case Token::HAAKJESLUIT:
			cout << ")";
			break;
		case Token::LAMBDA:
			cout << "\\";
			break;
		case Token::VARIABELE:
			cout << temp->var;
			break;
		default: // spatie
			cout << " ";
			break;
		}
	} // for
	cout << endl;
} // Parser::print

void Parser::leegVector(){
    for (int i = 0; i < size; i++){
        delete tokens[i];
        tokens[i] = nullptr; 
    } // for
} // ASTree::freeVector