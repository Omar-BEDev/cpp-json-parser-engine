#include "lexer.h"
#include "../token/token.h"
#include <cstdlib>
#include <string_view>
#include <vector>
#include "../error-handling/error-handling.h"

bool checkJsonHexValue(KeyState *ks, std::vector<char> *valueArr) {

    int hexLen = 0;

    while(++ks->i < ks->size 
        && ((ks->content[ks->i] >= '0' && ks->content[ks->i] <= '9')
        || (ks->content[ks->i] >= 'A' && ks->content[ks->i] <= 'F')
        || (ks->content[ks->i] >= 'a' && ks->content[ks->i] <= 'f'))
        ) {
            hexLen++;
            (ks->column)++;
            (*valueArr).push_back(ks->content[(ks->i)]);       
        }
        if (hexLen < JSON_HEX_VALUE_LENGTH ) {
             print_error_message(INVALID_HEX_VALUE, ks->line, ks->column);
             return false;
        }
        return true;
}

void collect_key(
    KeyState* ks
) {
    int columns_additions = 0;
    
    if (ks->content[(ks->i)++] == '"') {
        while (ks->i < ks->size && (ks->state == KEY && ks->content[(ks->i)++] != '"') ) {
            ks->textValue.push_back(ks->content[(ks->i)]);
            (ks->column)++;
            columns_additions++;
            if (columns_additions == 1) {
                (*ks->tokens)[ks->tokenIndex].column = ks->column;
            }
        }
    }
    if (columns_additions >= 1) {
    ks->key = ks->textValue.data();
    (*ks->tokens)[ks->tokenIndex].token_type = KEY;
    (*ks->tokens)[ks->tokenIndex].val = ks->key;
    (*ks->tokens)[ks->tokenIndex].line = ks->line;
    } 
                           
}

void collect_value(KeyState *ks) {
    int columns_additions = 0;
    
    ks->i++;
    while (ks->i < ks->size && (ks->state == KEY && ks->content[(ks->i)++] != '"') ) {
        ks->textValue.push_back(ks->content[(ks->i)]);
        (ks->column)++;
        columns_additions++;
        if (columns_additions == 1) {
            (*ks->tokens)[ks->tokenIndex].column = ks->column;
        }
        if (ks->i+1 < ks->size && ks->content[ks->i] == BACKSLACH) {
            if (ks->content[(ks->i) + 1] == 'u') {
                bool result = checkJsonHexValue(ks,&ks->textValue);
                if (!result) {
                    return;
                }
            }
            ErrorType errorType = INVALID_ESCAPE_SEQUENCE;
            switch (ks->content[ks->i] + 1) {
                case 'q': case 'f': case 'r': case 'n': case 't':
                ks->textValue.push_back(ks->content[(ks->i)]);
                (ks->column)++;
                continue;
                default :
                print_error_message(errorType, ks->line, ks->column);
                return; 
            }
        }
    }
    
    if (columns_additions >= 1) {
    ks->value = ks->textValue.data();
    (*ks->tokens)[ks->tokenIndex].token_type = VALUE;
    (*ks->tokens)[ks->tokenIndex].val = ks->value;
    (*ks->tokens)[ks->tokenIndex].line = ks->line;
    }    
}
void makeSymbolToken(std::vector<Token> *tokens, char symbol, KeyState* ks) {
    TokenType token_type = searchSymbolType(symbol);
    ks->state = VALUE;
    ks->column++;
    (*tokens)[ks->tokenIndex].token_type = token_type;
    (*tokens)[ks->tokenIndex].val = symbol;
    (*tokens)[ks->tokenIndex].column = ks->column;
    (ks->i)++;
}

std::vector<Token> lexer(std::string_view content) {
    uint size = content.size();
    std::vector<Token> tokens;
    KeyState keystate = {
    .tokens = &tokens,
    .key = {},
    .value = {},
    .content = content,
    .size = size,
    .textValue = {}
    };
     
    while (keystate.i < keystate.size) {
        if (keystate.content[keystate.i] == '[') {
            makeSymbolToken(&tokens, '[', &keystate);
        }
        else if (keystate.content[keystate.i] == '{') {
            makeSymbolToken(&tokens, '{', &keystate);
        }
        else if (keystate.content[keystate.i] == '"' && keystate.state == KEY) {
            collect_key(&keystate);
        }
        if (keystate.content[keystate.i] == ':') {
            makeSymbolToken(&tokens, ':', &keystate);
        }

        if (keystate.state == VALUE && tokens[keystate.tokenIndex - 1].val == ":") {
            collect_value(&keystate);
        }
        if (keystate.content[keystate.i] == '}') {
            makeSymbolToken(&tokens, '}', &keystate);
        }
        if (keystate.content[keystate.i] == ']') {
            makeSymbolToken(&tokens, ']', &keystate);
        }
        else if (keystate.content[keystate.i] == '\n' || keystate.content[keystate.i] == SPACE_ASSCIV_ALUE) {
            if (keystate.content[keystate.i] == '\n') {
            keystate.line++;
            }
            keystate.i++;
        }
        else {
            keystate.i++;
        }
    }
    return tokens;
}