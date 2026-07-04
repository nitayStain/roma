#ifndef KEYWORD_H
#define KEYWORD_H

#include "lexer/token.h"
#include <stddef.h>

// returns the matching keyword TokenType for [start, start+length), or
// TOK_IDENTIFIER if it isn't a keyword
TokenType lookup_keyword(const char* start, size_t length);

// reverse of lookup_keyword - the source spelling for a keyword token type
const char* keyword_name(TokenType type);

#endif // KEYWORD_H
