#ifndef PARSER_ERROR_LIST_H
#define PARSER_ERROR_LIST_H

#define PARSER_ERROR_LIST \
  X(UnexpectedTokenError, "%s, got '%.*s' at line %zu, column %zu") \
  X(LiteralOutOfRangeError, "value %lld is out of range for '%s' at line %zu, column %zu")

#endif // PARSER_ERROR_LIST_H
