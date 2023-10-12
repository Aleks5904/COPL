#include "AST.h"
using namespace std;

Token::Token(string s): expressie(s) {}

ASTree::ASTree(const string& i, Token* newRoot): input(i), treeRoot(newRoot)
{treeRoot->type = Token::EXPRESSION;}


ASTree::ASTree() : treeRoot(nullptr) {}

string ASTree::getRoot(){
    return treeRoot->expressie;
}

Token* ASTree::parseExpression(string input, int& i){
    if (input[i] == ' ') i++; 
    if (input[i] == '\\')
    {
        i++; // move past '\'
        string varName = "";
        if (input[i] < input.size() && isalnum(input[i])){ 
            while (input[i] < input.size() && isalnum(input[i]))
                varName += input[i];
        }
        Token* var = new Token(varName); 
        var->type = Token::VARIABLE;
        Token* lambda = new Token("\\");
        lambda->type = Token::LAMBDA;
        lambda->left = var;
        lambda->right = parseExpression(input, i);
        return lambda;
    }
    else if (input[i] == '(')
    {
        i++; // move past (
        Token* subExpr = parseExpression(input, i);
        if (input[i] == ')'){
            i++;
            return subExpr;
        } 
    }
    else if(isalnum(input[i])){
        string varName = "";
        while (i < input.size() && isalnum(input[i]))
        {
            varName += input[i];
            i++;
        }
        Token* var = new Token(varName);
        var->type = Token::VARIABLE;
        return var;
    }
} // ASTree::parseExpression