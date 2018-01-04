#include "Tokenizer.h"

using namespace std;

Tokenizer::Tokenizer(string filePath) {
loadFile(filePath);
}

void Tokenizer::loadFile(string path) {
fileStream.open(path);
if(fileStream.good()) {
printf("%s", "File has been loaded.\n");
}
}

void Tokenizer::start() {
    keywords["auto"] = Token::Token::TokenType::AUTO;
    keywords["double"] = Token::TokenType::DOUBLE;
    keywords["int"] = Token::TokenType::INT;
    keywords["struct"] = Token::TokenType::STRUCT;
    keywords["const"] = Token::TokenType::CONST;
    keywords["float"] = Token::TokenType::FLOAT;
    keywords["short"] = Token::TokenType::SHORT;
    keywords["unsigned"] = Token::TokenType::UNSIGNED;
    keywords["break"] = Token::TokenType::BREAK;
    keywords["else"] = Token::TokenType::ELSE;
    keywords["long"] = Token::TokenType::LONG;
    keywords["switch"] = Token::TokenType::SWITCH;
    keywords["continue"] = Token::TokenType::CONTINUE;
    keywords["for"] = Token::TokenType::FOR;
    keywords["signed"] = Token::TokenType::SIGNED;
    keywords["void"] = Token::TokenType::VOID;
    keywords["case"] = Token::TokenType::CASE;
    keywords["enum"] = Token::TokenType::ENUM;
    keywords["register"] = Token::TokenType::REGISTER;
    keywords["typedef"] = Token::TokenType::TYPEDEF;
    keywords["default"] = Token::TokenType::DEFAULT;
    keywords["goto"] = Token::TokenType::GOTO;
    keywords["sizeof"] = Token::TokenType::SIZEOF;
    keywords["volatile"] = Token::TokenType::VOLATILE;
    keywords["char"] = Token::TokenType::CHAR;
    keywords["extern"] = Token::TokenType::EXTERN;
    keywords["return"] = Token::TokenType::RETURN;
    keywords["union"] = Token::TokenType::UNION;
    keywords["do"] = Token::TokenType::DO;
    keywords["if"] = Token::TokenType::IF;
    keywords["static"] = Token::TokenType::STATIC;
    keywords["while"] = Token::TokenType::WHILE;
    keywords["alignof"] = Token::TokenType::ALIGNOF;
    keywords["and"] = Token::TokenType::AND;
    keywords["and_eq"] = Token::TokenType::AND_EQ;
    keywords["asm"] = Token::TokenType::ASM;
    keywords["bool"] = Token::TokenType::BOOL;
    keywords["compl"] = Token::TokenType::COMPL;
    keywords["const_cast"] = Token::TokenType::CONST_CAST;
    keywords["delete"] = Token::TokenType::DELETE;
    keywords["dynamic_cast"] = Token::TokenType::DYNAMIC_CAST;
    keywords["exit"] = Token::TokenType::EXIT;
    keywords["explicit"] = Token::TokenType::EXPLICIT;
    keywords["export"] = Token::TokenType::EXPORT;
    keywords["false"] = Token::TokenType::FALSE;
    keywords["friend"] = Token::TokenType::FRIEND;
    keywords["inline"] = Token::TokenType::INLINE;
    keywords["mutable"] = Token::TokenType::MUTABLE;
    keywords["namespace"] = Token::TokenType::NAMESPACE;
    keywords["not"] = Token::TokenType::NOT;
    keywords["not_eq"] = Token::TokenType::NOT_EQ;
    keywords["operator"] = Token::TokenType::OPERATOR;
    keywords["or"] = Token::TokenType::OR;
    keywords["or_eq"] = Token::TokenType::OR_EQ;
    keywords["private"] = Token::TokenType::PRIVATE;
    keywords["protected"] = Token::TokenType::PROTECTED;
    keywords["public"] = Token::TokenType::PUBLIC;
    keywords["reinterpret_cast"] = Token::TokenType::REINTERPRET_CAST;
    keywords["static_cast"] = Token::TokenType::STATIC_CAST;
    keywords["template"] = Token::TokenType::TEMPLATE;
    keywords["this"] = Token::TokenType::THIS;
    keywords["throw"] = Token::TokenType::THROW;
    keywords["true"] = Token::TokenType::TRUE;
    keywords["typeid"] = Token::TokenType::TYPEID;
    keywords["typename"] = Token::TokenType::TYPENAME;
    keywords["using"] = Token::TokenType::USING;
    keywords["virtual"] = Token::TokenType::VIRTUAL;
    keywords["wchar_t"] = Token::TokenType::WCHAR_T;
    keywords["xor"] = Token::TokenType::XOR;
    keywords["xor_eq"] = Token::TokenType::XOR_EQ;



    hexSet.insert('a');
    hexSet.insert('b');
    hexSet.insert('c');
    hexSet.insert('d');
    hexSet.insert('e');
    hexSet.insert('f');
    hexSet.insert('A');
    hexSet.insert('B');
    hexSet.insert('C');
    hexSet.insert('D');
    hexSet.insert('E');
    hexSet.insert('F');

    suffixes.insert('u');
    suffixes.insert('U');
    suffixes.insert('l');
    suffixes.insert('L');
    suffixes.insert('f');
    suffixes.insert('F');
}


char Tokenizer::getNextChar() {
    char c;
    if(ungetStack.empty()) {
        fileStream.get(c);
        if(!fileStream.eof()) {
            return c;
        }
    }
    else {
        c = ungetStack.top();
        ungetStack.pop();
        return c;
    }
    return (char) -1;
}

void Tokenizer::ungetChar(char c) {
    ungetStack.push(c);
}

Token* Tokenizer::nextToken() {
    memset(tk, 0, sizeof(tk));
    //memset(testKey, 0, sizeof(testKey));
    char c;
    //index of token
    int i, state;
    char nextTest;

    //skip whitespace
    while (isspace(c = getNextChar()) || c == '\r' || c == '\n') ;

    //if at end of input stream
    if (c == (char) (-1)) {
        tk[0] = 'E';
        tk[1] = 'O';
        tk[2] = 'F';
        return new Token(Token::TokenType::END, tk, 3);
    }

    i = 0;
    tk[i++] = c;

    //if an integer or float
    if (isdigit(c) || c == '.') {
        if(c == 0) {
            char second = getNextChar();
            switch(second) {
                case 'x' :
                case 'X' :
                    state = 9;
                    break;
                default:
                    state = 11;
            }
        }
        if (c == '.') {
            char second = getNextChar();
            char third = getNextChar();
            if(second == '.') {
                if(third == '.') {
                    tk[1] = second;
                    tk[2] = third;
                    return new Token(Token::TokenType::VARIADIC, tk ,i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            state = 4;
            c = getNextChar();
            if (!isdigit(c)) {
                ungetChar(c);
                tk[0] = '.';
                second = getNextChar();
                if(second == '*') {
                    tk[1] = second;
                    return new Token(Token::TokenType::GROUP_MEM_POINTER, tk, i + 1);
                }
                ungetChar(second);
                return new Token(Token::TokenType::PERIOD, tk, 1);
            }
            ungetChar(c);
        } else state = 2;
        c = getNextChar();
        while (state > 0) {
            switch (state) {
                case 2:
                    if (isdigit(c)) state = 2;
                    else if (c == 'e' || c == 'E') state = 5;
                    else if (c == '.') state = 3;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }

                    else if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 3:
                    if (isdigit(c)) state = 8;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else if(!isalpha(c)){
                        state = -3;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 4:
                    if (isdigit(c)) state = 3;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -4;
                    break;
                case 5:
                    if (isdigit(c)) state = 6;
                    else if (c == '-' || c == '+') state = 7;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -5;
                    break;
                case 6:
                    if (isdigit(c)) state = 6;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -6;
                    break;

                case 7:
                    if (isdigit(c)) state = 6;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else state = -7;
                    break;
                case 8:
                    if (isdigit(c)) state = 8;
                    else if (c == 'e' || c == 'E') state = 5;
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    else if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }

                    break;

                case 9 :
                    if(isdigit(c) || hexSet.count(c)) {
                        state = 9;
                    }
                    else if(suffixes.count(c)) {
                        if(c == 'L') {
                            state = 12;
                        }
                        else if(c == 'l') {
                            state = 13;
                        }
                        else if(c == 'u' || c == 'U') {
                            state = 14;
                        }
                    }
                    break;

                case 10:
                    if(!suffixes.count(c)) {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }

                    break;

                case 11: {
                    int x = c - '0';
                    if (x <= 7) {
                        state = 11;
                    } else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                }
                    //seen one L
                case 12:
                    if(!suffixes.count(c) || c == 'l') {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    if(c == 'u' || c == 'U') {
                        state = 15;
                        break;
                    }
                    else {
                        state = 16;
                        break;
                    }
                    //seen one l
                case 13:
                    if(!suffixes.count(c) || c == 'L') {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    if(c == 'u' || c == 'U') {
                        //seen a u after l
                        state = 15;
                        break;
                    }
                    else {
                        state = 16;
                        break;
                    }
                    //seen a u or U before L
                case 14:
                    if(c == 'l') {
                        state = 17;
                    }
                    else if(c == 'L') {
                        state = 18;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                case 15:
                    if(!isalpha(c)){
                        state = -2;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                    //seen two L's or two l's
                case 16:
                    if(c == 'u' || c == 'U') {
                        state = 15;
                    }
                    else {
                        state = -15;
                    }
                    break;
                    //seen u/U then l
                case 17:
                    if(c == 'l') {
                        state = 15;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;
                    //seen u/u then L
                case 18:
                    if(c == 'L') {
                        state = 15;
                    }
                    else {
                        tk[i] = c;
                        return new Token(Token::TokenType::BAD_TOKEN, tk, i + 1);
                    }
                    break;

                default:
                    break;

            }
            if (state > 0) {
                tk[i++] = c;
                tk[i] = '\0';
                c = getNextChar();
            }
        }
        ungetChar(c);
        if (state == -3) ungetChar('.');
        switch (-state) {
            case 2:
                return new Token(Token::TokenType::INT, tk, i);
            case 3:
                return new Token(Token::TokenType::INT, tk, i - 1);
            case 6:
            case 8:
                return new Token(Token::TokenType::FLOAT, tk, i);
            case 4:
                break;
            case 15:
                return new Token(Token::TokenType::INT, tk, i);

            default:
                return new Token(Token::TokenType::BAD_TOKEN, tk, i);
        }
    }

    //ident starting with #
    if (c == '#') {
        i=0;
        tk[i++] = c;
        c = getNextChar();
        while (isalpha(c) || isdigit(c)) {
            tk[i++] = c;
            c = getNextChar();
        }
        ungetChar(c);
        return new Token(Token::TokenType::IDENT, tk, i);
    }

    //identifier
    if (isalpha(c) || c == '_' || c == '$') {
        c = getNextChar();
        while (isalpha(c) || isdigit(c) || c == '_' || c == '$') {
            tk[i++] = c;
            c = getNextChar();
        }
        string testKey;
        for(int x = 0; x < i; x++) {
            testKey += tk[x];
        }
        Token::TokenType type = Token::TokenType::NOPE;
        if(keywords.count(testKey)) {
            type = keywords.at(testKey);
        }
        if(type == Token::TokenType::NOPE) {
            if(strcmp(tk, "delete") == 0) {
                if(c == '[') {
                    type = Token::TokenType::DELETE;
                }
                else {
                    type = Token::TokenType::IDENT;
                }
            }
            else if(strcmp(tk, "new") == 0) {
                if(c == '[') {
                    type = Token::TokenType::NEW;
                }
                else {
                    type = Token::TokenType::IDENT;
                }
            }
            else {
                type = Token::TokenType::IDENT;
            }
        }
        ungetChar(c);
        return new Token(type, tk, i);
    }

    //identifier starting with a @
    if ( c=='@') {
        while (isdigit(c = getNextChar()) || c == ';') {
            tk[i++] = c;
        }
        ungetChar(c);
        return new Token(Token::TokenType::IDENT,tk,i);
    }

    //OLDCOMMENT
    if (tk[0] == '/' && (c = getNextChar()) == '*') {
        tk[1] = '*';

        while ((c = getNextChar()) != '*' && (c = getNextChar()) != '/') {

        }
        return new Token(Token::TokenType::LONGCOMMENT, tk, i + 1);
    }
    if (tk[0] == '/') {
        ungetChar(c);
        c = tk[0];
    }

    //LINECOMMENT
    if (tk[0] == '/' && (c = getNextChar()) == '/') {
        tk[1] = '/';

        while ((c = getNextChar()) != '\n'  && c != (char) -1) {

        }
        return new Token(Token::TokenType::LINECOMMENT, tk, i + 1);
    }
    if (tk[0] == '/') {
        ungetChar(c);
        c = tk[0];
    }

    //CONSTSTRING
    if (tk[0] == '"') {
        bool badString = false;
        while ((c = getNextChar()) != '"' || (tk[i - 1] == '\\' && tk[i - 2] != '\\')) {
            if(c == '"' || c == -1) {
                badString = true;
            }
            if (c == (char) (-1)) {
                break;
            }
            tk[i++] = c;
        }
        if(c == '"') {
            badString = false;
        }
        if(badString) {
            return new Token(Token::TokenType::BAD_TOKEN, tk, i);
        }
        char next = getNextChar();
        if(!isalpha(next)){
            ungetChar(next);
            tk[i++] = c;
            return new Token(Token::TokenType::STRING, tk, i);
        }
        else {
            tk[i] = c;
            tk[i+1] = next;
            return new Token(Token::TokenType::BAD_TOKEN, tk, i + 2);
        }
    }

    //CONSTCHAR

    if (tk[0] == '\'') {
        bool badChar = false;
        while ((c = getNextChar()) != '\'' || (tk[i - 1] == '\\' && tk[i - 2] != '\\')) {
            if(c == '\'' || c == -1) {
                badChar = true;
            }
            if (c == (char) (-1)) {
                break;
            }
            tk[i++] = c;
        }
        if(c == '\'') {
            badChar = false;
        }
        if(badChar) {
            return new Token(Token::TokenType::BAD_TOKEN, tk, i);
        }
        char next = getNextChar();
        if(!isalpha(next)){
            ungetChar(next);
            tk[i++] = c;
            return new Token(Token::TokenType::CHAR, tk, i);
        }
        else {
            tk[i] = c;
            tk[i+1] = next;
            return new Token(Token::TokenType::BAD_TOKEN, tk, i + 2);
        }
    }

    //punctation

    char first = c;
    char second;
    char third;

    tk[0] = first;

    switch(first) {

        case '(' :
            return new Token(Token::TokenType::OPEN_PARENT, tk, i);

        case ')' :
            return new Token(Token::TokenType::CLOSE_PARENT, tk, i);

        case '{' :
            return new Token(Token::TokenType::OPEN_BRACE, tk, i);

        case '}' :
            return new Token(Token::TokenType::CLOSE_BRACE, tk, i);

        case '[' :
            return new Token(Token::TokenType::OPEN_BRACKET, tk, i);

        case ']' :
            return new Token(Token::TokenType::CLOSE_BRACKET, tk, i);

        case ',' :
            return new Token(Token::TokenType::COMMA, tk, i);

        case ':' :
            second = getNextChar();
            if(second == '>') {
                tk[1] = second;
                return new Token(Token::TokenType::CLOSE_BRACKET, tk, i + 1);
            }
            if(second == ':') {
                tk[1] = second;
                return new Token(Token::TokenType::SCOPE, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::COLON, tk, i);

        case ';' :
            return new Token(Token::TokenType::SEMI_COLON, tk, i);

        case '+' :
            second = getNextChar();
            if(second == '+') {
                tk[1] = second;
                return new Token(Token::TokenType::INCREMENT, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_SUM, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::ADD_OP, tk, i);

        case '-' :
            second = getNextChar();
            if(second == '>') {
                tk[1] = second;
                return new Token(Token::TokenType::ARROW, tk, i + 1);
            }
            if(second == '-') {
                tk[1] = second;
                return new Token(Token::TokenType::DECREMENT, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_DIF, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::SUB_OP, tk, i);

        case '!' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::NOT_EQUAL, tk, i + 1);

            }
            ungetChar(second);
            return new Token(Token::TokenType::LOG_NOT, tk, i);

        case '~' :
            return new Token(Token::TokenType::BIT_NOT, tk, i);

        case '*' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_PROD, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::MULT_OP, tk, i);

        case '/' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_QUOT, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::DIV_OP, tk, i);

        case '%' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_MOD, tk, i + 1);
            }
            if(second == '>') {
                tk[1] = second;
                return new Token(Token::TokenType::CLOSE_BRACE, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::MOD_OP, tk, i);

        case '<' :
            second = getNextChar();
            third = getNextChar();
            if(second == ':') {
                tk[1] = second;
                ungetChar(third);
                return new Token(Token::TokenType::OPEN_BRACKET, tk, i + 1);
            }
            if(second == '%') {
                tk[1] = second;
                ungetChar(third);
                return new Token(Token::TokenType::OPEN_BRACE, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                ungetChar(third);
                return new Token(Token::TokenType::LESS_THAN_EQUAL, tk, i + 1);
            }
            if(second == '<') {
                if(third == '=') {
                    tk[1] = second;
                    tk[2] = third;
                    return new Token(Token::TokenType::ASSIGN_LEFT, tk, i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return new Token(Token::TokenType::BIT_LEFT, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            return new Token(Token::TokenType::LESS_THAN, tk, i);

        case '>' :
            second = getNextChar();
            third = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::GREATER_THAN_EQUAL, tk, i + 1);
            }
            if(second == '>') {
                if(third == '=') {
                    tk[1] = second;
                    tk[2] = third;
                    return new Token(Token::TokenType::ASSIGN_RIGHT, tk, i + 2);
                }
                ungetChar(third);
                tk[1] = second;
                return new Token(Token::TokenType::BIT_RIGHT, tk, i + 1);
            }
            ungetChar(third);
            ungetChar(second);
            return new Token(Token::TokenType::GREATER_THAN, tk, i);

        case '&' :
            second = getNextChar();
            if(second == '&') {
                tk[1] = second;
                return new Token(Token::TokenType::LOG_AND, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_AND, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::BIT_AND, tk, i);

        case '^' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_XOR, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::BIT_XOR, tk, i);

        case '|' :
            second = getNextChar();
            if(second == '|') {
                tk[1] = second;
                return new Token(Token::TokenType::LOG_OR, tk, i + 1);
            }
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::ASSIGN_OR, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::BIT_OR, tk, i);

        case '?' :
            second = getNextChar();
            if(second == ':') {
                tk[1] = second;
                return new Token(Token::TokenType::COND_OP, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::TERNARY, tk, i);

        case '=' :
            second = getNextChar();
            if(second == '=') {
                tk[1] = second;
                return new Token(Token::TokenType::EQUAL, tk, i + 1);
            }
            ungetChar(second);
            return new Token(Token::TokenType::ASSIGN, tk, i);

        default:
            break;

    }
    return new Token(Token::TokenType::BAD_TOKEN, tk, i);

}
