#include "AST.h"
using namespace std;

Token::Token(string s): expressie(s) {}

ASTree::ASTree(const string& i, Token* newRoot): input(i), treeRoot(newRoot)
{}

ASTree::ASTree() : treeRoot(nullptr) {}

string ASTree::getRoot(){
    return treeRoot->expressie;
}