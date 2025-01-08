#pragma once

#include "../hash_table.h"

typedef enum {
  StatementAssignement,
  StatementFunctionDefinition,
  StatementIf,
  StatementReturn,
} StatementKind;

typedef struct {
} Expression;

typedef struct {
  StatementKind kind;

  union {
    struct {
      Expression expression;
    } _return;

  } data;
} Statement;

typedef struct {
  HashTable registry;
} Scope;
