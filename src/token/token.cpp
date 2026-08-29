#include <unordered_map>
#include "token.h"
std::unordered_map<char, TokenType> symbols {
    {'{', OPEN_CURLY_BRACES},
    {'}', CLOSE_CURLY_BRACES},
    {':', COLON},
    {'[', OPEN_SQUARE_BRACKET},
    {']', CLOSE_SQUARE_BRACKET},
};

TokenType searchSymbolType(char symbol) {
    auto it = symbols.find(symbol);
    if (it == symbols.end()) {
        return NOT_TOKEN_TYPE;
    }
    return it->second;
}