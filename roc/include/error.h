#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
  int type;               // enum value from the owning module's *_error.h, opaque here
  const char* type_name;  // e.g. "UnexpectedCharacterError" - stringified enum constant
  char* message;          // owned, heap-allocated, already rendered from the type's template
} Error;

typedef struct {
  Error* items;
  size_t count;
  size_t capacity;
} ErrorStack;

void error_stack_init(ErrorStack* stack);

void error_stack_push(ErrorStack* stack, int type, const char* type_name, const char* fmt, ...)
  __attribute__((format(printf, 4, 5)));
bool error_stack_has_errors(ErrorStack* stack);
void error_stack_print(ErrorStack* stack, FILE* out);
void error_stack_free(ErrorStack* stack);

#endif // ERROR_H
