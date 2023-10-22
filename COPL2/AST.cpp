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
			t->type = Token::APPLICATION;
			t->var = "@";
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

Token* ASTree::expr(Token* ingang) {
    std::cout << ingang->var << std::endl;
    Token* check1;
    Token* check2;
    std::cout << "expr" << std::endl;
    check1 = lexpr(ingang);
    if (check1 == nullptr) return nullptr; 
    ingang->left = check1;
    std::cout << "test: " << ingang->var << std::endl;
    ingang = peek();
    std::cout << "expr var: " << ingang->var << std::endl;
    ingang->right = expr1(ingang);
    std::cout << "expr return: " << ingang->var << std::endl;
    return ingang;
}

Token* ASTree::lexpr(Token* ingang) {
        std::cout << ingang->var << std::endl;
        if (positie >= size) {
            std::cout << "Er mist een expressie" << std::endl;
            exit(1);
        }
        ingang = peek();
        // std::cout << ingang->type << std::endl;
        if (ingang->type == Token::HAAKJESLUIT) {
            std::cout << "Er mist een expressie" << std::endl;
            haakje--;
            exit(1);
        }
        if (ingang->type == Token::SLASH) {
            positie++;
            ingang->left = var();
            expr(ingang->right);
        }
        else {
            std::cout << "here" << std::endl;
            ingang = pexpr(ingang);
			std::cout << "pexpr: " << ingang->var << std::endl;
			return ingang;
        }
    }

    Token* ASTree::expr1(Token* ingang) {
        std::cout << ingang->var << std::endl;
        Token* check;
        std::cout << "expr1" << std::endl;
        if (positie >= size) {
            std::cout << "expr1 size" << std::endl;
            return nullptr;
        }
        check = peek();
        std::cout << check->var << std::endl;
        if (check->type == Token::HAAKJESLUIT) {
            std::cout << "Expr1 sluit" << std::endl;
            haakje--;
            return nullptr;
        }
        return expr(ingang);
    }

    Token* ASTree::pexpr(Token* ingang) {
        std::cout << ingang->var << std::endl;
        Token* check;

        std::cout << "pexpr" << std::endl;
        if (positie >= size) {
            exit(1);
        }
        ingang = peek();
        std::cout << "test1: " << ingang->var << std::endl;
        if (ingang->type == Token::VARIABELE) {
            std::cout << "variabele" << std::endl;
            return var();
        }
        else if (ingang->type == Token::APPLICATION) {
            std::cout << "haakje open" << std::endl;
            positie++;
            haakje++;
            check = peek();
            ingang->left = expr(check);
            if (positie >= size) {
                std::cout << "te weinig sluit haakjes" << std::endl;
                exit(1);
            }
            check = peek();
            if (check->type == Token::HAAKJESLUIT) {
                std::cout << check->var << std::endl;
                positie++;
                std::cout << "returning:" << ingang->var << std::endl;
                return ingang;
            }
            return ingang;
            
        }
        return nullptr;
    }

    Token* ASTree::var() {
        std::cout << "var" << std::endl;
        Token* check;
        if (positie >= size) {
            return nullptr;
        }
        check = peek();
        positie++;
        if (check->type == Token::HAAKJEOPEN) haakje++;
        if (check->type == Token::HAAKJESLUIT) haakje--;
        else if (check->type != Token::VARIABELE) {
            std::cout << "geen variabele type" << std::endl;
            exit(1);
        }
        std::cout << check->var << std::endl;
        return check;
    }

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
