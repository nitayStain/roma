#ifndef AST_NODE_LIST_H
#define AST_NODE_LIST_H

/* expression nodes - values that evaluate to something */
#define AST_EXPR_NODE_LIST \
  X(NODE_LITERAL)     \
  X(NODE_IDENTIFIER)  \
  X(NODE_UNARY)       \
  X(NODE_BINARY)      \
  X(NODE_ASSIGN)      \
  X(NODE_UPDATE)

/* statement nodes - things that execute */
#define AST_STMT_NODE_LIST \
  X(NODE_EXPR_STMT)   \
  X(NODE_VAR_DECL)    \
  X(NODE_BLOCK)       \
  X(NODE_IF)          \
  X(NODE_WHILE)       \
  X(NODE_BREAK)       \
  X(NODE_RETURN)

/* adding a new node kind: add it to the matching list above, add its
 * payload struct + constructor to ast_expr.h/.c or ast_stmt.h/.c, then
 * handle it in the ast_free/ast_print switches in ast.c */
#define AST_NODE_LIST \
  AST_EXPR_NODE_LIST  \
  AST_STMT_NODE_LIST  \

#endif // AST_NODE_LIST_H
