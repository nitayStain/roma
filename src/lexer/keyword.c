#include "lexer/keyword.h"
#include "lexer/keyword_list.h"

#include <string.h>

typedef struct {
  const char* name;
  size_t length;
  TokenType type;
} Keyword;

static const Keyword keywords[] = {
#define X(token, str) { str, sizeof(str) - 1, token },
  KEYWORD_LIST
#undef X
};

static const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

TokenType lookup_keyword(const char* start, size_t length) {
  for(size_t i = 0; i < keyword_count; i++) {
    if(length == keywords[i].length && memcmp(start, keywords[i].name, length) == 0)
      return keywords[i].type;
  }

  return TOK_IDENTIFIER;
}
