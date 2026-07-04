#ifndef LEXER_H
#define LEXER_H

#include "lexer/token.h"
#include "error.h"
#include <unistd.h>

typedef struct Lexer {
  const char* code;
  char* current;

  size_t code_length;

  struct {
    size_t line;
    size_t column;
  } pos;

  ErrorStack errors;
} Lexer;

Lexer* lexer_from_file(const char* filename);
Token lexer_next(Lexer* lexer);
void lexer_free(Lexer* lexer);

#endif // LEXER_H
