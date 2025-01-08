#pragma once

#include <stdio.h>
typedef enum {
  ReadFileErrorCouldNotOpenFile,
  ReadFileErrorNotEnoughMemory,
  ReadFileErrorDidNotReadExpectedBytes,
} ReadFileError;

typedef enum { Ok, Err } ResultType;

typedef struct {
  ResultType type;
  union {
    char *ok;
    ReadFileError err;
  };
} ReadFileResult;

const char *stringify_file_error(ReadFileError err);

ReadFileResult read_file(char *file);
