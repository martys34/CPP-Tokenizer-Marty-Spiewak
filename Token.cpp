//
// Created by Marty Spiewak on 12/27/17.
//

#include "Token.h"

using namespace std;

Token::Token(TokenType type, char value[], int end) {
    token = type;
    if(value != nullptr) {
        string temp;
        for(int i = 0; i < end; i++) {
            temp += value[i];
        }
        lexeme = temp;
    }
    else {
        lexeme = nullptr;
    }
}

string Token::getLexeme() {
    return lexeme;
}

void Token::setLexeme(string lex) {
    lexeme = lex;
}

Token::TokenType Token::getToken() {
    return token;
}

void Token::setToken(TokenType tk) {
    token = tk;
}



string Token::toString() {
    string result = "Token: ";
    result += enumString[token];
    result += ", LEXEME: ";
    result += lexeme;
    return result;
}
