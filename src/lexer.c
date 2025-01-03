#include "lexer.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool done(Lexer *lexer) { return lexer->source_len <= lexer->index; }

char consume(Lexer *lexer) {
  size_t index = lexer->index;
  lexer->index++;

  return lexer->source[index];
}

char current(Lexer *lexer) { return lexer->source[lexer->index]; }
char peek(Lexer *lexer) { return lexer->source[lexer->index + 1]; }

/* Return the skipped tokens count */
size_t consume_until(Lexer *lexer, const char target) {
  size_t initial = lexer->index;
  while (!done(lexer)) {
    if (target == current(lexer)) {
      break;
    }

    consume(lexer);
  }
  lexer->index--;

  return lexer->index - initial;
}

LexerToken lex_comment(Lexer *lexer) {

  LexerToken token = (LexerToken){
      .kind = Comment,
      .span = (Span){.from = lexer->index,
                     .to = lexer->index + consume_until(lexer, '\n')}};

  // printf("Parsed comment from %zu->%zu {%zu}\n", token.span.from,
  // token.span.to, lexer->index);

  return token;
}

LexerToken lex_whitespace(Lexer *lexer) {
  size_t from = lexer->index;

  while (!done(lexer)) {
    char c = current(lexer);
    if (c != '\t' && c != ' ' && c != '\n') {
      break;
    }

    consume(lexer);
  }
  lexer->index--;

  // printf("Parsed whitespace from %zu->%zu\n", from, lexer->index);

  return (LexerToken){.kind = Whitespace,
                      .span = (Span){.from = from, .to = lexer->index}};
}

bool is_numeric(char token) { return token >= '0' && token <= '9'; }

bool could_be_word_start(char token) {
  return token == '_' || (token >= 'A' && token <= 'Z') ||
         (token >= 'a' && token <= 'z');
}

bool could_be_word_char(char token) {
  return could_be_word_start(token) || is_numeric(token);
}

bool is_reserved_keyword(char *keyword) {
  static const char *keywords[] = {
      "if", "else", "return", "def", "True", "False",
  };

  size_t keywords_count = sizeof(keywords) / sizeof(keywords[0]);

  for (size_t i = 0; i < keywords_count; i++) {
    if (strcmp(keyword, keywords[i]) == 0) {
      return true;
    }
  }

  return false;
}

LexerToken lex_word(Lexer *lexer) {
  size_t from = lexer->index;

  while (!done(lexer)) {
    char token = current(lexer);
    if (!could_be_word_char(token)) {
      break;
    }

    consume(lexer);
  }
  lexer->index--;

  Span span = {.from = from, .to = lexer->index};

  LexerTokenKind kind;
  {
    char *content = read_span(lexer->source, &span);
    if (is_reserved_keyword(content)) {
      kind = Keyword;
    } else {
      kind = Identifier;
    }
  }

  return (LexerToken){.kind = kind, .span = span};
}

char *read_span(char *source, Span *span) {
  size_t span_length = span->to - span->from;

  char *span_content = malloc(span_length + 1);
  memcpy(span_content, source + span->from, span_length + 1);
  span_content[span_length + 1] = '\0';

  return span_content;
}

LexerToken lex_numeric_literal(Lexer *lexer) {
  size_t from = lexer->index;

  while (!done(lexer)) {
    char token = current(lexer);
    if (!is_numeric(token)) {
      break;
    }

    consume(lexer);
  }
  lexer->index--;

  // printf("From %zu To %zu", from, lexer->index);

  return (LexerToken){.kind = NumericLiteral,
                      .span = {.from = from, .to = lexer->index}};
}

LexerToken lex_string_literal(Lexer *lexer, char string_delimiter) {
  size_t from = lexer->index;

  while (!done(lexer)) {
    char token = current(lexer);
    char next = peek(lexer);
    consume(lexer);

    if (token != '\\' && next == string_delimiter) {
      break;
    }
  }

  return (LexerToken){.kind = StringLiteral,
                      .span = {.from = from, .to = lexer->index}};
}

Program lex(char *source) {
  Lexer lexer = {.source = source, .index = 0, .source_len = strlen(source)};

  // TODO: make this resize-able
  LexerToken *tokens = malloc(105 * sizeof(LexerToken));
  size_t token_index = 0;

  while (!done(&lexer)) {
    char token = current(&lexer);
    // printf("Starting to parse from token `%c`@%zu\n", token, lexer.index);

    switch (token) {
    case '#':
      tokens[token_index++] = lex_comment(&lexer);

      break;

    case ' ':
    case '\t':
    case '\n':
      tokens[token_index++] = lex_whitespace(&lexer);
      break;
    case '\'':
    case '"':
      tokens[token_index++] = lex_string_literal(&lexer, token);
      break;
    case '(':
    case ')':
    case '{':
    case '}':
    case ':':
      tokens[token_index++] =
          (LexerToken){.kind = Punctuation,
                       .span = {.to = lexer.index, .from = lexer.index}};

      break;
    default:
      if (is_numeric(token)) {
        tokens[token_index++] = lex_numeric_literal(&lexer);
      } else if (could_be_word_start(token)) {
        tokens[token_index++] = lex_word(&lexer);
      }

      break;
    }

    consume(&lexer);
  }

  return (Program){.tokens_count = token_index, .tokens = tokens};
}
