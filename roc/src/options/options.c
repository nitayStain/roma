#include "options/options.h"

#include <stdio.h>
#include <unistd.h>
#include <libgen.h>

void print_help_usage(FILE* fd, const char* program_name) {
  char* prog_name = (char*) program_name;
  char* binary_name = basename(prog_name);
  fprintf(fd, "Usage: %s [-v] [-h] <filename>\n", binary_name);
  fprintf(fd, "Options:\n");
  fprintf(fd, " | -v\tEnable verbose output\n");
  fprintf(fd, " | -h\tDisplay the help message\n");
}

void init_options(Options* options, char** argv, int argc)
{
  options->is_help = false;
  options->filename = NULL;
  char opt;

  while((opt = getopt(argc, argv, "vh")) != -1) {
    switch(opt) {
      case 'h':
        options->is_help = 1;
        goto usage;
        break;
      case 'v':
        options->verbose = 1;
        break;
      default:
        goto usage;
    }
  }
  
  if(optind < argc) {
    options->filename = argv[optind];
    return;
  } else {
    fprintf(stderr, "Error: Missing filename\n\n");
  }

usage:
  print_help_usage(options->is_help ? stdout : stderr, argv[0]);
}
