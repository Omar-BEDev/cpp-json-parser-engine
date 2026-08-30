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
    std::vector<char> keyArr;
    if (ks->content[(ks->i)++] == '"') {
        while (ks->i < ks->size && (ks->state == KEY && ks->content[(ks->i)++] != '"') ) {
            keyArr.push_back(ks->content[(ks->i)]);
            (ks->column)++;
            columns_additions++;
            if (columns_additions == 1) {
                (*ks->tokens)[ks->tokenIndex].column = ks->column;
            }
        }
    }
    if (columns_additions >= 1) {
    *ks->key = keyArr.data();
    (*ks->tokens)[ks->tokenIndex].token_type = KEY;
    (*ks->tokens)[ks->tokenIndex].val = *ks->key;
    (*ks->tokens)[ks->tokenIndex].line = ks->line;
    }                                   
}

void collect_value(KeyState *ks) {
    int columns_additions = 0;
    std::vector<char> valueArr;
    ks->i++;
    while (ks->i < ks->size && (ks->state == KEY && ks->content[(ks->i)++] != '"') ) {
        valueArr.push_back(ks->content[(ks->i)]);
        (ks->column)++;
        columns_additions++;
        if (columns_additions == 1) {
            (*ks->tokens)[ks->tokenIndex].column = ks->column;
        }
        if (ks->i+1 < ks->size && ks->content[ks->i] == BACKSLACH) {
            if (ks->content[(ks->i) + 1] == 'u') {
                bool result = checkJsonHexValue(ks,&valueArr);
                if (!result) {
                    return;
                }
            }
            ErrorType errorType = INVALID_ESCAPE_SEQUENCE;
            switch (ks->content[ks->i] + 1) {
                case 'q': case 'f': case 'r': case 'n': case 't':
                valueArr.push_back(ks->content[(ks->i)]);
                (ks->column)++;
                continue;
                default :
                print_error_message(errorType, ks->line, ks->column);
                return; 
            }
        }
    }
    
    if (columns_additions >= 1) {
    *ks->value = valueArr.data();
    (*ks->tokens)[ks->tokenIndex].token_type = VALUE;
    (*ks->tokens)[ks->tokenIndex].val = *ks->value;
    (*ks->tokens)[ks->tokenIndex].line = ks->line;
    }    
}

Token* lexer(std::string_view content) {
    int size = content.size();
    std::vector<Token> tokens;
    KeyState keystate = {
    .tokens = &tokens,
    .key = NULL,
    .value = NULL,
    .content = content,
    .size = size,
    };
    std::string_view *key;
    std::string_view *value; 
    while (keystate.i < keystate.size) {

    }
}