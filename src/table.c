#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void table_init(Table *t, const char *name) {
    memset(t, 0, sizeof(*t));
    strncpy(t->name, name, TABLE_NAME_LEN-1);
    t->records = NULL;
    t->count = 0;
    t->capacity = 0;
    t->next_id = 1;
}

void table_free(Table *t) {
    free(t->records);
    t->records = NULL;
    t->count = 0;
    t->capacity = 0;
}

int table_insert(Table *t, const char *name, int age) {
    if (t->count >= t->capacity) {
        size_t newcap = (t->capacity == 0) ? 8 : t->capacity * 2;
        Record *tmp = (Record*)realloc(t->records, newcap * sizeof(Record));
        if (!tmp) return 0;
        t->records = tmp;
        t->capacity = newcap;
    }
    Record *r = &t->records[t->count++];
    r->id = t->next_id++;
    strncpy(r->name, name, RECORD_NAME_LEN-1);
    r->name[RECORD_NAME_LEN-1] = '\0';
    r->age = age;
    return 1;
}

int table_delete_by_id(Table *t, int id) {
    for (size_t i = 0; i < t->count; ++i) {
        if (t->records[i].id == id) {
            t->records[i] = t->records[t->count - 1];
            t->count--;
            return 1;
        }
    }
    return 0;
}

void table_print_all(const Table *t) {
    if (!t) return;
    printf("Tabla '%s' (%zu registros)\n", t->name, t->count);
    for (size_t i = 0; i < t->count; ++i) {
        const Record *r = &t->records[i];
        printf("  id=%d  name=\"%s\"  age=%d\n", r->id, r->name, r->age);
    }
}

static int str_icmp(const char *a, const char *b) {
    while (*a && *b) {
        char ca = (*a >= 'A' && *a <= 'Z') ? (*a + 32) : *a;
        char cb = (*b >= 'A' && *b <= 'Z') ? (*b + 32) : *b;
        if (ca != cb) return (unsigned char)ca - (unsigned char)cb;
        a++; b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int table_matches_where(const Record *r, const char *field, const char *op, const char *value) {
    if (strcmp(field, "id") == 0) {
        int val = atoi(value);
        if (strcmp(op, "=") == 0) return r->id == val;
        if (strcmp(op, ">") == 0) return r->id > val;
        if (strcmp(op, "<") == 0) return r->id < val;
    } else if (strcmp(field, "age") == 0) {
        int val = atoi(value);
        if (strcmp(op, "=") == 0) return r->age == val;
        if (strcmp(op, ">") == 0) return r->age > val;
        if (strcmp(op, "<") == 0) return r->age < val;
    } else if (strcmp(field, "name") == 0) {
    
        if (strcmp(op, "=") == 0) return (str_icmp(r->name, value) == 0);
        if (strcmp(op, "like") == 0) {
          
            const char *s = r->name;
            size_t lv = strlen(value);
            for (; *s; ++s) {
                if (strncasecmp(s, value, lv) == 0) return 1;
            }
            return 0;
        }
    }
    return 0;
}

void table_select_where(const Table *t, const char *field, const char *op, const char *value) {
    if (!t) return;
    printf("Resultados en '%s':\n", t->name);
    int found = 0;
    for (size_t i = 0; i < t->count; ++i) {
        const Record *r = &t->records[i];
        if (table_matches_where(r, field, op, value)) {
            printf("  id=%d  name=\"%s\"  age=%d\n", r->id, r->name, r->age);
            found = 1;
        }
    }
    if (!found) printf("  (ningun registro)\n");
}
