#include "command/run.h"
#include "command/type.h"
#include <stddef.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  Command command;
  switch (argc) {
  case 1:
    command.command_type = Help;
    break;
  default:
    command.command_type = parse_command(argv[1]);
  };

  switch (command.command_type) {
  case Run:
    if (argc != 3) {
      printf("You must specify a file to run\n");

      return 1;
    }
    command.data.run.file = argv[2];
  default:
    break;
  }

  switch (command.command_type) {
  case Version:
    printf("0.1.0\n");

    return 0;
  case None:
  case Help:
    printf("Cinder interpreter\n"
           "Usage:\n"
           "> cinder version # Print version\n"
           "> cinder help # Print this message\n"
           "> cinder run [file] # Run a file");
    return 0;
  case Run:
    return run_file(command);
  }
}
