#include "lexer/token.h"

Token create_token(
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

