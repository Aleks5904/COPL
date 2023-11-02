#include "AST.h"
#include <iostream>
#include "token.h"
using namespace std;

ASTree::ASTree(string invoer) {
    input = invoer; // de doorgegevn invoer
	tokenize();
}

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
	positie = -1;
	haakje = 0;
    treeRoot = tokens[0];
	treeRoot = expr(nullptr);
    return treeRoot;
} // ASTree::checkExpression

Token* ASTree::expr(Token* ingang){
    std::cout << "expr" << std::endl;
    std::cout << positie << std::endl;
    Token* temp = lexpr(ingang);
    if (temp == nullptr) {
        std::cout << "lexpr is leeg" << std::endl;
        exit(1);
    }
    ingang = expr1(temp);
    return ingang;
}

Token* ASTree::expr1(Token* ingang){
    std::cout << "expr1" << std::endl;
    std::cout << positie << std::endl;
    Token* temp = lexpr(ingang);
    if (temp != nullptr) {
        ingang = expr1(temp);
        return ingang;
    }
    return ingang;
}

Token* ASTree::lexpr(Token* ingang){
    std::cout << "lexpr" << std::endl;
    std::cout << positie << std::endl;
    positie++;
    Token* huidig = peek();
    Token* temp = pexpr();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cout << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(temp != nullptr){
        if (ingang == nullptr) {
            return temp;
        }
        Token* tok = new Token();
        tok->type = Token::APPLICATION;
        tok->var = "@";
        tok->left = ingang;
        tok->right = temp;
        return tok;
    }
    else if (huidig->type == Token::SLASH)
    {
        Token* lambda = new Token();
        lambda->type = Token::SLASH;
        lambda->var = "\\";
        positie++;
        huidig = peek();
        if (huidig->type == Token::VARIABELE)
        {
           lambda->left = huidig;
           temp = lexpr(nullptr);
           if (temp == nullptr)
           {
             std::cout << "no expr in lambda abstraction" << std::endl;
             exit(1);
           }
           lambda->right = temp;
           if (ingang == nullptr)
           {
               return lambda;
           }
            // deleteSubtree(lambda);
            Token* tok = new Token();
            tok->type = Token::APPLICATION;
            tok->var = "@";
            tok->left = ingang;
            tok->right = lambda;
            return tok;
           
        }
        else
        {
            std::cout << "no var in lambda abstraction" << std::endl;
            exit(1);
        }
    }
    else
    {
        return nullptr;
    }
}

Token* ASTree::pexpr() {
    std::cout << "pexpr" << std::endl;
    std::cout << positie << std::endl;
    Token* huidig = peek();
    if (haakje == 0 && huidig->type == Token::HAAKJESLUIT){
        std::cout << "geen opende haakje" << std::endl;
        exit(1);
    }
    if(huidig->type == Token::VARIABELE) return huidig;
    else if(huidig->type == Token::HAAKJEOPEN){
        haakje++;
        Token* temp = expr(nullptr);
        huidig = peek();
        if(huidig->type == Token::HAAKJESLUIT){
            haakje--;
            return temp;
        }
        else
        {
            std::cout << "geen sluitende haakje" << std::endl;
            deleteSubtree(temp);
            exit(1);
        }
        
    } else{
        return nullptr;
    }
}

Token* ASTree::peek(){
	return tokens[positie];
}; // ASTree::peek

void ASTree::printBoom(Token* ingang){
    if (ingang->type == Token::VARIABELE)
    {
        std::cout <<  ingang->var;
        return;
    }
    else if(ingang->type == Token::SLASH){
         std::cout << "(";
        std::cout << ingang->var;
        printBoom(ingang->left);
        std::cout << " ";
        printBoom(ingang->right);
        std::cout << ")";
    }
    else if(ingang->type == Token::APPLICATION){
        std::cout << "(";
        printBoom(ingang->left);
        std::cout << " ";
        printBoom(ingang->right);
        std::cout << ")";
    }
}

void ASTree::freeVector(){
    for (int i = 0; i < tokens.size(); i++){
        // only vars are used in tree creation
        if (tokens[i]->type != Token::VARIABELE){ 
            delete tokens[i];
            tokens[i] = nullptr; 
        }
    }
} // ASTree::freeVector

void ASTree::deleteSubtree(Token* ingang){
    if (ingang)
    {
        // go to the left child
        if (ingang->left != nullptr) deleteSubtree(ingang->left);
        
        // go to the right child
        if(ingang->right != nullptr) deleteSubtree(ingang->right);
        
    }    
    delete ingang;
    ingang = nullptr;
} // Tree::deleteSubtree

Token* ASTree::copySubtree(Token* ingang) {
    if (!ingang) return nullptr; // empty subtree
    
    Token* copy = new Token; // Token used to copy the tree
    copy -> var = ingang -> var;
    copy -> type = ingang -> type; // copy the type

    if (ingang -> left != nullptr) { // go to the left child
        copy -> left = copySubtree(ingang -> left);
        }

    if (ingang -> right != nullptr) { // go to the right child
        copy -> right = copySubtree(ingang -> right);
        }
    return copy;
} // Tree::copySubtree

bool ASTree::findGivenVar(Token* ingang, string variable) {
    if (ingang == nullptr) 
        return false; 

    if (ingang->var == variable) 
        return true; 

    bool leftResult = findGivenVar(ingang->left, variable);
    if (leftResult) 
        return true; 

    bool rightResult = findGivenVar(ingang->right, variable);
    if (rightResult) 
        return true;

    return false;
}

Token* ASTree::findLambda(Token* ingang) {
    if (ingang == nullptr) {
        return nullptr; 
    }

    if (ingang->type == Token::SLASH) {
        return ingang->left;  
    }

    Token* leftResult = findLambda(ingang->left);
    if (leftResult != nullptr) {
        return leftResult; 
    }

    Token* rightResult = findLambda(ingang->right);
    if (rightResult != nullptr) {
        return rightResult; 
    }  
    return nullptr;
}

Token* ASTree::postOrder(Token* ingang) {
    if (ingang == nullptr) {
        return ingang;  // Base case: If the current node is null, exit the function.
    }

    // Recursively process the left and right subtrees in a post-order manner
    ingang->left = postOrder(ingang->left);
    ingang->right = postOrder(ingang->right);

    // Check if the current node meets the condition for processing
    if (ingang->type == Token::APPLICATION && ingang->left != nullptr
        && ingang->left->type == Token::SLASH) {
        while (ingang->type == Token::APPLICATION && ingang->left != nullptr
            && ingang->left->type == Token::SLASH) {
            ingang = betaReduction(ingang);
        }
    }

    return ingang;
}


Token* ASTree::betaReduction(Token* ingang){
    std::cout << "beta found" << std::endl;
    int numRight = 0;
    bool toTheRight = false;
    Token* antwoord = nullptr;
    Token* copy = nullptr;
    Token* ingang2 = nullptr;
    if (ingang != nullptr)
    {
        Token* N = copySubtree(ingang->right);
        deleteSubtree(ingang->right);
        ingang->right = nullptr;
        copy = ingang;
        ingang = ingang->left;
        Token* alfa = findLambda(ingang->right);
        if (alfa != nullptr) // possible bound variable
        {
            bool bound = findGivenVar(N, alfa->var);
            if (bound)
            {
                alfa->var += "'";
            }
        }
        while (ingang->right->type != Token::VARIABELE)
        {
            ingang = ingang->right;
            std::cout << "toTheRight" << std::endl;
            toTheRight = true;
        }
        delete ingang->right;
        ingang->right = nullptr;
        ingang->right = N;
        
        if (toTheRight){
            std::cout << "return #1" << std::endl;
            ingang2 = copySubtree(ingang);
        }

        else {
            std::cout << "return #3" << std::endl;
            ingang2 = copySubtree(ingang->right);
        }
        deleteSubtree(copy);
        ingang = ingang2;
        return ingang;
    }
    return nullptr;

    // ingang = aantwoord
    // returnn aantwoord
} // ASTree::betaReduction

