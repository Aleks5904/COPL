#include "AST.h"
using namespace std;

Token::Token(Type t,const string& s): expression(s), type(t) {}

ASTree::ASTree(const string& i, Token* newRoot): input(i), treeRoot(newRoot)
{treeRoot->type = Token::EXPRESSION;}


ASTree::ASTree() : treeRoot(nullptr) {}

string ASTree::getRoot(){
    return treeRoot->expression;;
}

// Token* ASTree::expr(){
        
// }

Token* ASTree::parse(const string& input) {
    int i = 0;
    return parseExpression(input, i);
}

Token* ASTree::parseExpression(string input, int& i) {
    if (input[i] == ' ') i++;
    if (i < input.size() && input[i] == '\\') {
        return parseLambda(input, i);
    } else if (i < input.size() && isalpha(input[i])) {
        return parseVariable(input, i);
    } else if (i < input.size() && input[i] == '(') {
        i++; // skip the (
        std::cout <<  "test0" << std::endl;
        Token* left = parseExpression(input, i);
        if (i < input.size() && input[i] == ')') {
            i++;
            return left;
        } 
    } 
    return nullptr;
}

Token* ASTree::parseVariable(string input, int& i) {
    string varName = "";
    while (i < input.size() && isalpha(input[i])) {
        varName += input[i];
        i++;
    }
    return new Token(Token::VARIABLE, varName);
}

Token* ASTree::parseLambda(string input, int& i) {
    Token* lambda = new Token(Token::LAMBDA, "\\");
    i++; // Move past the '\'
    if (i < input.size() && isalpha(input[i])) {
        string param = "";
        while (i < input.size() && isalpha(input[i])) {
            param += input[i];
            i++;
            std::cout << "isalpha" << std::endl;
            Token* t = new Token(Token::EXPRESSION, param);
            lambda->left = t;
        }
        std::cout << param << std::endl;
        if (i < input.size() && input[i] == ' ') {
            i++; // Move past the space
            Token* body = parseExpression(input, i);
            lambda->right = body;
            return lambda;
        }
    }
    return nullptr;
}