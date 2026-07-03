#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType {
  TOK_IDENTIFIER = 0,
  TOK_EOF
} TokenType;

typedef struct Token {
  TokenType type;
  char* value;
  int length;

  int line, column;
} Token;

Token token_init(
    TokenType type, 
    const char* start, 
    int length, 
    int line, 
    int column);

#endif // TOKEN_H
