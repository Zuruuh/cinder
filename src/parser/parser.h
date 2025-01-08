#pragma once

#include "statement.h"

typedef enum {
  ParserTokenKindStatement,
  ParserTokenKindExpression,
  ParserTokenKindBinaryExpression,
  ParserTokenKindTerm,
} ParserTokenKind;

typedef struct {
  ParserTokenKind kind;
  union {
    Statement statement;
  };
} ParserToken;
