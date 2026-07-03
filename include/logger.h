#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdbool.h>

extern bool is_verbose;

#define LOG(fmt, ...) \
  do { \
    if(is_verbose) \
      fprintf(stdout, fmt, ##__VA_ARGS__); \
  } while(0)

#endif // LOGGER_H
