#include <string>
#include "Tokenizer.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    string path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = "../Tokenizer.cpp";
    }

    Tokenizer* tokenizer = new Tokenizer(path);

    tokenizer->start();

    Token tk = *tokenizer->nextToken();
    while (tk.getToken() != Token::TokenType::END) {
        cout << tk.toString() << endl;
        tk = *tokenizer->nextToken();
    }
    cout << tk.toString() << endl;

    return 0;

}