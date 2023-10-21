#include "AST.h"
using namespace std;

// Token::Token(){
//     left = nullptr;
//     right = nullptr;
//     var = "";
// }
Token::Token(){};

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

ASTree::ASTree() {}

void ASTree::parse(string invoer){
	input = invoer; // de doorgegevn invoer
	positie = 0; // start positie in de vector
	haakje = 0; // aantal haakjes (+ = open) (- = sluit)
	tokenize();
} // ASTree::Parser

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
		else if(input[i] != ' ') { // variabele
			string variable = "";
			std::cout << "var encountered at " << i  << std::endl;
			t->type = Token::VARIABELE;
			variable += input[i];
			while (input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' && input[i+1] != '\\' && i != StringSize-1){
				i++;
				variable += input[i];
				std::cout << "langere var" << std::endl;
			} // check variabele op correctheid en sla aan
			t->var = variable;
			std::cout << "var: " << variable << std::endl;
			tokens.push_back(t);
		}
		else if(input[i] == ' '){ // ga door als spatie
			std::cout << "' ' encountered at " << i  << std::endl;
			continue;
		}
		// else continue;
	} // for
	size = tokens.size();
	std::cout << " size: " << size << std::endl;
} // ASTree::tokenize

bool ASTree::maakBoom(){
	positie = 0;
	haakje = 0;
    treeRoot = tokens[0];
	return expr(treeRoot);
} // ASTree::checkExpression

Token* ASTree::expr(Token*  ingang){
	std::cout << ingang->var << std::endl; 
	Token * check1;
    Token * check2;
	cout << "expr" << endl;
	check1 = lexpr(ingang);
	if(check1 == nullptr){
		return nullptr;
	}
    ingang = check1;
	std::cout << "test: "<< check1->var << std::endl; 
	std::cout << "test: "<< ingang->var << std::endl; 
	check2 = expr1(ingang);
    ingang->right = check2;
    return ingang;	
} // ASTree::expr

Token* ASTree::lexpr(Token*  ingang){
	std::cout << ingang->var << std::endl; 
	Token* check;
	cout << "lexpr" << endl;
	if(positie >= size){
		cout << "Er mist een expressie" << endl;
		exit(1);
	}
	check = peek();
	std::cout << ingang->type << std::endl;
	//Haakje sluit mag hier niet
	if(check->type == Token::HAAKJESLUIT){
		cout << "Er mist een expressie" << endl;
		haakje --;
		exit(1);
	}
	//Slash checken
	if(check->type == Token::SLASH){
		positie++;
        ingang->left = var();
        expr(ingang->right);
	}
	else{
		std::cout << "here" << std::endl;
		return pexpr(ingang);
	}
} // ASTree::lexpr

Token* ASTree::expr1(Token*  ingang){
	std::cout << ingang->var << std::endl; 
	Token* check;
	cout << "expr1" << endl;
	//Lege expressie mag
	if(positie >= size){
		cout << "expr1 size" << endl;
		return nullptr;
	}
	check = peek();
	cout << check->var << std::endl;
	if(check->type == Token::HAAKJESLUIT){
		cout << "Expr1 sluit" << endl;
		haakje --;
		return nullptr;
	}
	return expr(ingang);
} // ASTree::expr1

Token* ASTree::pexpr(Token*  ingang){
	std::cout << ingang->var << std::endl; 
	int check1;
	Token* check;

	cout << "pexpr" << endl;
	if(positie >= size){
		exit(1);
	}
	ingang = peek();
	std::cout << "test1: " << ingang->var << std::endl;
	if(ingang->type == Token::VARIABELE){
		std::cout << "variabele" << std::endl;
		return var();
	}
	else if(ingang->type== Token::HAAKJEOPEN){
		std::cout << "haakje open" << std::endl;
		positie++;
		haakje++;
        ingang->left = peek();
		expr(ingang->left);
		if(positie >= size){
			cout << "te weinig sluit haakjes" << endl;
			exit(1);
		}
		check = peek();
		if(check->type == Token::HAAKJESLUIT){
			cout << check1 << endl;
			positie++;
			// return(check1/2);
		}
		return 0;
	}
	return 0;
} // ASTree::pexpr

Token* ASTree::var(){
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
		exit(1);
		// return false;
	}
	cout << check->var << endl;
	var = check->var;
	VarSize = var.size();
	// controleer variabele op syntax
	for(int i = 0; i < VarSize; i++){
		if(!CharInSet(var[i], i == 0)){
			cout << "geen variabele string" << endl;
			exit(1);
		}
	}
	cout << "var true" << endl;
	return check;
} // ASTree::var

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

// void ASTree::print(){
// 	int size = tokens.size();
// 	Token* werk; // hulp variabele voor het oplsaan van tokens
// 	for (int i = 0; i < size; i++){
// 		werk = tokens[i];
// 		switch (werk->type)
// 		{
// 		case Token::HAAKJEOPEN:
// 			cout << "(";
// 			break;
// 		case Token::HAAKJESLUIT:
// 			cout << ")";
// 			break;
// 		case Token::SLASH:
// 			cout << "\\";
// 			break;
// 		case Token::VARIABELE:
// 			cout << werk->var;
// 			break;
// 		default: // spatie
// 			cout << " ";
// 			break;
// 		}
// 	}
// 	cout << endl;
// } // ASTree::print
