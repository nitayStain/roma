#ifndef AST_STMT_H
#define AST_STMT_H

#include "lexer/token.h"
#include "parser/ast_list.h"

typedef struct Node Node;

typedef struct ExprStmtNode {
  Node* expr;
} ExprStmtNode;

typedef struct VarDeclNode {
  TokenType type_tok;
  Token name;
  Node* initializer; // NULL if not initialized
} VarDeclNode;

typedef struct BlockNode {
  NodeList statements;
} BlockNode;

typedef struct IfNode {
  Node* condition;
  Node* then_branch;
  Node* else_branch; // NULL if no else
} IfNode;

typedef struct WhileNode {
  Node* condition;
  Node* body;
} WhileNode;

typedef struct ReturnNode {
  Node* value; // NULL for bare 'return;'
} ReturnNode;

/* break carries no payload */

void nodelist_init(NodeList* list);
void nodelist_push(NodeList* list, Node* node);
void nodelist_free(NodeList* list);

Node* ast_new_expr_stmt(Node* expr, Token pos);
Node* ast_new_var_decl(TokenType type_tok, Token name, Node* initializer);
Node* ast_new_block(void);
void  ast_block_append(Node* block, Node* stmt);
Node* ast_new_if(Node* condition, Node* then_branch, Node* else_branch, Token pos);
Node* ast_new_while(Node* condition, Node* body, Token pos);
Node* ast_new_break(Token pos);
Node* ast_new_return(Node* value, Token pos);

#endif // AST_STMT_H
