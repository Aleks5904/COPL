#include <iostream>
#include <vector>
#include <string>
using namespace std;

//FUCK GIT !!!
//Deze functie checkt of een ingevoerd karakter is toegestaan voor een variabele
//first =  true geeft aan dat het het eerste karakter is
bool CharInSet(char input, bool first){
	if(input >= 'a' && input <= 'z')
		return true;
	if(input >= 'A' && input <= 'Z')
		return true;
	if(input >= '0' && input <= '9' && !first)
		return true;
	return false;
}

//struct token stores what kind expression it is.
//Union is used to store a variable string var
struct Token{
	enum{
		VARIABELE,
		HAAKJEOPEN,
		HAAKJESLUIT,
		SLASH	
	} type;

	union 
	{
		string var;
		
	};

	Token() : type(VARIABELE), var("") {} // Initialize type to VARIABLE and var to an empty string
};

//This is the parser used to test the syntax of the input
class Parser{
	public:
		Parser();				//default constructor because it is needed
		Parser(string invoer);	//Actual input constructor that takes the input string
		
		bool checkExpression();
		void print();
	private:
		//Recursive descent functions
			
			//Step 1 check if there is a expression and if it is there call lexpr and expr1
			bool expr();
			
			//step 2 check if there is another expression or if it is empty. If not empty call itself and lexpr
			bool expr1();

			//step 3 check if there is a \ or labda. If there is call var and lexpr. if not call p expr
			bool lexpr();

			//step 4 check if there is a variable. If not call expr and store depth of the brackets.
			bool pexpr();

			//check if the var is allowed
			bool var();
			

		//Turn the input string into tokens. If token is var. Store that as well.
		void tokenize();

		string input;
		size_t index;
		
		//positie geeft de plaats in de token vector aan.
		int positie;
		int haakje;
		int haakjeArr[50];
		//A vector class of token pointers used to store all the tokens we find.
		vector<Token*> tokens;

		//Return the token pointer on positie 
		Token* peek();
};

Parser::Parser(){
}

Parser::Parser(string invoer){
	input = invoer;
	positie = 0;
	haakje = 0;

	tokenize();
	for (int i = 0; i < tokens.size(); i++)
	{
		haakjeArr[i] = false;
	}
	
}

void Parser::tokenize(){
	int size = input.size();
	for (int i = 0; i < size; i++){
		Token* t = new Token;
		if (input[i] == '('){
			t->type = Token::HAAKJEOPEN;
			tokens.push_back(t);
		}
		else if(input[i] == ')'){
			t->type = Token::HAAKJESLUIT;
			tokens.push_back(t);
		}
		else if(input[i] == '\\'){
			t->type = Token::SLASH;
			tokens.push_back(t);
		}
		else if(input[i] != ' ') {
			string variable = "";
			variable.push_back(input[i]);
			t->type = Token::VARIABELE;
			while (input[i+1] != ' ' && input[i+1] != '(' && input[i+1] != ')' && input[i+1] != '\\' && i != size-1){
				i++;
				variable.push_back(input[i]);
			}
			t->var = variable;
			tokens.push_back(t);
		}
		else if(input[i] == ' '){
			continue;
		}
	} // for
	// size = tokens.size();
	// for (int i = 0; i < size; i++){
	// 	cout << tokens[i]->type;
	// }
} // Parser::tokenize

bool Parser::checkExpression(){
	return expr();
}

bool Parser::expr(){
	cout << "expr: " << positie << endl;
	bool check1 = false, check2 = false;
	check1 = lexpr();
	check2 = expr1();
	return(check1 && check2);
}

bool Parser::lexpr(){
	cout << "lexpr: " << positie << endl;
	Token * check;
	int size = tokens.size();
	bool check1 = false, check2 = false;
	if(positie >= size){
		return false;
	}
	check = peek();
	if(check->type == Token::SLASH){
		positie++;
		check1 = var();
		check2 = lexpr();
		return(check1 && check2);
	}
	else if(check->type == Token::HAAKJESLUIT){
		if(haakjeArr[positie] == false){
			haakje++;
			haakjeArr[positie] = true;
			std::cout << "positie!: " << positie << std::endl;
		}
		positie ++;
		return false;
	}
	else{
		return pexpr();
	}
}

bool Parser::expr1(){
	cout << "expr1: " << positie << endl;
	Token * check;
	int size = tokens.size();
	bool check1 = false, check2 = false;
	if(positie >= size){
		return true;
	}
	check = peek();
	//Haakjesluit dus lege input
	if(check->type == Token::HAAKJESLUIT){
		if(haakjeArr[positie] == false){
			haakje--;
			haakjeArr[positie] = true;
			std::cout << "positie: " << positie << std::endl;
		}
		std::cout << "haakje: " << haakje << std::endl;
		if (haakje < 0 && positie >= tokens.size()-1)
		{
			cerr << "teveel sluit-haakjes" << endl;
			return false;
		}
		return true;
	}
	else{ //empty
		check1 = lexpr();
		check2 = expr1();
		return(check1 && check2);
	}
}

bool Parser::pexpr(){
	cout << "pexpr: " << positie << endl;
	Token * check = peek();
	int size = tokens.size();
	bool check1 = false, check2 = false;
	if(check->type == Token::VARIABELE){
		return var();
	}
	if(check->type == Token::HAAKJEOPEN){
		if(haakjeArr[positie] == false){
			haakje++;
			haakjeArr[positie] = true;
			std::cout << "positie: " << positie << std::endl;
		}
		positie++;
		
		std::cout << "haakje: " << haakje << std::endl;
		check1 = expr();
	}
	if(positie >= size){
		return false;
	}
	check = peek();
	if(check->type == Token::HAAKJESLUIT && check1){
		if(haakjeArr[positie] == false){
			haakje--;
			haakjeArr[positie] = true;
			std::cout << "positie: " << positie << std::endl;
		}
		positie++;
		
		std::cout << "haakje: " << haakje << std::endl;
		check2 = true;
	}
	return(check1 && check2);
}

bool Parser::var(){
	Token * check = peek();
	string var;
	int size = tokens.size();
	if(positie >= size){
		return false;
	}
	check = peek();
	positie++;
	if(check->type != Token::VARIABELE){
		return false;
	}
	var = check->var;
	size = var.size();
	for(int i = 0; i < size; i++){
		if(!CharInSet(var[i], i == 0)){
			cout << "var false" << endl;
			return false;
		}
	}
	cout << "var true" << endl;
	return true;
}

Token* Parser::peek(){
	return tokens[positie];
};

void Parser::print(){
	int size = tokens.size();
	Token* werk;
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
		default:
			cout << " ";
			break;
		}
	}
	cout << endl;
}

int main(){
	string input;
	Parser * temp;
	cout << "voer string in" << endl;
	getline(cin, input);
	cout << input << endl;
	temp = new Parser(input);
	cout << "check \n" << temp->checkExpression() << endl;
	temp->print();
	return 0;
}//main
