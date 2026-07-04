#ifndef LEXER_ERROR_H
#define LEXER_ERROR_H

#include "error.h"
#include "lexer/lexer_error_list.h"

typedef enum LexerErrorType {
#define X(name, tmpl) name,
  LEXER_ERROR_LIST
#undef X
} LexerErrorType;

extern const char* const lexer_error_templates[];

// err_type must be a bare LexerErrorType enum constant (e.g. UnexpectedCharacterError)
// so #err_type stringifies to its name for display; ... fills that type's template
#define LEXER_ERROR(lexer, err_type, ...) \
  error_stack_push(&(lexer)->errors, err_type, #err_type, lexer_error_templates[err_type], ##__VA_ARGS__)

#endif // LEXER_ERROR_H
