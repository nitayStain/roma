#include "lexer/token.h"

Token token_init(
    TokenType type, 
    const char* start, 
    int length, 
    int line, 
    int column) {
  Token token = {};
  token.type = type;
  token.value = (char*)start;
  token.length = length;
  token.line = line;
  token.column = column;
  
  return token;
}

