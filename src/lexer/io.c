#include "lexer/lexer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filename, size_t* buffer_len)
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

  *buffer_len = bytes_read;

  fclose(file);
  return buff;

error_abort: {
   int saved_errno = errno;
    fclose(file);
    errno = saved_errno;
    return NULL;
  }
}

Lexer* lexer_from_file(const char* filename)
{
  size_t file_len;
  char* file_content = read_file(filename, &file_len);
  if(!file_content) return NULL;

  Lexer* lexer = (Lexer*)malloc(sizeof(Lexer));
  lexer->code = file_content;
  lexer->code_length = file_len;
  lexer->current = file_content;

  lexer->pos.column = 0;
  lexer->pos.line = 0;

  error_stack_init(&lexer->errors);

  return lexer;
}

