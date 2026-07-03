#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType {
  IDENTIFIER = 0,
} TokenType;

typedef struct Token {
  TokenType type;
  char* value;
  int length;

  int line, column;
} Token;

Token create_token(
    TokenType type, 
    const char* start, 
    int length, 
    int line, 
    int column);

#endif // TOKEN_H
