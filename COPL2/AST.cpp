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
    Token* temp = treeRoot;
	treeRoot = expr(nullptr);
    return treeRoot;
} // ASTree::checkExpression

// <expr>  ::= <lexpr><expr1>
// <expr1> ::= <expr>          ||empty
// <lexpr> ::= '\'<var><expr>  ||<pexpr>
// <pexpr> ::= <var>           || '('<expr>')'

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
        tok->left = ingang;
        tok->right = temp;
        return tok;
    }
    else if (huidig->type == Token::SLASH)
    {
        Token* lambda = new Token();
        lambda->type = Token::SLASH;
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
            Token* tok = new Token();
            tok->type = Token::APPLICATION;
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
        std::cout << "\\";
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

Token* ASTree::startingPoint(Token* ingang) {
    if (ingang != nullptr) {
        if (ingang->type == Token::APPLICATION && ingang->left != nullptr && ingang->left->type == Token::SLASH) {
            return ingang;
        }
        
        // Recursively check the left and right subtrees.
        Token* leftResult = startingPoint(ingang->left);
        Token* rightResult = startingPoint(ingang->right);
        
        // If a match is found in the left subtree, return it.
        if (leftResult != nullptr) {
            return leftResult;
        }
        
        // If a match is found in the right subtree, return it.
        if (rightResult != nullptr) {
            return rightResult;
        }
    }
    
    // If no match is found in this subtree, return nullptr.
    return nullptr;
}

bool ASTree::findAbstraction(Token* ingang){
    if (ingang != nullptr)
        if (ingang->type == Token::SLASH) return true;
    return false;
} // ASTree::findAbstraction

Token* ASTree::copySubtree(Token* const ingang) {
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

Token* ASTree::alfaConversion(Token* ingang){
    int asciiValue = 97; // starting with A
    if (ingang != nullptr)
    {
       if (ingang->left->var == ingang->right->right->var)
       {
        std::cout << "alfa" << std::endl;
        while (ingang->left->var == ingang->right->right->var)
        {
            ingang->left->var = static_cast<char>(asciiValue);
            asciiValue++;
        }
       }
       
    }
    return ingang;
}

Token* ASTree::wrapper(Token* ingang){
    int numOfIterations = 0;
    bool cycle = false;
    ingang = betaReduction(ingang);
    if (ingang != nullptr)
    {
        if (ingang->left != nullptr) {
            cycle = findAbstraction(ingang->left);
            std::cout << "[1]cycle: " << cycle << std::endl;
        }
            
        if (cycle == false && ingang->right->left != nullptr){
            cycle = findAbstraction(ingang->right->left);  
            std::cout << "[2]cycle: " << cycle << std::endl;
        }
                 
        while (cycle && numOfIterations < 3) // perform max 3 beta-reductions
        {
            std::cout << "while" << std::endl;
            numOfIterations++;
            // std::cout << "copy" << std::endl;
            // printBoom(ingang);
            // std::cout << "copy" << std::endl;
            // ingang = betaReduction(ingang);
        }
        return ingang;
    }
} // ASTree::wrapper

Token* ASTree::betaReduction(Token* ingang){
    std::cout << "test0";
    int numOfSlash = 0;
    int i = 0;
    Token* t;
    std::cout << "test1";
    Token* temp = copySubtree(ingang); // make a copy of the current tree
    std::cout << "test2";
    Token* extraKind;
    std::cout << std::endl;
    if (temp != nullptr && temp->left != nullptr)
    {   
        if (temp->type != Token::APPLICATION || temp->left->type != Token::SLASH)
        {
            i = 1;
            std::cout << "@ and \\ searching" << std::endl;
            temp = startingPoint(ingang);
        }
        // while (!(temp->type == Token::APPLICATION 
        // && temp->left->type == Token::SLASH)) // find @ and \ together
        // {
        //   i++;
        //   temp = temp->left;
        //   std::cout << "@ and \\ searching" << std::endl;
        // }
        if (ingang->right->type == Token::APPLICATION ||
                ingang->right->type == Token::SLASH )
            {
                t = copySubtree(ingang->right); // N
                std::cout << "i: " << i << std::endl;
                if (i == 0) temp = temp->left; // for simple tree move one level in depth

                while (temp->right->type != Token::VARIABELE) // not sure abt this statement
                {
                    temp = temp->right;
                    if (temp->type == Token::SLASH) numOfSlash++;
                    std::cout << "temp->right is nog geen var" << std::endl;
                }
                deleteSubtree(temp->right);
                temp->right = t;
                if (temp->left->type == Token::SLASH) // meerdere kinderen
                {
                    extraKind = temp->left->right; // sla tweede kind op
                    deleteSubtree(temp->left);
                    temp->left = extraKind;

                }                
                if (temp->type == Token::APPLICATION || numOfSlash > 0){
                    std::cout << "moeilijke tree" << std::endl;
                    if (temp->type == Token::SLASH)
                    {
                        Token* t = alfaConversion(temp);
                        return t;
                    }
                    else return temp;
                }
                    
                else {
                    std::cout << "makkelijke tree" << std::endl;
                    return temp->right;
                }
                    
            } 
    }
    std::cout << "nullptr" << std::endl;
    return nullptr;
} // ASTree::betaReduction