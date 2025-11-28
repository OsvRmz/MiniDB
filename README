## MiniBD
"Motor de base de datos" basico escrito en C, diseñado para funcionar desde la linea de comandos.
Permite crear tablas, insertar registros, consultarlos, eliminarlos y guardar/cargar los datos desde un archivo binario.

**Contenido del proyecto**

Manejo de archivos, memoria dinamica, estructuras de datos y parseo basico de comandos *simulando* SQL.

## Caracteristicas

- Sintaxis similar a SQL muy simplificada
- Soporte para multiples tablas
- Campos soportados por defecto:
- id (autonumerico)
- name (string)
- age (entero)
- Comandos disponibles: create, insert, select, delete, save, load

- Persistencia mediante archivo binario (data.db)

- Proyecto dividido en modulos (parser, database, utils, main)

## Ejemplo de uso
```bash
> create table users
> insert users "Juan" 21
> insert users "Maria" 30
> select users
1 | Juan | 21
2 | Maria | 30
> select users where age > 25
2 | Maria | 30
> delete users where id = 1
> select users
2 | Maria | 30
> save
Guardado en data.db
```

## Comandos disponibles
(dentro de la consola puedes ingresar "help" para volver a abrir esta lista cuando lo necesites)
```bash
create table <name>
insert <table> "<name>" <age>
select <table>
select <table> where <field> <op> <value>
delete <table> where id = <n>
save [filename]
load [filename]
exit
```
## Compilación

En Windows (MinGW):
```
gcc -std=c99 -Wall src/*.c -o mini-db.exe
```

El archivo data.db será generado o regenerado tras cada ejecución del archivo debido a que estará continuamente actualizandose, el proyecto ya contiene uno como base y con información base, pero puede ser borrado para empezar desde 0.

