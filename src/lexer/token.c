#include "lexer/token.h"

Token make_token(
  char* start,
  size_t length,
  size_t column,
  size_t line,
  TokenType type
    ) {
  Token token = {};
  token.start = start;
  token.length = length;
  token.pos.column = column;
  token.pos.line = line;
  token.type = type;

  return token;
}
