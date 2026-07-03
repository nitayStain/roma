#include "parser/ast.h"
#include "logger.h"

#include <stdlib.h>

#define NODELIST_INITIAL_CAPACITY 8

Node* ast_node_alloc(NodeType type, Token pos) {
  Node* node = (Node*) calloc(1, sizeof(Node));
  node->type = type;
  node->pos = pos;

  return node;
}

void nodelist_init(NodeList* list) {
  list->items = NULL;
  list->count = 0;
  list->capacity = 0;
}

void nodelist_push(NodeList* list, Node* node) {
  if(list->count == list->capacity) {
    list->capacity = list->capacity ? list->capacity * 2 : NODELIST_INITIAL_CAPACITY;
    list->items = (Node**) realloc(list->items, list->capacity * sizeof(Node*));
  }

  list->items[list->count++] = node;
}

void nodelist_free(NodeList* list) {
  for(size_t i = 0; i < list->count; i++)
    ast_free(list->items[i]);

  free(list->items);
  list->items = NULL;
  list->count = 0;
  list->capacity = 0;
}

void ast_free(Node* node) {
  if(!node) return;

  switch(node->type) {
    case NODE_LITERAL:
    case NODE_IDENTIFIER:
    case NODE_BREAK:
      break;

    case NODE_UNARY:
      ast_free(node->as.unary.operand);
      break;
    case NODE_BINARY:
      ast_free(node->as.binary.left);
      ast_free(node->as.binary.right);
      break;
    case NODE_ASSIGN:
      ast_free(node->as.assign.target);
      ast_free(node->as.assign.value);
      break;

    case NODE_EXPR_STMT:
      ast_free(node->as.expr_stmt.expr);
      break;
    case NODE_VAR_DECL:
      ast_free(node->as.var_decl.initializer);
      break;
    case NODE_BLOCK:
      nodelist_free(&node->as.block.statements);
      break;
    case NODE_IF:
      ast_free(node->as.if_stmt.condition);
      ast_free(node->as.if_stmt.then_branch);
      ast_free(node->as.if_stmt.else_branch);
      break;
    case NODE_WHILE:
      ast_free(node->as.while_stmt.condition);
      ast_free(node->as.while_stmt.body);
      break;
    case NODE_RETURN:
      ast_free(node->as.ret.value);
      break;
  }

  free(node);
}

static void print_indent(int indent) {
  for(int i = 0; i < indent; i++) LOG("  ");
}

void ast_print(Node* node, int indent) {
  if(!node) return;

  print_indent(indent);

  switch(node->type) {
    case NODE_LITERAL:
      switch(node->as.literal.value_type) {
        case VAL_INT:   LOG("Literal<int> %ld\n", node->as.literal.as_int);   break;
        case VAL_FLOAT: LOG("Literal<float> %f\n", node->as.literal.as_float); break;
        case VAL_BOOL:  LOG("Literal<bool> %s\n", node->as.literal.as_bool ? "true" : "false"); break;
        case VAL_STRING: LOG("Literal<string> %.*s\n", node->as.literal.as_str.length, node->as.literal.as_str.chars); break;
        case VAL_UNDEFINED: LOG("Literal<undefined>\n"); break;
      }
      break;
    case NODE_IDENTIFIER:
      LOG("Identifier '%.*s'\n", (int)node->as.identifier.name.length, node->as.identifier.name.start);
      break;
    case NODE_UNARY:
      LOG("Unary op=%d\n", node->as.unary.op);
      ast_print(node->as.unary.operand, indent + 1);
      break;
    case NODE_BINARY:
      LOG("Binary op=%d\n", node->as.binary.op);
      ast_print(node->as.binary.left, indent + 1);
      ast_print(node->as.binary.right, indent + 1);
      break;
    case NODE_ASSIGN:
      LOG("Assign op=%d\n", node->as.assign.op);
      ast_print(node->as.assign.target, indent + 1);
      ast_print(node->as.assign.value, indent + 1);
      break;

    case NODE_EXPR_STMT:
      LOG("ExprStmt\n");
      ast_print(node->as.expr_stmt.expr, indent + 1);
      break;
    case NODE_VAR_DECL:
      LOG("VarDecl type=%d name='%.*s'\n", node->as.var_decl.type_tok,
          (int)node->as.var_decl.name.length, node->as.var_decl.name.start);
      if(node->as.var_decl.initializer) ast_print(node->as.var_decl.initializer, indent + 1);
      break;
    case NODE_BLOCK:
      LOG("Block\n");
      for(size_t i = 0; i < node->as.block.statements.count; i++)
        ast_print(node->as.block.statements.items[i], indent + 1);
      break;
    case NODE_IF:
      LOG("If\n");
      ast_print(node->as.if_stmt.condition, indent + 1);
      ast_print(node->as.if_stmt.then_branch, indent + 1);
      if(node->as.if_stmt.else_branch) ast_print(node->as.if_stmt.else_branch, indent + 1);
      break;
    case NODE_WHILE:
      LOG("While\n");
      ast_print(node->as.while_stmt.condition, indent + 1);
      ast_print(node->as.while_stmt.body, indent + 1);
      break;
    case NODE_BREAK:
      LOG("Break\n");
      break;
    case NODE_RETURN:
      LOG("Return\n");
      if(node->as.ret.value) ast_print(node->as.ret.value, indent + 1);
      break;
  }
}
