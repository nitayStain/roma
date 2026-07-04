#ifndef LEXER_ERROR_LIST_H
#define LEXER_ERROR_LIST_H

#define LEXER_ERROR_LIST \
  X(UnexpectedCharacterError, "unexpected character '%c' at line %zu, column %zu") \
  X(UnterminatedStringError,  "unterminated string literal at line %zu, column %zu")

#endif // LEXER_ERROR_LIST_H
