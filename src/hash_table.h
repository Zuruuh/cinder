#pragma once

#include <stdbool.h>
#include <stddef.h>

// Inspired by https://benhoyt.com/writings/hash-table-in-c/

typedef struct {
  const char *key;
  void *value;
} HashTableEntry;

typedef struct {
  HashTableEntry *entries;
  size_t capacity;
  size_t length;
} HashTable;

HashTable *hash_table_new(void);
void hash_table_destroy(HashTable *hash_table);
void *hash_table_get(HashTable *hash_table, const char *key);
const char *hash_table_set(HashTable *hash_table, const char *key, void *value);
size_t hash_table_length(HashTable *hash_table);

typedef struct {
  const char *key;
  void *value;

  HashTable *_hash_table;
  size_t _index;
} HashTableIterator;

HashTableIterator hash_table_iterate(HashTable *hash_table);

bool hash_table_iterator_next(HashTableIterator *hash_table_iterator);
