#include "program.h"

#include <stdlib.h>

Program* program_from_file(const char* filename) {
  Lexer* lexer = lexer_from_file(filename);
  if(!lexer) return NULL;

  Program* program = malloc(sizeof(Program));
  program->lexer = lexer;
  program->parser = parser_init(lexer);
  program->ast = parse_program(&program->parser);

  error_stack_init(&program->errors);
  error_stack_extend(&program->errors, &lexer->errors);
  error_stack_extend(&program->errors, &program->parser.errors);

  return program;
}

bool program_has_errors(Program* program) {
  return error_stack_has_errors(&program->errors);
}

void program_print_errors(Program* program, FILE* out) {
  error_stack_print(&program->errors, out);
}

void program_free(Program* program) {
  error_stack_free(&program->errors);
  ast_free(program->ast);
  parser_free(&program->parser);
  lexer_free(program->lexer);
  free(program);
}
