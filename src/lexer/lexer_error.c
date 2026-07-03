#include "lexer/lexer_error.h"

const char* const lexer_error_templates[] = {
#define X(name, tmpl) tmpl,
  LEXER_ERROR_LIST
#undef X
};
