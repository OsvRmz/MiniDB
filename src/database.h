#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"

typedef struct {
    Table *tables;
    size_t count;
    size_t capacity;
} Database;

void db_init(Database *db);
void db_free(Database *db);
int db_create_table(Database *db, const char *name);
Table* db_find_table(Database *db, const char *name);
int db_insert(Database *db, const char *table, const char *name, int age);
void db_select_all(Database *db, const char *table);
void db_select_where(Database *db, const char *table, const char *field, const char *op, const char *value);
int db_delete_by_id(Database *db, const char *table, int id);
int db_save(Database *db, const char *filename);
int db_load(Database *db, const char *filename);

#endif 
