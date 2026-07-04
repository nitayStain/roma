#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <stdio.h>

extern bool is_verbose;

#define LOG(fmt, ...) \
  do { \
    if(is_verbose) \
      fprintf(stdout, fmt, ##__VA_ARGS__); \
  } while(0)

#endif // LOGGER_H
