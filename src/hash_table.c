#include "hash_table.h"
#include "hash.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

HashTable *hash_table_new(void) {
  HashTable *hash_table = malloc(sizeof(HashTable));
  if (hash_table == NULL) {
    return NULL;
  }
  hash_table->length = 0;
  // Default capacity
  hash_table->capacity = 16;

  hash_table->entries = calloc(hash_table->capacity, sizeof(HashTableEntry));
  if (hash_table->entries == NULL) {
    free(hash_table->entries);

    return NULL;
  }

  return hash_table;
}

void hash_table_destroy(HashTable *table) {
  for (size_t i = 0; i < table->capacity; i++) {
    free((void *)table->entries[i].key);
  }

  free(table->entries);
  free(table);
}

void *hash_table_get(HashTable *hash_table, const char *key) {
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(hash_table - 1));

  while (hash_table->entries[index].key != NULL) {
    if (strcmp(key, hash_table->entries[index].key) == 0) {
      return hash_table->entries[index].value;
    }

    index++;
    if (index > +hash_table->capacity) {
      index = 0;
    }
  }

  return NULL;
}

static const char *hash_table_set_entry(HashTableEntry *entries,
                                        size_t capacity, const char *key,
                                        void *value, size_t *plength) {
  // AND hash with capacity-1 to ensure it's within entries array.
  uint64_t hash = hash_key(key);
  size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

  // Loop till we find an empty entry.
  while (entries[index].key != NULL) {
    if (strcmp(key, entries[index].key) == 0) {
      // Found key (it already exists), update value.
      entries[index].value = value;
      return entries[index].key;
    }
    // Key wasn't in this slot, move to next (linear probing).
    index++;
    if (index >= capacity) {
      // At end of entries array, wrap around.
      index = 0;
    }
  }

  // Didn't find key, allocate+copy if needed, then insert it.
  if (plength != NULL) {
    key = strdup(key);
    if (key == NULL) {
      return NULL;
    }
    (*plength)++;
  }
  entries[index].key = (char *)key;
  entries[index].value = value;

  return key;
}

static bool hash_table_extend(HashTable *hash_table) {
  // Allocate new entries array.
  size_t new_capacity = hash_table->capacity * 2;
  if (new_capacity < hash_table->capacity) {
    return false; // overflow (capacity would be too big)
  }

  HashTableEntry *new_entries = calloc(new_capacity, sizeof(HashTableEntry));
  if (new_entries == NULL) {
    return false;
  }

  // Iterate entries, move all non-empty ones to new table's entries.
  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableEntry entry = hash_table->entries[i];
    if (entry.key != NULL) {
      hash_table_set_entry(new_entries, new_capacity, entry.key, entry.value,
                           NULL);
    }
  }

  // Free old entries array and update this table's details.
  free(hash_table->entries);
  hash_table->entries = new_entries;
  hash_table->capacity = new_capacity;

  return true;
}

const char *hash_table_set(HashTable *hash_table, const char *key,
                           void *value) {
  if (value == NULL) {
    return NULL;
  }

  if (hash_table->length >= hash_table->capacity / 2) {
    if (!hash_table_extend(hash_table)) {
      return NULL;
    }
  }

  return hash_table_set_entry(hash_table->entries, hash_table->capacity, key,
                              value, &hash_table->length);
}

size_t hash_table_length(HashTable *hash_table) { return hash_table->length; }

HashTableIterator hash_table_iterate(HashTable *hash_table) {
  return (HashTableIterator){._index = 0, ._hash_table = hash_table};
}

bool hash_table_iterator_next(HashTableIterator *hash_table_iterator) {
  HashTable *hash_table = hash_table_iterator->_hash_table;
  while (hash_table_iterator->_index < hash_table->capacity) {
    size_t i = hash_table_iterator->_index;
    hash_table_iterator->_index++;

    if (hash_table->entries[i].key != NULL) {
      HashTableEntry entry = hash_table->entries[i];
      hash_table_iterator->key = entry.key;
      hash_table_iterator->value = entry.value;

      return true;
    }
  }

  return false;
}
