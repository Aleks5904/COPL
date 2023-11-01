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

Token* ASTree::startingPoint(Token* ingang) {
    if (ingang != nullptr) {
        if (ingang->type == Token::APPLICATION && ingang->left != nullptr && ingang->left->type == Token::SLASH) {
            return ingang;
        }
        if (ingang->left != nullptr) return startingPoint(ingang->left);
      
    }
    
    // If no match is found in this subtree, return nullptr.
    return nullptr;
}

bool ASTree::findAbstraction(Token* ingang) {
    if (ingang != nullptr) {
        if (ingang->type == Token::SLASH) {
            return true;
        }
        if (ingang->left != nullptr) {
            // Return the result of the recursive call.
            if (findAbstraction(ingang->left)) {
                return true;
            }
        }
    }
    std::cout << "here";
    return false;
}


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
    return ingang;
    if (ingang != nullptr)
    {
        if (ingang->left != nullptr) {
            cycle = findAbstraction(ingang->left);
            std::cout << "[1]cycle: " << cycle << std::endl;
        }
        while(cycle && numOfIterations < 3) // perform max 3 beta-reductions
        {
            numOfIterations++;
            cycle = false;
            std::cout << "while" << std::endl;
            numOfIterations++;
            std::cout << "copy" << std::endl;
            printBoom(ingang);
            std::cout << "copy" << std::endl;
            ingang = betaReduction(ingang);
            if (ingang->left != nullptr) {
                printBoom(ingang->left);
                cycle = findAbstraction(ingang->left);
            }
        }
        return ingang;
    }
} // ASTree::wrapper

// Token* ASTree::betaReduction(Token* ingang){
//     bool extraStep = false;
//     bool toTheRight = false;
//     Token* extraChild = nullptr;
//     Token* parent;
//     parent->type = Token::APPLICATION;
//     parent->var = "@";
//     Token* copy = copySubtree(ingang);
//     Token* copy2;
//     Token* temp;
//     if(startingPoint(ingang) == nullptr) {
//         std::cout << "no beta reduction can be performed" << std::endl;
//         deleteSubtree(treeRoot);
//         freeVector();
//         // delete parent;
//         exit(0);
//     }
//     if (ingang != nullptr)
//     {
//         Token* N = copySubtree(copy->right);
//         deleteSubtree(copy->right);
//         if (ingang != nullptr && ingang->left != nullptr)
//         {
//             if (ingang->type != Token::APPLICATION || ingang->left->type != Token::SLASH)
//             {
//                 temp = ingang;
//                 copy = startingPoint(copy);
//                 std::cout << "extra step" << std::endl;
//                 extraStep = true;
//             }
//         }
//         if (extraStep)
//         {
//             extraChild = copy->right;
//         }
//         copy = copy->left;
//         while (copy->right->type != Token::VARIABELE)
//         {
//             copy = copy->right;
//             std::cout << "toTheRight" << std::endl;
//             toTheRight = true;
//         }
//         if (extraChild == nullptr){
//             delete copy->right;
//             copy->right = N;
//         }
//         if (extraChild != nullptr) // we have an additional child
//         {
//             std::cout << "creating parent" << std::endl;
//             parent->left = N;
//             parent->right = extraChild;
//             copy = parent;
//         }
//         deleteSubtree(ingang);
//         if (toTheRight){
//             std::cout << "return #1" << std::endl;
//             if (copy->right->type = Token::SLASH)
//                 copy = alfaConversion(copy);
//             treeRoot = copy;
//             return treeRoot;
//         }
//         if(extraChild != nullptr){
//             std::cout << "return #2" << std::endl;
//             treeRoot = copy;
//             return treeRoot;
//         }
//         else {
//             std::cout << "return #3" << std::endl;
//             treeRoot = copy->right;
//             return treeRoot;
//         }
//     }
//     return nullptr;
// } // ASTree::betaReduction

Token* ASTree::betaReduction(Token* ingang){
    bool extraStep = false;
    bool toTheRight = false;
    Token* extraChild = nullptr;
    Token* parent;
    parent->type = Token::APPLICATION;
    parent->var = "@";
    Token* temp;
    if(startingPoint(ingang) == nullptr) {
        std::cout << "no beta reduction can be performed" << std::endl;
        deleteSubtree(treeRoot);
        freeVector();
        // delete parent;
        exit(0);
    }
    if (ingang != nullptr)
    {
        Token* N = copySubtree(ingang->right);
        deleteSubtree(ingang->right);
        if (ingang != nullptr && ingang->left != nullptr)
        {
            if (ingang->type != Token::APPLICATION || ingang->left->type != Token::SLASH)
            {
                // temp = copySubtree(ingang);
                // deleteSubtree(ingang);
                temp = ingang;
                ingang = startingPoint(temp);
                std::cout << "extra step" << std::endl;
                extraStep = true;
            }
        }
        if (extraStep)
        {
            extraChild = ingang->right;
        }
        ingang = ingang->left;
        while (ingang->right->type != Token::VARIABELE)
        {
            ingang = ingang->right;
            std::cout << "toTheRight" << std::endl;
            toTheRight = true;
        }
        if (extraChild == nullptr){
            delete ingang->right;
            ingang->right = N;
        }
        if (extraChild != nullptr) // we have an additional child
        {
            std::cout << "creating parent" << std::endl;
            parent->left = N;
            parent->right = extraChild;
            ingang = parent;
        }
        if (toTheRight){
            std::cout << "return #1" << std::endl;
            if (ingang->right->type == Token::SLASH)
                ingang = alfaConversion(ingang);
            // delete parent;
            treeRoot = ingang;
            return treeRoot;
        }
        if(extraChild != nullptr){
            std::cout << "return #2" << std::endl;
            deleteSubtree(N);
            treeRoot = ingang;
            return treeRoot;
        }
        else {
            std::cout << "return #3" << std::endl;
            // delete parent;
            treeRoot = ingang->right;
            ingang->right = nullptr;
            return treeRoot;
        }
    }
    return nullptr;
} // ASTree::betaReduction

