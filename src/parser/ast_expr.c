#include "parser/ast.h"

Node* ast_new_literal_int(long value, Token pos) {
  Node* node = ast_node_alloc(NODE_LITERAL, pos);
  node->as.literal.value_type = VAL_INT;
  node->as.literal.as_int = value;

  return node;
}

Node* ast_new_literal_float(double value, Token pos) {
  Node* node = ast_node_alloc(NODE_LITERAL, pos);
  node->as.literal.value_type = VAL_FLOAT;
  node->as.literal.as_float = value;

  return node;
}

Node* ast_new_literal_bool(bool value, Token pos) {
  Node* node = ast_node_alloc(NODE_LITERAL, pos);
  node->as.literal.value_type = VAL_BOOL;
  node->as.literal.as_bool = value;

  return node;
}

Node* ast_new_identifier(Token name) {
  Node* node = ast_node_alloc(NODE_IDENTIFIER, name);
  node->as.identifier.name = name;

  return node;
}

Node* ast_new_unary(TokenType op, Node* operand, Token pos) {
  Node* node = ast_node_alloc(NODE_UNARY, pos);
  node->as.unary.op = op;
  node->as.unary.operand = operand;

  return node;
}

Node* ast_new_binary(TokenType op, Node* left, Node* right, Token pos) {
  Node* node = ast_node_alloc(NODE_BINARY, pos);
  node->as.binary.op = op;
  node->as.binary.left = left;
  node->as.binary.right = right;

  return node;
}

Node* ast_new_assign(TokenType op, Node* target, Node* value, Token pos) {
  Node* node = ast_node_alloc(NODE_ASSIGN, pos);
  node->as.assign.op = op;
  node->as.assign.target = target;
  node->as.assign.value = value;

  return node;
}
