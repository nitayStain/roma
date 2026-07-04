#ifndef PROGRAM_H
#define PROGRAM_H

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "error.h"

typedef struct Program {
  Lexer* lexer;
  Parser parser;
  Node* ast;
  ErrorStack errors; // combined lexer + parser errors, in that order
} Program;

Program* program_from_file(const char* filename);

bool program_has_errors(Program* program);
void program_print_errors(Program* program, FILE* out);

void program_free(Program* program);

#endif // PROGRAM_H
