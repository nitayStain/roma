#ifndef AST_EXPR_H
#define AST_EXPR_H

#include <stdbool.h>
#include "lexer/token.h"

typedef struct Node Node;

typedef enum ValueType {
  VAL_INT,
  VAL_FLOAT,
  VAL_BOOL,
  VAL_STRING,
  VAL_UNDEFINED
} ValueType;

typedef struct LiteralNode {
  ValueType value_type;
  union {
    long as_int;
    double as_float;
    bool as_bool;

    // TODO: figure out an abstractish way for heap objects
    struct {
      char* chars;
      int length;
    } as_str;
  };
} LiteralNode;

typedef struct IdentifierNode {
  Token name;
} IdentifierNode;

typedef struct UnaryNode {
  TokenType op;
  Node* operand;
} UnaryNode;

typedef struct BinaryNode {
  TokenType op;
  Node* left;
  Node* right;
} BinaryNode;

typedef struct AssignNode {
  TokenType op; // TOK_ASSIGN, TOK_PLUS_EQUAL, ...
  Node* target;
  Node* value;
} AssignNode;

Node* ast_new_literal_int(long value, Token pos);
Node* ast_new_literal_float(double value, Token pos);
Node* ast_new_literal_bool(bool value, Token pos);
Node* ast_new_identifier(Token name);
Node* ast_new_unary(TokenType op, Node* operand, Token pos);
Node* ast_new_binary(TokenType op, Node* left, Node* right, Token pos);
Node* ast_new_assign(TokenType op, Node* target, Node* value, Token pos);

#endif // AST_EXPR_H
