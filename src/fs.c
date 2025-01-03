#include "fs.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *stringify_file_error(ReadFileError err) {
  switch (err) {
  case CouldNotOpenFile:
    return "Could not open file";
  case NotEnoughMemory:
    return "Not enough memory to allocate!";
  case DidNotReadExpectedBytes:
    // TODO: recheck if this can even happen ?
    return "Did not read expected bytes count ?";
  }
}

ReadFileResult read_file(char *file) {
  FILE *fptr = fopen(file, "r");

  if (fptr == NULL) {
    ReadFileResult result = {.type = Err, .err = CouldNotOpenFile};

    return result;
  }

  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buffer = malloc(file_size + 1);

  if (buffer == NULL) {
    free(buffer);
    fclose(fptr);
    return (ReadFileResult){.type = Err, .err = NotEnoughMemory};
  }

  size_t bytes_read = fread(buffer, 1, file_size, fptr);
  // IDK how this can happen honestly ?
  if (bytes_read != file_size) {
    free(buffer);
    fclose(fptr);

    return (ReadFileResult){.type = Err, .err = DidNotReadExpectedBytes};
  }

  buffer[bytes_read] = '\0';
  return (ReadFileResult){.type = Ok, .ok = buffer};
}
