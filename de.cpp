#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

enum class TokenType {
    IDENTIFIER,
    CONSTANT,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    LEFT_PAREN,
    RIGHT_PAREN,
    SEMICOLON,
    END_OF_INPUT,
    INVALID,
};

struct Token {
    TokenType type;
    string lexeme;
};

class Parser {
public:
    Parser(string input) : input_(move(input)), currentPos_(0) {}

    void parse() {
        parseDeclarations();
        parseStatements();
        printSymbolTable();
    }

private:
    string input_;
    size_t currentPos_;
    unordered_map<string, int> symbolTable;

    Token getNextToken() {
        while (currentPos_ < input_.size() && isspace(input_[currentPos_])) {
            currentPos_++;
        }

        if (currentPos_ >= input_.size()) {
            return {TokenType::END_OF_INPUT, ""};
        }

        char currentChar = input_[currentPos_];
        switch (currentChar) {
            case '+':
                currentPos_++;
                return {TokenType::ADD, "+"};
            case '-':
                currentPos_++;
                return {TokenType::SUBTRACT, "-"};
            case '*':
                currentPos_++;
                return {TokenType::MULTIPLY, "*"};
            case '/':
                currentPos_++;
                return {TokenType::DIVIDE, "/"};
            case '=':
                currentPos_++;
                return {TokenType::ASSIGN, "="};
            case '(':
                currentPos_++;
                return {TokenType::LEFT_PAREN, "("};
            case ')':
                currentPos_++;
                return {TokenType::RIGHT_PAREN, ")"};
            case ';':
                currentPos_++;
                return {TokenType::SEMICOLON, ";"};
            default:
                if (isalpha(currentChar)) {
                    stringstream identifier;
                    while (isalnum(input_[currentPos_])) {
                        identifier << input_[currentPos_++];
                    }
                    return {TokenType::IDENTIFIER, identifier.str()};
                } else if (isdigit(currentChar) || (currentChar == '-' && isdigit(input_[currentPos_ + 1]))) {
                    stringstream constant;
                    constant << input_[currentPos_++];
                    while (isdigit(input_[currentPos_]) || input_[currentPos_] == '.') {
                        constant << input_[currentPos_++];
                    }
                    return {TokenType::CONSTANT, constant.str()};
                } else {
                    currentPos_++;
                    return {TokenType::INVALID, ""};
                }
        }
    }

    void error(const string& message) {
        cerr << "Error: " << message << endl;
        exit(EXIT_FAILURE);
    }

    void match(TokenType expected) {
        Token token = getNextToken();
        if (token.type != expected) {
            error("Unexpected token: " + token.lexeme);
        }
    }

    void factor() {
        Token token = getNextToken();
        switch (token.type) {
            case TokenType::IDENTIFIER:
                return;
            case TokenType::CONSTANT:
                return;
            case TokenType::LEFT_PAREN:
                expression();
                match(TokenType::RIGHT_PAREN);
                return;
            default:
                error("Unexpected token in factor: " + token.lexeme);
        }
    }

    void term() {
        factor();
        while (true) {
            Token token = getNextToken();
            switch (token.type) {
                case TokenType::MULTIPLY:
                    factor();
                    break;
                case TokenType::DIVIDE:
                    factor();
                    break;
                default:
                    currentPos_--;
                    return;
            }
        }
    }

    void expression() {
        term();
        while (true) {
            Token token = getNextToken();
            switch (token.type) {
                case TokenType::ADD:
                    term();
                    break;
                case TokenType::SUBTRACT:
                    term();
                    break;
                default:
                    currentPos_--;
                    return;
            }
        }
    }

    void assignment() {
        Token identifier = getNextToken();
        match(TokenType::ASSIGN);
        expression();
        symbolTable[identifier.lexeme] = 0; // placeholder value
    }

    void declaration() {
        Token identifier = getNextToken();
        if (getNextToken().type == TokenType::ASSIGN) {
            match(TokenType::ASSIGN);
            expression();
        }
        match(TokenType::SEMICOLON);
        symbolTable[identifier.lexeme] = 0; // placeholder value
    }

    void statement() {
        Token token = getNextToken();
        switch (token.type) {
            case TokenType::IDENTIFIER:
                currentPos_--;
                assignment();
                break;
            case TokenType::CONSTANT:
                currentPos_--;
                expression();
                match(TokenType::SEMICOLON);
                break;
            default:
                error("Unexpected token in statement: " + token.lexeme);
        }
    }

    void parseDeclarations() {
        while (true) {
            declaration();
            Token token = getNextToken();
            if (token.type != TokenType::IDENTIFIER && token.type != TokenType::END_OF_INPUT) {
                break;
            }
        }
    }

    void parseStatements() {
        while (true) {
            statement();
            Token token = getNextToken();
            if (token.type == TokenType::END_OF_INPUT) {
                break;
            }
        }
    }

    void printSymbolTable() const {
        cout << "Variable Values:" << endl;
        for (const auto& entry : symbolTable) {
            cout << entry.first << " = " << entry.second << endl;
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return EXIT_FAILURE;
    }

    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return EXIT_FAILURE;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    string input = buffer.str();
    Parser parser(input);
    parser.parse();

    return 0;
}
