#!/bin/sh
gcc -ggdb create_table.c table.c -o create_table
gcc -ggdb insert_into.c table.c -o insert_into
gcc -ggdb select_from.c table.c -o select_from
gcc -ggdb delete_from.c table.c -o delete_from
gcc -ggdb update.c table.c -o update
