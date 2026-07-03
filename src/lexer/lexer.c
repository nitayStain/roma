#include "lexer/lexer.h"

#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename)
{
  FILE* file = fopen(filename, "rb");
  if(!file) return NULL;

  if(fseek(file, 0, SEEK_END) != 0) goto error_abort; 
  
  long file_size = ftell(file);
  if(file_size < 0) goto error_abort;

  if(fseek(file, 0, SEEK_SET) != 0) goto error_abort;

  char* buff = (char*) malloc(file_size);
  if(buff == NULL) goto error_abort;

  size_t bytes_read = fread(buff, 1, file_size, file);
  if(bytes_read < (size_t)file_size) {
    free(buff);
    goto error_abort;
  }
  
  fclose(file);
  return buff;
  
error_abort:
  fclose(file);
  return NULL;
}

Lexer* lexer_from_file(const char* filename)
{
  char* file_content = read_file(filename);
  if(!file_content) {
    perror("failed reading file");
    return NULL;
  }

  printf("%c%c%c\n", file_content[0], file_content[1], file_content[2]);

  free(file_content);
  return NULL;
}

Token* lexer_lex(Lexer* lexer)
{
  return NULL;
}
