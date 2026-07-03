#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"
#include "parser/ast.h"
#include "error.h"

#include <stdbool.h>

typedef struct Lexer Lexer;

typedef struct Parser {
  Lexer* lexer;
  Token current;
  Token previous;

  ErrorStack errors;
  bool panic_mode; // suppresses cascading errors until the next synchronize()
} Parser;

Parser parser_init(Lexer* lexer);
void parser_free(Parser* parser);

// parses a whole program, will return a whole block (see parser/ast_stmt.h->BlockNode)
Node* parse_program(Parser* parser);

#endif // PARSER_H
