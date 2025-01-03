#include "type.h"
#include <string.h>

CommandType parse_command(char *command) {
  if (strcmp(command, "version") == 0) {
    return Version;
  }

  if (strcmp(command, "help") == 0) {
    return Help;
  }

  if (strcmp(command, "run") == 0) {
    return Run;
  }

  return None;
};
