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

  /* arithematic operators */
  TOK_PLUS, // '+'
  TOK_MINUS, // '-'
  TOK_MULTIPLY, // '*'
  TOK_DIVIDE, // '/'
  TOK_PERCENT, // '%'
  TOK_INCREMENT, // '++'
  TOK_DECREMENT, // '--'

  /* assignment operator */
  TOK_ASSIGN, // '='

  /* <OP>EQUAL operators */
  TOK_PLUS_EQUAL,   // '+='
  TOK_MINUS_EQUAL,  // '-='
  TOK_STAR_EQUAL,   // '*='
  TOK_SLASH_EQUAL,  // '/='
  TOK_PERCENT_EQUAL, // '%='
  TOK_CARET_EQUAL,  // '^='
  TOK_AMP_EQUAL,    // '&='
  TOK_PIPE_EQUAL,   // '|='

  /* logical operators */
  TOK_GREATER, // '>'
  TOK_LOWER, // '<'
  TOK_NOT, // '!'
  TOK_GREATER_OR_EQUALS, // '>='
  TOK_LOWER_OR_EQUALS, // '<='
  TOK_AND, // '&&'
  TOK_OR, // '||'
  TOK_EQUALS, // '=='
  TOK_NOT_EQUALS, // '!='
  
  /* Bitwise operators */
  TOK_BIT_AND, // '&'
  TOK_BIT_NOT, // '~'
  TOK_BIT_OR, // '|'
  TOK_LEFT_SHIFT, // '<<'
  TOK_RIGHT_SHIFT, // '>>'
  TOK_XOR, // '^'

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
