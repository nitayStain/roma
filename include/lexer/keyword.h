#ifndef KEYWORD_H
#define KEYWORD_H

#include "lexer/token.h"
#include <stddef.h>

// returns the matching keyword TokenType for [start, start+length), or
// TOK_IDENTIFIER if it isn't a keyword
TokenType lookup_keyword(const char* start, size_t length);

#endif // KEYWORD_H
