#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"
#include "btree.h"

void main ( int argc, char **argv ) {
   
    int i;
    char campo[30];

    char *nome_campo;

    if (argc < 2) {   
        printf("Usage: ./insert_into <tablename> <ValueX> <ValueY> <ValueZ> ...\n");
        printf("OR\n");
        printf("Usage: ./insert_into <tablename>\n");
    }

    table *tmpTable;

    read_header(argv[1],&tmpTable);

    if (argc-2 != tmpTable->numFields || argc == 2) {   //
        printf("[TABLE: %s; numFields: %i]\n",tmpTable->name, tmpTable->numFields);
        for (i=0;i<tmpTable->numFields;i++) {
            printf("[FIELD: %s; fieldSize: %i]\n",(tmpTable->fields[i]).name, (tmpTable->fields[i]).fieldSize);
        }
        free(tmpTable);
        return;
    }
    
    long newRowIdx = get_num_rows(tmpTable);
    
    table_row *row;
    row = init_row(tmpTable);

    for (i=2;i<argc;i++) {
        set_row_field(tmpTable,row,(tmpTable->fields[i-2]).name,argv[i]);
        if (hasIndex(tmpTable,(tmpTable->fields[i-2]).name)) {
            rdstart();
            insert(argv[i],newRowIdx);
            printtree(root);
            wrstart();
            fclose(fptree);
        }
    }
    save_row(tmpTable,row);

    free(tmpTable);
    free(row);

    return;
}

