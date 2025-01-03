#include "../fs.h"
#include "../lexer.h"
#include "type.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *stringify_lexer_token(LexerToken token, char *source) {
  char *repr;
  switch (token.kind) {
  case Identifier:
    repr = "Identifier";
    break;
  case Keyword:
    repr = "Keyword";
    break;
  case Punctuation:
    repr = "Punctuation";
    break;
  case Operator:
    repr = "Operator";
    break;
  case StringLiteral:
    repr = "StringLiteral";
    break;
  case NumericLiteral:
    repr = "NumericLiteral";
    break;
  case Comment:
    repr = "Comment";
    break;
  case Whitespace:
    repr = "Whitespace";
    break;
  };

  if (token.kind == Whitespace) {
    return repr;
  }

  char *span_content = read_span(source, &token.span);

  size_t str_repr_length = sizeof(repr) +
                           sizeof(token.span.to - token.span.from) + 5 +
                           (15 * sizeof(char));
  char *str_repr = (char *)malloc(str_repr_length);
  snprintf(str_repr, str_repr_length, "%s(`%s`)", repr, span_content);
  // printf("%s(`%s`)\n", repr, span_content);
  return str_repr;
}

void do_run_file(char *source) {
  Program program = lex(source);

  for (size_t i = 0; i < program.tokens_count; i++) {
    LexerToken token = program.tokens[i];
    printf("%s\n", stringify_lexer_token(token, source));
  }
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
