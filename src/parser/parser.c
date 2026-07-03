#include "parser/parser.h"
#include "parser/parser_error.h"
#include "lexer/lexer.h"

#include <stdlib.h>
#include <string.h>

static void parser_advance(Parser* parser);
static bool parser_check(Parser* parser, TokenType type);
static bool parser_match(Parser* parser, TokenType type);
static Token parser_expect(Parser* parser, TokenType type, const char* message);
static void parser_error(Parser* parser, const char* message);
static void synchronize(Parser* parser);

static Node* parse_statement(Parser* parser);
static Node* parse_block(Parser* parser);
static Node* parse_var_decl(Parser* parser);
static Node* parse_if(Parser* parser);
static Node* parse_while(Parser* parser);
static Node* parse_return(Parser* parser);
static Node* parse_expr_stmt(Parser* parser);

static Node* parse_expression(Parser* parser);
static Node* parse_assignment(Parser* parser);
static Node* parse_binary(Parser* parser, int min_prec);
static Node* parse_unary(Parser* parser);
static Node* parse_primary(Parser* parser);

static int binop_precedence(TokenType type);
static bool is_type_token(TokenType type);
static bool is_assign_op(TokenType type);

static long parse_int_literal(Token token);
static double parse_float_literal(Token token);

Parser parser_init(Lexer* lexer) {
  Parser parser = {};
  parser.lexer = lexer;
  error_stack_init(&parser.errors);

  parser_advance(&parser); // prime parser.current
  return parser;
}

void parser_free(Parser* parser) {
  error_stack_free(&parser->errors);
}

Node* parse_program(Parser* parser) {
  Node* program = ast_new_block();

  while(!parser_check(parser, TOK_EOF)) {
    Node* stmt = parse_statement(parser);
    if(parser->panic_mode) synchronize(parser);
    if(stmt) ast_block_append(program, stmt);
  }

  return program;
}

static void parser_advance(Parser* parser) {
  parser->previous = parser->current;

  // TOK_ERROR tokens are lexer-level failures already recorded in
  // lexer->errors; skip them here instead of also raising a parser error
  Token token = lexer_next(parser->lexer);
  while(token.type == TOK_ERROR)
    token = lexer_next(parser->lexer);

  parser->current = token;
}

static bool parser_check(Parser* parser, TokenType type) {
  return parser->current.type == type;
}

static bool parser_match(Parser* parser, TokenType type) {
  if(!parser_check(parser, type)) return false;
  parser_advance(parser);
  return true;
}

static Token parser_expect(Parser* parser, TokenType type, const char* message) {
  if(!parser_check(parser, type)) {
    parser_error(parser, message);
    return parser->current; // caller may build a node from this; discarded on next synchronize()
  }

  Token token = parser->current;
  parser_advance(parser);
  return token;
}

static void parser_error(Parser* parser, const char* message) {
  if(parser->panic_mode) return; // already unwinding this statement, don't cascade
  parser->panic_mode = true;

  PARSER_ERROR(parser, UnexpectedTokenError, message,
      (int)parser->current.length, parser->current.start,
      parser->current.pos.line, parser->current.pos.column);
}

// skips tokens until we're at a plausible start of the next statement, so
// one bad statement doesn't take down the rest of the parse
static void synchronize(Parser* parser) {
  parser->panic_mode = false;

  while(!parser_check(parser, TOK_EOF)) {
    if(parser->previous.type == TOK_SEMICOLON) return;

    switch(parser->current.type) {
      case TOK_IF:
      case TOK_WHILE:
      case TOK_RETURN:
      case TOK_BREAK:
      case TOK_INT:
      case TOK_FLOAT:
      case TOK_BOOL:
      case TOK_LBRACE:
        return;
      default:
        break;
    }

    parser_advance(parser);
  }
}

/* ---- statements ---- */

static bool is_type_token(TokenType type) {
  return type == TOK_INT || type == TOK_FLOAT || type == TOK_BOOL;
}

static Node* parse_statement(Parser* parser) {
  if(parser_check(parser, TOK_LBRACE)) return parse_block(parser);
  if(is_type_token(parser->current.type)) return parse_var_decl(parser);
  if(parser_check(parser, TOK_IF)) return parse_if(parser);
  if(parser_check(parser, TOK_WHILE)) return parse_while(parser);
  if(parser_check(parser, TOK_RETURN)) return parse_return(parser);

  if(parser_check(parser, TOK_BREAK)) {
    Token pos = parser->current;
    parser_advance(parser);
    parser_expect(parser, TOK_SEMICOLON, "expected ';' after 'break'");
    return ast_new_break(pos);
  }

  return parse_expr_stmt(parser);
}

static Node* parse_block(Parser* parser) {
  parser_expect(parser, TOK_LBRACE, "expected '{'");
  Node* block = ast_new_block();

  while(!parser_check(parser, TOK_RBRACE) && !parser_check(parser, TOK_EOF)) {
    Node* stmt = parse_statement(parser);
    if(parser->panic_mode) synchronize(parser);
    if(stmt) ast_block_append(block, stmt);
  }

  parser_expect(parser, TOK_RBRACE, "expected '}'");
  return block;
}

static Node* parse_var_decl(Parser* parser) {
  TokenType type_tok = parser->current.type;
  parser_advance(parser); // consume type keyword

  Token name = parser_expect(parser, TOK_IDENTIFIER, "expected variable name");

  Node* initializer = NULL;
  if(parser_match(parser, TOK_ASSIGN))
    initializer = parse_expression(parser);

  parser_expect(parser, TOK_SEMICOLON, "expected ';' after variable declaration");
  return ast_new_var_decl(type_tok, name, initializer);
}

static Node* parse_if(Parser* parser) {
  Token pos = parser->current;
  parser_advance(parser); // 'if'

  parser_expect(parser, TOK_LPAREN, "expected '(' after 'if'");
  Node* condition = parse_expression(parser);
  parser_expect(parser, TOK_RPAREN, "expected ')' after condition");

  Node* then_branch = parse_statement(parser);
  Node* else_branch = NULL;
  if(parser_match(parser, TOK_ELSE))
    else_branch = parse_statement(parser);

  return ast_new_if(condition, then_branch, else_branch, pos);
}

static Node* parse_while(Parser* parser) {
  Token pos = parser->current;
  parser_advance(parser); // 'while'

  parser_expect(parser, TOK_LPAREN, "expected '(' after 'while'");
  Node* condition = parse_expression(parser);
  parser_expect(parser, TOK_RPAREN, "expected ')' after condition");

  Node* body = parse_statement(parser);
  return ast_new_while(condition, body, pos);
}

static Node* parse_return(Parser* parser) {
  Token pos = parser->current;
  parser_advance(parser); // 'return'

  Node* value = NULL;
  if(!parser_check(parser, TOK_SEMICOLON))
    value = parse_expression(parser);

  parser_expect(parser, TOK_SEMICOLON, "expected ';' after return statement");
  return ast_new_return(value, pos);
}

static Node* parse_expr_stmt(Parser* parser) {
  Token pos = parser->current;
  Node* expr = parse_expression(parser);
  parser_expect(parser, TOK_SEMICOLON, "expected ';' after expression");
  return ast_new_expr_stmt(expr, pos);
}

/* ---- expressions ---- */

static bool is_assign_op(TokenType type) {
  switch(type) {
    case TOK_ASSIGN:
    case TOK_PLUS_EQUAL:
    case TOK_MINUS_EQUAL:
    case TOK_STAR_EQUAL:
    case TOK_SLASH_EQUAL:
    case TOK_PERCENT_EQUAL:
    case TOK_CARET_EQUAL:
    case TOK_AMP_EQUAL:
    case TOK_PIPE_EQUAL:
      return true;
    default:
      return false;
  }
}

static Node* parse_expression(Parser* parser) {
  return parse_assignment(parser);
}

static Node* parse_assignment(Parser* parser) {
  Node* target = parse_binary(parser, 1);

  if(is_assign_op(parser->current.type)) {
    TokenType op = parser->current.type;
    Token pos = parser->current;
    parser_advance(parser);

    Node* value = parse_assignment(parser); // right-associative
    return ast_new_assign(op, target, value, pos);
  }

  return target;
}

// higher number = binds tighter
static int binop_precedence(TokenType type) {
  switch(type) {
    case TOK_OR:                                          return 1;
    case TOK_AND:                                          return 2;
    case TOK_BIT_OR:                                       return 3;
    case TOK_XOR:                                          return 4;
    case TOK_BIT_AND:                                      return 5;
    case TOK_EQUALS: case TOK_NOT_EQUALS:                  return 6;
    case TOK_GREATER: case TOK_LOWER:
    case TOK_GREATER_OR_EQUALS: case TOK_LOWER_OR_EQUALS:  return 7;
    case TOK_LEFT_SHIFT: case TOK_RIGHT_SHIFT:             return 8;
    case TOK_PLUS: case TOK_MINUS:                         return 9;
    case TOK_MULTIPLY: case TOK_DIVIDE: case TOK_PERCENT:  return 10;
    default:                                               return 0;
  }
}

static Node* parse_binary(Parser* parser, int min_prec) {
  Node* left = parse_unary(parser);

  for(;;) {
    int prec = binop_precedence(parser->current.type);
    if(prec == 0 || prec < min_prec) break;

    TokenType op = parser->current.type;
    Token pos = parser->current;
    parser_advance(parser);

    Node* right = parse_binary(parser, prec + 1); // left-associative
    left = ast_new_binary(op, left, right, pos);
  }

  return left;
}

static Node* parse_unary(Parser* parser) {
  if(parser_check(parser, TOK_MINUS) || parser_check(parser, TOK_NOT) ||
     parser_check(parser, TOK_BIT_NOT)) {
    TokenType op = parser->current.type;
    Token pos = parser->current;
    parser_advance(parser);

    Node* operand = parse_unary(parser);
    return ast_new_unary(op, operand, pos);
  }

  return parse_primary(parser);
}

static Node* parse_primary(Parser* parser) {
  Token token = parser->current;

  if(parser_match(parser, TOK_INT_LITERAL))
    return ast_new_literal_int(parse_int_literal(token), token);

  if(parser_match(parser, TOK_FLOAT_LITERAL))
    return ast_new_literal_float(parse_float_literal(token), token);

  if(parser_match(parser, TOK_TRUE))
    return ast_new_literal_bool(true, token);

  if(parser_match(parser, TOK_FALSE))
    return ast_new_literal_bool(false, token);

  if(parser_match(parser, TOK_IDENTIFIER))
    return ast_new_identifier(token);

  if(parser_match(parser, TOK_LPAREN)) {
    Node* expr = parse_expression(parser);
    parser_expect(parser, TOK_RPAREN, "expected ')' after expression");
    return expr;
  }

  parser_error(parser, "expected expression");
  parser_advance(parser); // consume the bad token so callers can't spin on it
  return NULL;
}

static int hex_digit_value(char c) {
  if(c >= '0' && c <= '9') return c - '0';
  if(c >= 'a' && c <= 'f') return c - 'a' + 10;
  return c - 'A' + 10; // caller only ever passes isxdigit() chars
}

// token text isn't NUL-terminated (it points into the source buffer), so
// literals are parsed by hand/bounded copy instead of strtol/strtod directly
static long parse_int_literal(Token token) {
  // 0x/0X hex and 0b/0B binary prefixes - anything else falls through to decimal
  if(token.length > 2 && token.start[0] == '0' && (token.start[1] == 'x' || token.start[1] == 'X')) {
    long value = 0;
    for(size_t i = 2; i < token.length; i++)
      value = value * 16 + hex_digit_value(token.start[i]);

    return value;
  }

  if(token.length > 2 && token.start[0] == '0' && (token.start[1] == 'b' || token.start[1] == 'B')) {
    long value = 0;
    for(size_t i = 2; i < token.length; i++)
      value = value * 2 + (token.start[i] - '0');

    return value;
  }

  long value = 0;
  for(size_t i = 0; i < token.length; i++)
    value = value * 10 + (token.start[i] - '0');

  return value;
}

static double parse_float_literal(Token token) {
  char buf[64];
  size_t len = token.length < sizeof(buf) - 1 ? token.length : sizeof(buf) - 1;

  memcpy(buf, token.start, len);
  buf[len] = '\0';

  return strtod(buf, NULL);
}
