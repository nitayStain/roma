#ifndef PARSER_ERROR_H
#define PARSER_ERROR_H

#include "error.h"
#include "parser/parser_error_list.h"

typedef enum ParserErrorType {
#define X(name, tmpl) name,
  PARSER_ERROR_LIST
#undef X
} ParserErrorType;

extern const char* const parser_error_templates[];

// err_type must be a bare ParserErrorType enum constant (e.g. UnexpectedTokenError)
// so #err_type stringifies to its name for display; ... fills that type's template
#define PARSER_ERROR(parser, err_type, ...) \
  error_stack_push(&(parser)->errors, err_type, #err_type, parser_error_templates[err_type], ##__VA_ARGS__)

#endif // PARSER_ERROR_H
