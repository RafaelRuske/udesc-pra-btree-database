#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"
#include "btree.h"

void main ( int argc, char **argv ) {

    
    long i,x;
    char found = 0;

    char *nome_campo;

    if (argc == 3) {   //
        printf("Usage: ./update <tablename> <SearchField> <SearchValue> <FieldX> <ValueX> ...\n");
        printf("OR\n");
        printf("Usage: ./update <tablename>\n");
        return;
    }

    table *tmpTable;
    
    read_header(argv[1],&tmpTable);

    if (argc == 2) {   //
        printf("[TABLE: %s; numFields: %i]\n",tmpTable->name, tmpTable->numFields);
        for (i=0;i<tmpTable->numFields;i++) {
            printf("[FIELD: %s; fieldSize: %i]\n",(tmpTable->fields[i]).name, (tmpTable->fields[i]).fieldSize);
        }
        free(tmpTable);
        return;
    }
    
    table_row *row;

    if (hasIndex(tmpTable,argv[2])) {
        printf("[FIELD: %s IS INDEXED!]\n",argv[2]);
        rdstart();
        i = search(argv[3]);
        fclose(fptree);
    } else {
        i = 0;
    }
    
    if (i == -1) {
        printf("Nenhum registro encontrado!\n");
    }

    
    for (i=0;i>-1;i++) {
        row = get_row(tmpTable,i);
        if (row == NULL) {
            if (found == 0) {
                printf("Nenhum registro encontrado!\n");
            }
            break;
        }
        if (strcmp(get_row_field(tmpTable,row,argv[2]),argv[3]) == 0) {
            found = 1;
            for (x=4;x<argc;x+=2) {
                if (hasIndex(tmpTable,argv[x])) {
                    rdstart();
                    delnode(get_row_field(tmpTable,row,argv[x]));
                    insert(argv[x+1],i);
                    printtree(root);
                    wrstart();
                    fclose(fptree);
                }
                set_row_field(tmpTable,row,argv[x],argv[x+1]);
            }
            update_row(tmpTable,i,row);
        }
        free(row);
    }
    

    free(tmpTable);

    return;
}
