#pragma once

// #include "<glib-2.0/glib.h>"
#include <glib.h>

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
  // HashTable registry;
} Scope;
