#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
typedef enum {
  // User-defined tokens
  Identifier,
  // Reserved tokens
  Keyword,
  // Parenthesis, Curly braces, etc...
  Punctuation,
  // +, *, -, =, etc...
  Operator,
  // "strings", numbers (12, -42),
  StringLiteral,
  NumericLiteral,
  // # Comments
  Comment,
  Whitespace,
} LexerTokenKind;

typedef struct {
  size_t index;
  char *source;
  size_t source_len;
} Lexer;

typedef struct {
  size_t from;
  size_t to;
} Span;

typedef struct {
  LexerTokenKind kind;
  Span span;
} LexerToken;

typedef struct {
  LexerToken *tokens;
  size_t tokens_count;
} Program;

char *read_span(char *source, Span *span);
Program lex(char *source);
char *stringify_lexer_token(LexerToken token, char *source);

#endif // LEXER_H
