#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "database.h"

#define DB_FILENAME "data.db"
#define LINE_MAX 512

static void prompt(void) {
    printf("> ");
    fflush(stdout);
}

int main(void) {
    char line[LINE_MAX];
    Database db;
    db_init(&db);
    db_load(&db, DB_FILENAME); 

    printf("mini-db: motor de BD. Escribe 'help' para ayuda.\n");
    while (1) {
        prompt();
        if (!fgets(line, sizeof(line), stdin)) break;
        size_t ln = strlen(line);
        if (ln && line[ln-1] == '\n') line[ln-1] = '\0';

        char **argv = NULL;
        int argc = 0;
        parse_tokens(line, &argv, &argc);
        if (argc == 0) { free_tokens(argv, argc); continue; }

        if (strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "quit") == 0) {
            free_tokens(argv, argc);
            break;
        } else if (strcmp(argv[0], "help") == 0) {
            printf("Comandos:\n");
            printf("  create table <name>\n");
            printf("  insert <table> \"<name>\" <age>\n");
            printf("  select <table> [where <field> <op> <value>]\n");
            printf("  delete <table> where id = <n>\n");
            printf("  save [filename]\n");
            printf("  load [filename]\n");
            printf("  exit\n");
        } else if (strcmp(argv[0], "create") == 0 && argc >= 3 && strcmp(argv[1], "table") == 0) {
            db_create_table(&db, argv[2]);
        } else if (strcmp(argv[0], "insert") == 0 && argc >= 4) {
            const char *table = argv[1];
            const char *name = argv[2];
            int age = atoi(argv[3]);
            if (!db_insert(&db, table, name, age)) {
                printf("Error: tabla '%s' no existe.\n", table);
            }
        } else if (strcmp(argv[0], "select") == 0 && argc >= 2) {
            const char *table = argv[1];
            if (argc >= 6 && strcmp(argv[2], "where") == 0) {
                const char *field = argv[3];
                const char *op = argv[4];
                const char *value = argv[5];
                db_select_where(&db, table, field, op, value);
            } else {
                db_select_all(&db, table);
            }
        } else if (strcmp(argv[0], "delete") == 0 && argc >= 5 && strcmp(argv[2], "where") == 0) {
            const char *table = argv[1];
            if (strcmp(argv[3], "id") == 0 && strcmp(argv[4], "=") == 0 && argc >= 6) {
                int id = atoi(argv[5]);
                if (!db_delete_by_id(&db, table, id)) {
                    printf("No se encontro registro con id=%d en %s\n", id, table);
                }
            } else {
                printf("Formato delete: delete <table> where id = <n>\n");
            }
        } else if (strcmp(argv[0], "save") == 0) {
            const char *fn = (argc >= 2) ? argv[1] : DB_FILENAME;
            if (db_save(&db, fn)) printf("Guardado en %s\n", fn);
            else printf("Error guardando en %s\n", fn);
        } else if (strcmp(argv[0], "load") == 0) {
            const char *fn = (argc >= 2) ? argv[1] : DB_FILENAME;
            db_free(&db);
            db_init(&db);
            if (db_load(&db, fn)) printf("Cargado desde %s\n", fn);
            else printf("No se pudo cargar %s (archivo inexistente?)\n", fn);
        } else {
            printf("Comando no reconocido. Escribe 'help'.\n");
        }

        free_tokens(argv, argc);
    }

    db_save(&db, DB_FILENAME); 
    db_free(&db);
    printf("Saliendo...\n");
    return 0;
}
