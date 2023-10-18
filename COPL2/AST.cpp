#include "AST.h"
using namespace std;

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

// Token::Token(Type t,const string& s): expression(s), type(t) {}

ASTree::ASTree(const string& i, Token* newRoot): input(i), treeRoot(newRoot)
{treeRoot->type = Token::EXPRESSION;}


ASTree::ASTree() : treeRoot(nullptr) {}

// string ASTree::getRoot(){
//     return treeRoot->expression;;
// }

// Token* ASTree::expr(){
        
// }

Token* ASTree::parse(const string& input) {
    int i = 0;
    return parseExpression(input, i);
}


// ASTree::Parser(string invoer){
// 	input = invoer; // de doorgegevn invoer
// 	positie = 0; // start positie in de vector
// 	haakje = 0; // aantal haakjes (+ = open) (- = sluit)
// 	tokenize();
// } // ASTree::Parser

void ASTree::tokenize(){
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
} // ASTree::tokenize

bool ASTree::maakBoom(){
	positie = 0;
	haakje = 0;
    treeRoot = tokens[0];
	return expr(treeRoot);
} // ASTree::checkExpression

Token* ASTree::expr(Token* & ingang){
	Token * check1;
    Token * check2;
	cout << "expr" << endl;
	check1 = lexpr(ingang);
	if(check1 == nullptr){
		return nullptr;
	}
    ingang = check1;
	check2 = expr1(ingang);
    ingang->left = check2;
    return ingang;	
} // ASTree::expr

Token* ASTree::lexpr(Token* & ingang){
	Token* check;
	cout << "lexpr" << endl;
	if(positie >= size){
		cout << "Er mist een expressie" << endl;
		exit(1);
	}
	ingang = peek();
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
		return pexpr(ingang);
	}
} // ASTree::lexpr

Token* ASTree::expr1(Token* & ingang){
	Token* check;
	cout << "expr1" << endl;
	//Lege expressie mag
	if(positie >= size){
		cout << "expr1 size" << endl;
		return nullptr;
	}
	check = peek();
	if(check->type == Token::HAAKJESLUIT){
		cout << "Expr1 sluit" << endl;
		haakje --;
		return nullptr;
	}
	return expr(ingang);
} // ASTree::expr1

Token* ASTree::pexpr(Token* & ingang){
	int check1;
	Token* check;

	cout << "pexpr" << endl;
	if(positie >= size){
		exit(1);
	}
	ingang = peek();
	if(ingang->type == Token::VARIABELE){
		return var();
	}
	else if(ingang->type== Token::HAAKJEOPEN){
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
			return(check1/2);
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
