#include "../fs.h"
#include "../lexer.h"
#include "type.h"
#include <stddef.h>
#include <stdio.h>

void do_run_file(char *source) {
  /* Program program =  */ lex(source);

  // TODO: run
  /* for (size_t i = 0; i < program.tokens_count; i++) { */
  /*   LexerToken token = program.tokens[i]; */
  /*   printf("%s\n", stringify_lexer_token(token, source)); */
  /* } */
}

int run_file(Command command) {
  ReadFileResult maybe_file_content = read_file(command.data.run.file);
  switch (maybe_file_content.type) {
  case Ok:
    do_run_file(maybe_file_content.ok);
    break;
  case Err:

    fprintf(stderr, "\033[31m> %s\33[0m\n",
            stringify_file_error(maybe_file_content.err));

    return 1;
  }

  return 0;
}
