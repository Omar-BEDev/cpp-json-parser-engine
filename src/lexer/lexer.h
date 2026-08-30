#ifndef LEXER 
#define LEXER
#include "../token/token.h"
#include <iostream>
#include <string_view>
#include <vector>
#define BACKSLACH 92
#define SLACH 47
#define JSON_HEX_VALUE_LENGTH 4
#define SPACE_ASSCIV_ALUE 32

typedef struct KeyState {
    std::vector<Token> *tokens ;
    std::string_view key;
    std::string_view value;
    std::string_view content;
    uint i = 0;
    uint size;
    TokenType state = KEY;
    int line = 0;
    int column = 0;
    uint tokenIndex = 0;
    std::vector<char> textValue;
} KeyState ;
std::vector<Token> lexer(std::string_view content);
#endif