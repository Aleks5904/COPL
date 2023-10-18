#include "parser.h"
#include "token.h"

// @function CharInSet()
// @abstract Contorleert of het input een nummer of character is. 
// @param input: Input die gecontroleerd wordt.
// @param first: geeft aan of de input op "nummer" of "character" 
// @param gecontroleerd moet worden.
// @return return geeft terug of input een character of nummer is
// @return (true= het is nummer/char, anders false).
// @pre: Er is een correcte input doorgegeven.
// @post: De input wordt qua type gecontroleerd.  
bool CharInSet(char input, bool first){
	if(input >= 'a' && input <= 'z')
		return true; // charcter
	if(input >= 'A' && input <= 'Z')
		return true; // character
	if(input >= '0' && input <= '9' && !first)
		return true; // nummer
	return false; // geen char/num
} // CharInSet

Parser::Parser(string invoer){
	input = invoer; // de doorgegevn invoer
	positie = 0; // start positie in de vector
	haakje = 0; // aantal haakjes (+ = open) (- = sluit)
	tokenize();
} // Parser::Parser

void Parser::tokenize(){
	int StringSize = input.size();
	for (int i = 0; i < StringSize; i++){
		Token* t = new Token;
		if (input[i] == '('){ // haakje-open
			t->type = Token::HAAKJEOPEN;
			tokens.push_back(t);
		}
		else if(input[i] == ')'){ // haakje-sluit
			t->type = Token::HAAKJESLUIT;
			tokens.push_back(t);
		}
		else if(input[i] == '\\'){ // slash
			t->type = Token::SLASH;
			tokens.push_back(t);	
		}
		else if(input[i] != ' ') { // variabele
			string variable = "";
			t->type = Token::VARIABELE;
			variable.push_back(input[i]);
			while (input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' && input[i+1] != '\\' && i != StringSize-1){
				i++;
				variable.push_back(input[i]);
			} // check variabele op correctheid en sla aan
			t->var = variable;
			tokens.push_back(t);
		}
		else if(input[i] == ' '){ // ga door als spatie
			continue;
		}
	} // for
	size = tokens.size();
} // Parser::tokenize

bool Parser::checkExpression(){
	positie = 0;
	haakje = 0;
	return expr() == 1;
} // Parser::checkExpression

int Parser::expr(){
	int check1, check2;
	cout << "expr" << endl;
	check1 = lexpr();
	if(check1 == 0){
		return 0;
	}
	check2 = expr1();
	return(check1*check2);
	
} // Parser::expr

int Parser::lexpr(){
	bool check1, check2;
	Token* check;
	cout << "lexpr" << endl;
	if(positie >= size){
		cout << "Er mist een expressie" << endl;
		return 0;
	}
	check = peek();
	//Haakje sluit mag hier niet
	if(check->type == Token::HAAKJESLUIT){
		cout << "Er mist een expressie" << endl;
		haakje --;
		return 0;
	}
	//Slash checken
	if(check->type == Token::SLASH){
		positie++;
		check1 = var();
		if(check1 == 0)
			cout << "na een \\ moet een variable staan." << endl;
		check2 = expr();
		if(check2 == 0)
			cout << "Na een \\ moet een geldige expressie staan" << endl;
		return(check1*check2);
	}
	else{
		return pexpr();
	}

} // Parser::lexpr

int Parser::expr1(){
	Token* check;
	cout << "expr1" << endl;
	//Lege expressie mag
	if(positie >= size){
		cout << "expr1 size" << endl;
		return 1;
	}
	check = peek();
	if(check->type == Token::HAAKJESLUIT){
		cout << "Expr1 sluit" << endl;
		haakje --;
		return 2;
	}
	return expr();
} // Parser::expr1

int Parser::pexpr(){
	int check1;
	Token* check;

	cout << "pexpr" << endl;
	if(positie >= size){
		return 0;
	}
	check = peek();
	if(check->type == Token::VARIABELE){
		return var();
	}
	else if(check->type== Token::HAAKJEOPEN){
		positie++;
		haakje++;
		check1 = expr();
		if(positie >= size){
			cout << "te weinig sluit haakjes" << endl;
			return 0;
		}
		check = peek();
		if(check->type == Token::HAAKJESLUIT){
			cout << check1 << endl;
			positie++;
			return(check1/2);
		}
		return 0;
	}
	return 0;
} // Parser::pexpr

int Parser::var(){
	string var; // voor het opslaan van de informatie over token
	int VarSize;
	Token* check;
	cout << "var" << endl;
	if(positie >= size){
		return 0;
	}
	check = peek();
	positie++;
	if(check->type == Token::HAAKJEOPEN) haakje++;
	if(check->type == Token::HAAKJESLUIT) haakje--;
	else if(check->type != Token::VARIABELE){
		cout << "geen variabele type" << endl;
		return 0;
		// return false;
	}
	cout << check->var << endl;
	var = check->var;
	VarSize = var.size();
	// controleer variabele op syntax
	for(int i = 0; i < VarSize; i++){
		if(!CharInSet(var[i], i == 0)){
			cout << "geen variabele string" << endl;
			return 0;
			// return false;
		}
	}
	cout << "var true" << endl;
	return 1;
} // Parser::var

Token* Parser::peek(){
	return tokens[positie];
}; // Parser::peek

void Parser::print(){
	int size = tokens.size();
	Token* werk; // hulp variabele voor het oplsaan van tokens
	for (int i = 0; i < size; i++){
		werk = tokens[i];
		switch (werk->type)
		{
		case Token::HAAKJEOPEN:
			cout << "(";
			break;
		case Token::HAAKJESLUIT:
			cout << ")";
			break;
		case Token::SLASH:
			cout << "\\";
			break;
		case Token::VARIABELE:
			cout << werk->var;
			break;
		default: // spatie
			cout << " ";
			break;
		}
	}
	cout << endl;
} // Parser::print
