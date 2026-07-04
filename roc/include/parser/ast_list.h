#ifndef AST_LIST_H
#define AST_LIST_H

#include <stddef.h>

typedef struct Node Node;

typedef struct NodeList {
  Node** items;
  size_t count;
  size_t capacity;
} NodeList;

void nodelist_init(NodeList* list);
void nodelist_push(NodeList* list, Node* node);
void nodelist_free(NodeList* list);

#endif // AST_LIST_H
