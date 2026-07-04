#include "error.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_STACK_INITIAL_CAPACITY 8

void error_stack_init(ErrorStack* stack) {
  stack->items = NULL;
  stack->count = 0;
  stack->capacity = 0;
}

void error_stack_push(ErrorStack* stack, int type, const char* type_name, const char* fmt, ...) {
  if(stack->count == stack->capacity) {
    stack->capacity = stack->capacity ? stack->capacity * 2 : ERROR_STACK_INITIAL_CAPACITY;
    stack->items = (Error*) realloc(stack->items, stack->capacity * sizeof(Error));
  }

  char buf[256];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);

  Error* error = &stack->items[stack->count++];
  error->type = type;
  error->type_name = type_name;
  error->message = strdup(buf);
}

bool error_stack_has_errors(ErrorStack* stack) {
  return stack->count > 0;
}

void error_stack_print(ErrorStack* stack, FILE* out) {
  // only the first error prints for now, to avoid cascades of noise from
  // one root cause; once warnings exist (a distinct, non-fatal severity)
  // every warning should still print, just not every error
  if(stack->count == 0) return;

  Error* error = &stack->items[0];
  fprintf(out, "%s: %s\n", error->type_name, error->message);
}

void error_stack_free(ErrorStack* stack) {
  for(size_t i = 0; i < stack->count; i++)
    free(stack->items[i].message);

  free(stack->items);
  stack->items = NULL;
  stack->count = 0;
  stack->capacity = 0;
}

void error_stack_extend(ErrorStack* dest, ErrorStack* src) {
  for(size_t i = 0; i < src->count; i++) {
    if(dest->count == dest->capacity) {
      dest->capacity = dest->capacity ? dest->capacity * 2 : ERROR_STACK_INITIAL_CAPACITY;
      dest->items = (Error*) realloc(dest->items, dest->capacity * sizeof(Error));
    }
    dest->items[dest->count++] = src->items[i];
  }

  free(src->items);
  src->items = NULL;
  src->count = 0;
  src->capacity = 0;
}
