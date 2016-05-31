#!/bin/sh
gcc -ggdb create_table.c btree.c table.c -o create_table
gcc -ggdb insert_into.c btree.c table.c -o insert_into
gcc -ggdb select_from.c btree.c table.c -o select_from
gcc -ggdb create_index.c btree.c table.c -o create_index
gcc -ggdb delete_from.c btree.c table.c -o delete_from
gcc -ggdb update.c btree.c table.c -o update
gcc -ggdb table.c table_test.c btree.c -lcunit -o crud_teste
