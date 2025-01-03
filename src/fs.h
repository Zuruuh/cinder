#ifndef FS_H
#define FS_H

#include <stdio.h>
typedef enum {
  CouldNotOpenFile,
  NotEnoughMemory,
  DidNotReadExpectedBytes,
} ReadFileError;

typedef enum { Ok, Err } ResultType;

typedef struct {
  ResultType type;
  union {
    char *ok;
    ReadFileError err;
  };
} ReadFileResult;

char *stringify_file_error(ReadFileError err);

ReadFileResult read_file(char *file);

#endif // FS_H
