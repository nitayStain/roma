#include <roc/roc.h>
#include <napoli/napoli.h>

#include <errno.h>
#include <libgen.h>
#include <string.h>

// NOTE: may move this to another file of build consts
// will stay here for now
#define FORCE_VERBOSE true


bool is_verbose = true;

int main(int argc, char** argv)
{
  Options options = {};
  init_options(&options, argv, argc);
  if(options.is_help) return 0;
  if(!options.filename) return 1;
  
  // a bit stinky but for debug
  is_verbose = FORCE_VERBOSE ? true : options.verbose;
  
  LOG("Starting to parse code\n");

  Program* program = program_from_file(options.filename);
  if(!program) {
    fprintf(stderr, "%s: failed reading '%s' (%s)\n", basename(argv[0]), options.filename, strerror(errno));
    return 1;
  }

  bool has_errors = program_has_errors(program);
  if(has_errors) {
    program_print_errors(program, stderr);
  } else {
    ast_print(program->ast, 0);

    // napoli_init();
  }

  program_free(program);
  return has_errors ? 1 : 0;
}
