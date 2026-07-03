#include "options/options.h"
#include "lexer/lexer.h"
#include "logger.h"

// NOTE: may move this to another file of build consts
// will stay here for now
#define FORCE_VERBOSE true


bool is_verbose = true;

#define HEADER "ROMA 0.0.1\n"

int main(int argc, char** argv)
{
  printf("%s", HEADER);

  Options options = {};
  init_options(&options, argv, argc);
  if(options.is_help) return 0;
  if(!options.filename) return 1;
  
  // a bit stinky but for debug
  is_verbose = FORCE_VERBOSE ? true : options.verbose;
  
  LOG("Starting to parse code\n");
  
  Lexer* lexer = lexer_from_file(options.filename);

  lexer_free(lexer);
  return 0;
}
