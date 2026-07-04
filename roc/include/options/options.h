#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef struct Options {
  char* filename;
  char verbose;
  bool is_help;
} Options;

// Init the options struct
void init_options(Options* options, char** argv, int argc);

#endif // OPTIONS_H
