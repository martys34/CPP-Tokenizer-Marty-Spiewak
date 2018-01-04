//
// Created by Marty Spiewak on 1/3/18.
//

#ifndef C_INTERPRETER_MARTY_SPIEWAK_TOKENIZER_H
#define C_INTERPRETER_MARTY_SPIEWAK_TOKENIZER_H

#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "Token.cpp"

using namespace std;

class Tokenizer {
public:
    ifstream fileStream;
    stack<char> ungetStack;
    unordered_map<string, Token::TokenType> keywords;
    unordered_set<char> hexSet;
    unordered_set<char> suffixes;

    char tk[10000];

    Tokenizer(string filePath);

    void loadFile(string path);
    void start();
    char getNextChar();
    void ungetChar(char c);
    Token* nextToken();
};


#endif //C_INTERPRETER_MARTY_SPIEWAK_TOKENIZER_H
