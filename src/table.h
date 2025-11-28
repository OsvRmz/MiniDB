#ifndef TABLE_H
#define TABLE_H

#include <stddef.h>

#define TABLE_NAME_LEN 32
#define RECORD_NAME_LEN 64

typedef struct {
    int id;
    char name[RECORD_NAME_LEN];
    int age;
} Record;

typedef struct {
    char name[TABLE_NAME_LEN];
    Record *records;
    size_t count;
    size_t capacity;
    int next_id;
} Table;

void table_init(Table *t, const char *name);
void table_free(Table *t);
int table_insert(Table *t, const char *name, int age);
int table_delete_by_id(Table *t, int id);
void table_print_all(const Table *t);
int table_matches_where(const Record *r, const char *field, const char *op, const char *value);
void table_select_where(const Table *t, const char *field, const char *op, const char *value);

#endif 