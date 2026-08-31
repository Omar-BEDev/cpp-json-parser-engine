#include "lexer.h"
#include "../error-handling/error-handling.h"
#include "../token/token.h"
#include <cstdlib>
#include <string_view>
#include <vector>

bool checkJsonHexValue(KeyState *ks, std::vector<char> *valueArr) {

  int hexLen = 0;

  while (++ks->i < ks->size &&
         ((ks->content[ks->i] >= '0' && ks->content[ks->i] <= '9') ||
          (ks->content[ks->i] >= 'A' && ks->content[ks->i] <= 'F') ||
          (ks->content[ks->i] >= 'a' && ks->content[ks->i] <= 'f'))) {
    hexLen++;
    (ks->column)++;
    (*valueArr).push_back(ks->content[(ks->i)]);
  }
  if (hexLen < JSON_HEX_VALUE_LENGTH) {
    print_error_message(INVALID_HEX_VALUE, ks->line, ks->column);
    return false;
  }
  return true;
}

void collect_key(KeyState *ks) {
  int columns_additions = 0;

  while (ks->i < ks->size &&
         (ks->state == KEY && ks->content[++(ks->i)] != '"')) {
    ks->textValue.push_back(ks->content[(ks->i)]);
    (ks->column)++;
    columns_additions++;
    if (columns_additions == 1) {
      (*ks->tokens)[ks->tokenIndex].column = ks->column;
    }
  }

  if (columns_additions >= 1) {
    ks->key = ks->textValue.data();
    (*ks->tokens).push_back(Token{});
    (*ks->tokens)[ks->tokenIndex].token_type = KEY;
    (*ks->tokens)[ks->tokenIndex].val = ks->key;
    (*ks->tokens)[ks->tokenIndex++].line = ks->line;
  }
}

void collect_value(KeyState *ks) {
  int columns_additions = 0;

  while (ks->i < ks->size &&
         (ks->state == VALUE && ks->content[++(ks->i)] != '"')) {
    ks->textValue.push_back(ks->content[(ks->i)]);
    (ks->column)++;
    columns_additions++;
    if (columns_additions == 1) {
      (*ks->tokens)[ks->tokenIndex].column = ks->column;
    }
    if (ks->i + 1 < ks->size && ks->content[ks->i] == BACKSLACH) {
      if (ks->content[(ks->i) + 1] == 'u') {
        bool result = checkJsonHexValue(ks, &ks->textValue);
        if (!result) {
          return;
        }
      }
      ErrorType errorType = INVALID_ESCAPE_SEQUENCE;
      switch (ks->content[ks->i] + 1) {
      case 'q':
      case 'f':
      case 'r':
      case 'n':
      case 't':
        ks->textValue.push_back(ks->content[(ks->i)]);
        (ks->column)++;
        continue;
      default:
        print_error_message(errorType, ks->line, ks->column);
        return;
      }
    }
  }

  if (columns_additions >= 1) {
    ks->value = ks->textValue.data();
    (*ks->tokens).push_back(Token{});
    (*ks->tokens)[ks->tokenIndex].token_type = VALUE;
    (*ks->tokens)[ks->tokenIndex].val = ks->value;
    (*ks->tokens)[ks->tokenIndex++].line = ks->line;
    ks->state = KEY;
  }
}
void makeSymbolToken(char symbol, KeyState *ks) {
  if (symbol == ':') {
    ks->state = VALUE;
  }
  TokenType token_type = searchSymbolType(symbol);
  ks->column++;
  (*ks->tokens).push_back(Token{});
  (*ks->tokens)[ks->tokenIndex].token_type = token_type;
  (*ks->tokens)[ks->tokenIndex].val = symbol;
  (*ks->tokens)[ks->tokenIndex++].column = ks->column;
  (ks->i)++;
}

std::vector<Token> lexer(std::string_view content) {
  uint size = content.size();
  std::vector<Token> tokens;
  KeyState keystate = {.tokens = &tokens,
                       .key = {},
                       .value = {},
                       .content = content,
                       .size = size,
                       .textValue = {}};

  while (keystate.i < keystate.size) {
    if (keystate.content[keystate.i] == '[') {
      makeSymbolToken('[', &keystate);
    }
    if (keystate.content[keystate.i] == '{') {
      makeSymbolToken('{', &keystate);
    }
    if (keystate.content[keystate.i] == '"' && keystate.state == KEY) {
      collect_key(&keystate);
      keystate.textValue.clear();
    }
    if (keystate.content[keystate.i] == ':') {
      makeSymbolToken(':', &keystate);
    }

    if (keystate.content[keystate.i] == '}') {
      makeSymbolToken('}', &keystate);
    }
    if (keystate.content[keystate.i] == ']') {
      makeSymbolToken(']', &keystate);
    }

    if (keystate.tokenIndex > 0 &&
        (keystate.state == VALUE &&
         tokens[keystate.tokenIndex - 1].val == ":" &&
         keystate.content[keystate.i] == '"')) {
      collect_value(&keystate);
      keystate.textValue.clear();
    }

    if (keystate.content[keystate.i] == '\n' ||
        keystate.content[keystate.i] == SPACE_ASSCI_VALUE) {
      if (keystate.content[keystate.i] == '\n') {
        keystate.line++;
      }
      if (keystate.content[keystate.i + 1] == '{') {
        keystate.state = KEY;
      }
      keystate.i++;
    } else {
      keystate.i++;
    }
  }
  return tokens;
}
