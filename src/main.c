#include <roc/roc.h>
#include <napoli/napoli.h>

#include <errno.h>
#include <libgen.h>
#include <string.h>

// NOTE: may move this to another file of build consts
// will stay here for now
#define FORCE_VERBOSE true


bool is_verbose = true;

#define HEADER "ROMA 0.0.1\n"

int main(int argc, char** argv)
{
  Options options = {};
  init_options(&options, argv, argc);
  if(options.is_help) return 0;
  if(!options.filename) return 1;
  
  // a bit stinky but for debug
  is_verbose = FORCE_VERBOSE ? true : options.verbose;
  
  LOG("Starting to parse code\n");
  
  Lexer* lexer = lexer_from_file(options.filename);
  if(!lexer) {
    fprintf(stderr, "%s: failed reading '%s' (%s)\n", basename(argv[0]), options.filename, strerror(errno));
    return 1;
  }
  
  printf("%s", HEADER);
  
  Parser parser = parser_init(lexer);

  Node* program = parse_program(&parser);

  bool has_errors = error_stack_has_errors(&lexer->errors) || error_stack_has_errors(&parser.errors);
  if(has_errors) {
    error_stack_print(&lexer->errors, stderr);
    error_stack_print(&parser.errors, stderr);
  } else {
    ast_print(program, 0);

    napoli_init();
  }

  ast_free(program);
  parser_free(&parser);
  lexer_free(lexer);
  return has_errors ? 1 : 0;
}
