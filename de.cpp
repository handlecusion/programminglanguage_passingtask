#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>

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
                } else if (isdigit(currentChar)) {
                    stringstream constant;
                    while (isdigit(input_[currentPos_])) {
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

    int factor() {
        Token token = getNextToken();
        switch (token.type) {
            case TokenType::IDENTIFIER:
                return symbolTable[token.lexeme];
            case TokenType::CONSTANT:
                return stoi(token.lexeme);
            case TokenType::LEFT_PAREN: {
                int result = expression();
                match(TokenType::RIGHT_PAREN);
                return result;
            }
            default:
                error("Unexpected token in factor: " + token.lexeme);
                return 0;
        }
    }

    int term() {
        int result = factor();
        while (true) {
            Token token = getNextToken();
            switch (token.type) {
                case TokenType::MULTIPLY:
                    result *= factor();
                    break;
                case TokenType::DIVIDE: {
                    int denominator = factor();
                    if (denominator == 0) {
                        error("Division by zero");
                    }
                    result /= denominator;
                    break;
                }
                default:
                    currentPos_--;
                    return result;
            }
        }
    }

    int expression() {
        int result = term();
        while (true) {
            Token token = getNextToken();
            switch (token.type) {
                case TokenType::ADD:
                    result += term();
                    break;
                case TokenType::SUBTRACT:
                    result -= term();
                    break;
                default:
                    currentPos_--;
                    return result;
            }
        }
    }

    void assignment() {
        Token identifier = getNextToken();
        match(TokenType::ASSIGN);
        int value = expression();
        symbolTable[identifier.lexeme] = value;
    }

    void statement() {
        Token token = getNextToken();
        switch (token.type) {
            case TokenType::IDENTIFIER:
                currentPos_--;
                assignment();
                break;
            default:
                currentPos_--;
                cout << "Result: " << expression() << endl;
                break;
        }
        // 각 문장이 세미콜론으로 끝날 것으로 예상하고 추가
        match(TokenType::SEMICOLON);
    }

    void parseStatements() {
        while (true) {
            statement();
            // 입력 끝에 도달하면 종료
            if (getNextToken().type == TokenType::END_OF_INPUT) {
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

