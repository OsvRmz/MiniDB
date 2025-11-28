#include "database.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void db_init(Database *db) {
    db->tables = NULL;
    db->count = 0;
    db->capacity = 0;
}

void db_free(Database *db) {
    if (!db) return;
    for (size_t i = 0; i < db->count; ++i) {
        table_free(&db->tables[i]);
    }
    free(db->tables);
    db->tables = NULL;
    db->count = db->capacity = 0;
}

int db_create_table(Database *db, const char *name) {
    if (db_find_table(db, name)) {
        printf("La tabla '%s' ya existe.\n", name);
        return 0;
    }
    if (db->count >= db->capacity) {
        size_t ncap = (db->capacity == 0) ? 4 : db->capacity * 2;
        Table *tmp = (Table*)realloc(db->tables, ncap * sizeof(Table));
        if (!tmp) return 0;
        db->tables = tmp;
        db->capacity = ncap;
    }
    table_init(&db->tables[db->count], name);
    db->count++;
    printf("Tabla '%s' creada.\n", name);
    return 1;
}

Table* db_find_table(Database *db, const char *name) {
    if (!db) return NULL;
    for (size_t i = 0; i < db->count; ++i) {
        if (strncmp(db->tables[i].name, name, TABLE_NAME_LEN) == 0) return &db->tables[i];
    }
    return NULL;
}

int db_insert(Database *db, const char *table, const char *name, int age) {
    Table *t = db_find_table(db, table);
    if (!t) return 0;
    return table_insert(t, name, age);
}

void db_select_all(Database *db, const char *table) {
    Table *t = db_find_table(db, table);
    if (!t) { printf("Tabla '%s' no existe.\n", table); return; }
    table_print_all(t);
}

void db_select_where(Database *db, const char *table, const char *field, const char *op, const char *value) {
    Table *t = db_find_table(db, table);
    if (!t) { printf("Tabla '%s' no existe.\n", table); return; }
    table_select_where(t, field, op, value);
}

int db_delete_by_id(Database *db, const char *table, int id) {
    Table *t = db_find_table(db, table);
    if (!t) return 0;
    return table_delete_by_id(t, id);
}

// Serialization format (binary):
// uint32_t table_count
// for each table:
//   char name[TABLE_NAME_LEN]
//   uint32_t record_count
//   int32_t next_id
//   for each record:
//     int32_t id
//     char name[RECORD_NAME_LEN]
//     int32_t age

int db_save(Database *db, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    uint32_t tcount = (uint32_t)db->count;
    fwrite(&tcount, sizeof(tcount), 1, f);
    for (size_t i = 0; i < db->count; ++i) {
        Table *t = &db->tables[i];
        fwrite(t->name, sizeof(char), TABLE_NAME_LEN, f);
        uint32_t rcount = (uint32_t)t->count;
        fwrite(&rcount, sizeof(rcount), 1, f);
        int32_t nextid = t->next_id;
        fwrite(&nextid, sizeof(nextid), 1, f);
        for (size_t r = 0; r < t->count; ++r) {
            Record *rec = &t->records[r];
            int32_t id = rec->id;
            fwrite(&id, sizeof(id), 1, f);
            fwrite(rec->name, sizeof(char), RECORD_NAME_LEN, f);
            int32_t age = rec->age;
            fwrite(&age, sizeof(age), 1, f);
        }
    }
    fclose(f);
    return 1;
}

int db_load(Database *db, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    db_free(db);
    db_init(db);
    uint32_t tcount = 0;
    if (fread(&tcount, sizeof(tcount), 1, f) != 1) { fclose(f); return 0; }
    for (uint32_t i = 0; i < tcount; ++i) {
        char tname[TABLE_NAME_LEN];
        if (fread(tname, sizeof(char), TABLE_NAME_LEN, f) != TABLE_NAME_LEN) { fclose(f); return 0; }
        db_create_table(db, tname);
        Table *t = &db->tables[db->count - 1];
        uint32_t rcount = 0;
        if (fread(&rcount, sizeof(rcount), 1, f) != 1) { fclose(f); return 0; }
        int32_t nextid = 1;
        if (fread(&nextid, sizeof(nextid), 1, f) != 1) { fclose(f); return 0; }
        t->next_id = nextid;
        for (uint32_t r = 0; r < rcount; ++r) {
            int32_t id = 0;
            char rname[RECORD_NAME_LEN];
            int32_t age = 0;
            if (fread(&id, sizeof(id), 1, f) != 1) { fclose(f); return 0; }
            if (fread(rname, sizeof(char), RECORD_NAME_LEN, f) != RECORD_NAME_LEN) { fclose(f); return 0; }
            if (fread(&age, sizeof(age), 1, f) != 1) { fclose(f); return 0; }
            table_insert(t, rname, age);
     
            t->records[t->count - 1].id = id;
        }
    }
    fclose(f);
    return 1;
}
