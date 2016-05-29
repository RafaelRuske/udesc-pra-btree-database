#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"

void main ( int argc, char **argv ) {
    int i;
    char campo[30];

    char *nome_campo;

    if (argc < 3) {
        printf("Usage: ./create_table <tablename> <fieldNameX> <fieldSizeX> <fieldNameY> <fieldSizeY> ...");
    }

    write_header(argv[1],(argc-1),argv);

    table *tmpTable;

    read_header(argv[1],&tmpTable);
    printf("[TABLE: %s; numFields: %i; totalSize: %i]\n",tmpTable->name, tmpTable->numFields, tmpTable->totalSize);
    int x;
    for (x=0;x<tmpTable->numFields;x++) {
        printf("[FIELD: %s; startIndex: %i, fieldSize: %i]\n",(tmpTable->fields[x]).name, (tmpTable->fields[x]).startIndex,(tmpTable->fields[x]).fieldSize);
    }
}
