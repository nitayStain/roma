#include "lexer/lexer.h"
#include "lexer/token.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

Token make_keyword_or_identifier(Lexer* lexer, char* start, size_t column, size_t line);

char advance(Lexer* lexer);
char curr(Lexer* lexer);
char peek(Lexer* lexer);

bool is_at_end(Lexer* lexer);

void skip_whitespace(Lexer* lexer);
void skip_comments(Lexer* lexer);

Token lexer_next(Lexer* lexer) {
  TokenType type;
  int column = lexer->pos.column;
  int line = lexer->pos.line;
  
  skip_comments(lexer);
  skip_whitespace(lexer);
  
  char* start = lexer->current;

  char c = advance(lexer);
  
  if(isalpha(c) || c == '_') return make_keyword_or_identifier(lexer, start, column, line);
  switch(c) {
    case '\0':  type = TOK_EOF;     break;
    case '+':   type = TOK_PLUS;    break;
    case '-':   type = TOK_MINUS;   break;
    default:
      type = TOK_IDENTIFIER;
      break;
  }

  return make_token(start, 1, column, line, type);
}

void lexer_free(Lexer* lexer) {
  if(!lexer) return;

  if(lexer->code) free((void*) lexer->code);
  lexer->code = NULL;
  
  free(lexer);
}

Token make_keyword_or_identifier(Lexer* lexer, char* start, size_t column, size_t line) {
  TokenType type = TOK_IDENTIFIER;
  while(isalnum(curr(lexer)) || curr(lexer) == '_') advance(lexer);
  size_t length = (size_t)(lexer->current - start);
  return make_token(start, length, column, line, type);
}

char advance(Lexer* lexer) {
  if(is_at_end(lexer)) return '\0';
  char c = *lexer->current++;

  if(c == '\n') {
    lexer->pos.line++;
    lexer->pos.column = 0;
  } else {
    lexer->pos.column++;
  }

  return c;
}

char curr(Lexer* lexer) {
  if(is_at_end(lexer)) return '\0';
  return *lexer->current;
}

char peek(Lexer* lexer) {
  if(lexer->current + 1 >= lexer->code + lexer->code_length) return '\0'; // nullbyte will be our EOF
  return lexer->current[1];
}

bool is_at_end(Lexer* lexer) {
  return lexer->current >= lexer->code + lexer->code_length;
}

void skip_whitespace(Lexer* lexer) {
  while(!is_at_end(lexer) && isspace(curr(lexer))) 
    advance(lexer);
}

void skip_comments(Lexer* lexer) {
  if(curr(lexer) == '/' && peek(lexer) == '/') {
    do { advance(lexer); } while(curr(lexer) != '\n' && !is_at_end(lexer));
  }
}
