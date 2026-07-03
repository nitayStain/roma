#include "parser/parser_error.h"

const char* const parser_error_templates[] = {
#define X(name, tmpl) tmpl,
  PARSER_ERROR_LIST
#undef X
};
