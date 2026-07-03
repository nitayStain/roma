#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>
#include "logger.h"
#include "lexer/keyword_list.h"

typedef enum TokenType {
  TOK_IDENTIFIER = 0,

  /* keywords generated from keyword_list.h */
#define X(token, str) token,
  KEYWORD_LIST
#undef X

  /* operators */
  TOK_PLUS, // '+'
  TOK_MINUS, // '-'

  TOK_EOF
} TokenType;

typedef struct Token {
  TokenType type;
  char* start;
  size_t length;
  
  struct {
    size_t line;
    size_t column;
  } pos;
} Token;

Token make_token(
  char* start,
  size_t length,
  size_t column,
  size_t line,
  TokenType type
    );

// debug-only: dumps token data
#define LOG_TOKEN(token) \
  LOG("token type=%d value='%.*s' line=%zu col=%zu\n", \
      (token).type, (int)(token).length, (token).start, \
      (token).pos.line, (token).pos.column)

#endif // TOKEN_H
