#include "parser/ast.h"

Node* ast_new_expr_stmt(Node* expr, Token pos) {
  Node* node = ast_node_alloc(NODE_EXPR_STMT, pos);
  node->as.expr_stmt.expr = expr;

  return node;
}

Node* ast_new_var_decl(TokenType type_tok, Token name, Node* initializer) {
  Node* node = ast_node_alloc(NODE_VAR_DECL, name);
  node->as.var_decl.type_tok = type_tok;
  node->as.var_decl.name = name;
  node->as.var_decl.initializer = initializer;

  return node;
}

Node* ast_new_block(void) {
  Token pos = {};
  Node* node = ast_node_alloc(NODE_BLOCK, pos);
  nodelist_init(&node->as.block.statements);

  return node;
}

void ast_block_append(Node* block, Node* stmt) {
  nodelist_push(&block->as.block.statements, stmt);
}

Node* ast_new_if(Node* condition, Node* then_branch, Node* else_branch, Token pos) {
  Node* node = ast_node_alloc(NODE_IF, pos);
  node->as.if_stmt.condition = condition;
  node->as.if_stmt.then_branch = then_branch;
  node->as.if_stmt.else_branch = else_branch;

  return node;
}

Node* ast_new_while(Node* condition, Node* body, Token pos) {
  Node* node = ast_node_alloc(NODE_WHILE, pos);
  node->as.while_stmt.condition = condition;
  node->as.while_stmt.body = body;

  return node;
}

Node* ast_new_break(Token pos) {
  return ast_node_alloc(NODE_BREAK, pos);
}

Node* ast_new_return(Node* value, Token pos) {
  Node* node = ast_node_alloc(NODE_RETURN, pos);
  node->as.ret.value = value;

  return node;
}
