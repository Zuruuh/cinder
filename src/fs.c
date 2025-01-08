#include "fs.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *stringify_file_error(ReadFileError err) {
  switch (err) {
  case ReadFileErrorCouldNotOpenFile:
    return "Could not open file";
  case ReadFileErrorNotEnoughMemory:
    return "Not enough memory to allocate!";
  case ReadFileErrorDidNotReadExpectedBytes:
    // TODO: recheck if this can even happen ?
    return "Did not read expected bytes count ?";
  default:
    return "Unhandled case";
  }
}

ReadFileResult read_file(char *file) {
  FILE *fptr = fopen(file, "r");

  if (fptr == NULL) {
    ReadFileResult result = {.type = Err, .err = ReadFileErrorCouldNotOpenFile};

    return result;
  }

  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);
  char *buffer = malloc(file_size + 1);

  if (buffer == NULL) {
    free(buffer);
    fclose(fptr);
    return (ReadFileResult){.type = Err, .err = ReadFileErrorNotEnoughMemory};
  }

  size_t bytes_read = fread(buffer, 1, file_size, fptr);
  // IDK how this can happen honestly ?
  if (bytes_read != file_size) {
    free(buffer);
    fclose(fptr);

    return (ReadFileResult){.type = Err,
                            .err = ReadFileErrorDidNotReadExpectedBytes};
  }

  buffer[bytes_read] = '\0';
  return (ReadFileResult){.type = Ok, .ok = buffer};
}
