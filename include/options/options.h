#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct Options {
  char* filename;
  char verbose;
} Options;

// Init the options struct
void init_options(Options* options, char** argv, int argc);

#endif // OPTIONS_H
