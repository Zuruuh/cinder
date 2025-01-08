#pragma once

typedef enum {
  None,
  Help,
  Version,
  Run,
  Lex,
} CommandType;

typedef struct {
  CommandType command_type;
  union {
    struct {
      char *file;
    } run;

    struct {
      char *file;
    } lex;
  } data;
} Command;

CommandType parse_command(char *command);
