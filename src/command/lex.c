#include "../fs.h"
#include "../lexer.h"
#include "type.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *stringify_lexer_token_kind(LexerTokenKind token_kind) {
  switch (token_kind) {
  case LexerTokenKindIdentifier:
    return "Identifier";
  case LexerTokenKindKeyword:
    return "Keyword";
  case LexerTokenKindPunctuation:
    return "Punctuation";
  case LexerTokenKindOperator:
    return "Operator";
  case LexerTokenKindStringLiteral:
    return "StringLiteral";
  case LexerTokenKindNumericLiteral:
    return "NumericLiteral";
  case LexerTokenKindComment:
    return "Comment";
  case LexerTokenKindWhitespace:
    return "Whitespace";
  default:
    printf("Unhandled lexer token?");
    return "";
  }
}

char *stringify_lexer_token(LexerToken token, char *source) {
  const char *repr = stringify_lexer_token_kind(token.kind);

  if (token.kind == LexerTokenKindWhitespace) {
    char *_repr = (char *)malloc(strlen(repr) + 1);
    strcpy(_repr, repr);

    return _repr;
  }

  char *span_content = read_span(source, &token.span);

  size_t str_repr_length =
      (strlen(repr) + 1 + token.span.to - token.span.from + 5) * sizeof(char);

  char *str_repr = (char *)malloc(str_repr_length);
  snprintf(str_repr, str_repr_length, "%s(`%s`)", repr, span_content);

  return str_repr;
}

void do_lex_file(char *source) {
  Program program = lex(source);

  for (size_t i = 0; i < program.tokens_count; i++) {
    LexerToken token = program.tokens[i];
    printf("%s\n", stringify_lexer_token(token, source));
  }
}

int lex_file(Command command) {
  ReadFileResult maybe_file_content = read_file(command.data.run.file);
  switch (maybe_file_content.type) {
  case Ok:
    do_lex_file(maybe_file_content.ok);
    break;
  case Err:

    fprintf(stderr, "\033[31m> %s\33[0m\n",
            stringify_file_error(maybe_file_content.err));

    return 1;
  }

  return 0;
}
