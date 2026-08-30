#ifndef TOKEN
#define TOKEN 
#include <string>

typedef enum {
    OPEN_CURLY_BRACES,
    CLOSE_CURLY_BRACES,
    KEY,
    VALUE,
    COLON,
    OPEN_SQUARE_BRACKET,
    CLOSE_SQUARE_BRACKET,
    NOT_TOKEN_TYPE
} TokenType ;


typedef struct Token {
    std::string val;
    TokenType token_type;
    int line;
    int column;
}Token;

TokenType searchSymbolType(char symbol);
#endif