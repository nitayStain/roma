#ifndef AST_H
#define AST_H

#include "lexer/token.h"
#include "parser/ast_node_list.h"
#include "parser/ast_expr.h"
#include "parser/ast_stmt.h"

typedef enum NodeType {
#define X(node) node,
  AST_NODE_LIST
#undef X
} NodeType;

struct Node {
  NodeType type;
  Token pos; // where the node starts, for error reporting

  union {
    LiteralNode literal;
    IdentifierNode identifier;
    UnaryNode unary;
    BinaryNode binary;
    AssignNode assign;
    UpdateNode update;

    ExprStmtNode expr_stmt;
    VarDeclNode var_decl;
    BlockNode block;
    IfNode if_stmt;
    WhileNode while_stmt;
    ReturnNode ret;
  } as;
};

// allocates a zeroed node of the given type - used by the ast_new_* constructors
Node* ast_node_alloc(NodeType type, Token pos);

void ast_free(Node* node);
void ast_print(Node* node, int indent); // debug-only recursive dump

#endif // AST_H
