#include "lexer/lexer.h"
#include "lexer/token.h"
#include "lexer/keyword.h"
#include "lexer/lexer_error.h"

#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

Token make_string(Lexer* lexer, char* start, size_t column, size_t line);
Token make_keyword_or_identifier(Lexer* lexer, char* start, size_t column, size_t line);
Token make_number(Lexer* lexer, char* start, size_t column, size_t line);

char advance(Lexer* lexer);
char curr(Lexer* lexer);
char peek(Lexer* lexer);
bool match(Lexer* lexer, char expected);

bool is_at_end(Lexer* lexer);

void skip_whitespace(Lexer* lexer);
void skip_comments(Lexer* lexer);

Token lexer_next(Lexer* lexer) {
  TokenType type;

  skip_comments(lexer);
  skip_whitespace(lexer);

  // position must be captured after skipping, otherwise it points at
  // wherever the previous token ended instead of where this one starts
  int column = lexer->pos.column;
  int line = lexer->pos.line;

  char* start = lexer->current;

  char c = advance(lexer);
  
  if(c == '\'' || c == '"') return make_string(lexer, start, column, line);
  if(isalpha(c) || c == '_') return make_keyword_or_identifier(lexer, start, column, line);
  if(isdigit(c)) return make_number(lexer, start, column, line);
  switch(c) {
    case '\0':  type = TOK_EOF;             break;
    case '(':   type = TOK_LPAREN;          break;
    case ')':   type = TOK_RPAREN;          break;
    case '{':   type = TOK_LBRACE;          break;
    case '}':   type = TOK_RBRACE;          break;
    case ';':   type = TOK_SEMICOLON;       break;
    case '+':   type = match(lexer, '+') ? TOK_INCREMENT
                      : match(lexer, '=') ? TOK_PLUS_EQUAL
                      : TOK_PLUS;            break;
    case '-':   type = match(lexer, '-') ? TOK_DECREMENT
                      : match(lexer, '=') ? TOK_MINUS_EQUAL
                      : TOK_MINUS;           break;
    case '*':   type = match(lexer, '=') ? TOK_STAR_EQUAL : TOK_MULTIPLY; break;
    case '/':   type = match(lexer, '=') ? TOK_SLASH_EQUAL : TOK_DIVIDE;  break;
    case '%':   type = match(lexer, '=') ? TOK_PERCENT_EQUAL : TOK_PERCENT; break;
    case '=':   type = match(lexer, '=') ? TOK_EQUALS : TOK_ASSIGN;       break;
    case '!':   type = match(lexer, '=') ? TOK_NOT_EQUALS : TOK_NOT;      break;
    case '^':   type = match(lexer, '=') ? TOK_CARET_EQUAL : TOK_XOR;     break;
    case '~':   type = TOK_BIT_NOT;         break;
    case '>':   type = match(lexer, '=') ? TOK_GREATER_OR_EQUALS
                      : match(lexer, '>') ? TOK_RIGHT_SHIFT
                      : TOK_GREATER;         break;
    case '<':   type = match(lexer, '=') ? TOK_LOWER_OR_EQUALS
                      : match(lexer, '<') ? TOK_LEFT_SHIFT
                      : TOK_LOWER;           break;
    case '&':   type = match(lexer, '&') ? TOK_AND
                      : match(lexer, '=') ? TOK_AMP_EQUAL
                      : TOK_BIT_AND;         break;
    case '|':   type = match(lexer, '|') ? TOK_OR
                      : match(lexer, '=') ? TOK_PIPE_EQUAL
                      : TOK_BIT_OR;          break;
    default:
      LEXER_ERROR(lexer, UnexpectedCharacterError, c, (size_t)line, (size_t)column);
      type = TOK_ERROR;
      break;
  }

  size_t length = (size_t)(lexer->current - start);
  return make_token(start, length, column, line, type);
}

void lexer_free(Lexer* lexer) {
  if(!lexer) return;

  if(lexer->code) free((void*) lexer->code);
  lexer->code = NULL;

  error_stack_free(&lexer->errors);
  free(lexer);
}

Token make_string(Lexer* lexer, char* start, size_t column, size_t line) {
  char quote = *start;

  while(curr(lexer) != quote && !is_at_end(lexer)) {
    if(curr(lexer) == '\\') advance(lexer); // skip the escaped char, whatever it is
    advance(lexer);
  }

  if(is_at_end(lexer)) {
    LEXER_ERROR(lexer, UnterminatedStringError, line, column);
    size_t length = (size_t)(lexer->current - start);
    return make_token(start, length, column, line, TOK_ERROR);
  }

  advance(lexer); // consume closing quote
  size_t length = (size_t)(lexer->current - start);
  return make_token(start, length, column, line, TOK_STRING_LITERAL);
}

Token make_keyword_or_identifier(Lexer* lexer, char* start, size_t column, size_t line) {
  while(isalnum(curr(lexer)) || curr(lexer) == '_') advance(lexer);
  size_t length = (size_t)(lexer->current - start);
  
  TokenType type = lookup_keyword(start, length);
  return make_token(start, length, column, line, type);
}

Token make_number(Lexer* lexer, char* start, size_t column, size_t line) {
  // start[0] is the '0' already consumed by lexer_next before calling here,
  // so curr(lexer) is the char right after it - check for an 'x'/'b' prefix
  if(start[0] == '0' && (curr(lexer) == 'x' || curr(lexer) == 'X')) {
    advance(lexer); // consume 'x'/'X'
    while(isxdigit(curr(lexer))) advance(lexer);

    size_t length = (size_t)(lexer->current - start);
    return make_token(start, length, column, line, TOK_INT_LITERAL);
  }

  if(start[0] == '0' && (curr(lexer) == 'b' || curr(lexer) == 'B')) {
    advance(lexer); // consume 'b'/'B'
    while(curr(lexer) == '0' || curr(lexer) == '1') advance(lexer);

    size_t length = (size_t)(lexer->current - start);
    return make_token(start, length, column, line, TOK_INT_LITERAL);
  }

  while(isdigit(curr(lexer))) advance(lexer);

  bool is_float = false;
  if(curr(lexer) == '.' && isdigit(peek(lexer))) {
    is_float = true;
    advance(lexer); // consume '.'
    while(isdigit(curr(lexer))) advance(lexer);
  }

  size_t length = (size_t)(lexer->current - start);
  return make_token(start, length, column, line, is_float ? TOK_FLOAT_LITERAL : TOK_INT_LITERAL);
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

bool match(Lexer* lexer, char expected) {
  if(curr(lexer) != expected) return false;
  advance(lexer);
  return true;
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
