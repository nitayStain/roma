#ifndef LEXER_H
#define LEXER_H

#include "lexer/token.h"

typedef struct Lexer {
  char* code;
  int code_length;
  int current;
} Lexer;

Lexer* lexer_from_file(const char* filename);
Token* lexer_lex(Lexer* lexer);

#endif // LEXER_H
