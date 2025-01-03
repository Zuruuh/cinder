#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

typedef enum {
  None,
  Help,
  Version,
  Run,
} CommandType;

typedef struct {
  CommandType command_type;
  union {
    struct {
      char *file;
    } run;
  } data;
} Command;

CommandType parse_command(char *command);

#endif // COMMAND_TYPE_H
