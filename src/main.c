#include "options/options.h"
#include "lexer/lexer.h"
#include "logger.h"

bool is_verbose = true; // TODO: change is_verbose to false on release 
 
#define HEADER "ROMA 0.0.1\n"

int main(int argc, char** argv)
{
  Options options = {};
  init_options(&options, argv, argc);
  if(!options.filename) {
    return 1;
  }  
 
  is_verbose = options.verbose;
  
  LOG("Starting to parse code\n");
  
  Lexer* lexer = lexer_from_file(options.filename);
  Token* result = lexer_lex(lexer);


  return 0;
}
