#include <iostream> 
#include "lexer/lexer.h"
#include "token/token.h"
#include "utils/extract-content.h"


std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case OPEN_CURLY_BRACES: return "OPEN_CURLY_BRACES";
        case CLOSE_CURLY_BRACES: return "CLOSE_CURLY_BRACES";
        case KEY: return "KEY";
        case VALUE: return "VALUE";
        case COLON: return "COLON";
        case OPEN_SQUARE_BRACKET: return "OPEN_SQUARE_BRACKET";
        case CLOSE_SQUARE_BRACKET: return "CLOSE_SQUARE_BRACKET";
        case NOT_TOKEN_TYPE: return "NOT_TOKEN_TYPE";
        default: return "UNKNOWN";
    }
}

void printTokens(std::vector<Token> tokens) {
    uint i = 0;
    std::cout << tokens.size() << std::endl;
    while (i < tokens.size()) {
        std::cout << "(" << tokens[i].val << ": " << tokenTypeToString(tokens[i].token_type)
                  << " [Lin " << tokens[i].line << ", Col " << tokens[i].column << "])" << std::endl;
        i++;
    }
}
int main() {
    std::string content = extract_file_content("test.json");
    std::vector<Token> tokens = lexer(content);
    printTokens(tokens); 
    return 0;
}
