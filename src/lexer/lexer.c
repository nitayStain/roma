#include "lexer/lexer.h"
#include "lexer/token.h"

#include <stdlib.h>

Token lexer_next(Lexer* lexer) {
  if(lexer->code_length == lexer->current) return token_init(TOK_EOF, NULL, 0, lexer->pos.line, lexer->pos.column);
}

void lexer_free(Lexer* lexer) {
  if(!lexer) return;

  if(lexer->code) free(lexer->code);
  lexer->code = NULL;
  
  free(lexer);
}
