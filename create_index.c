#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"
#include "btree.h"

void main ( int argc, char **argv ) {
    long i;
    char found = 0;

    char *nome_campo;

    if (argc == 1) {   //
        printf("Usage: ./create_index <tablename> <FieldX>\n");
        printf("OR\n");
        printf("Usage: ./create_index <tablename>\n");
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
        printf("[FIELD: %s IS ALREADY INDEXED!]\n");
        rdstart();
        printtree(root);
        fclose(fptree);
        return;
    } else {
        char *indexFilename = get_index_filename(tmpTable, argv[2]);
        fptree = fopen(indexFilename, "w+b");
        wrstart();
    }
    
    for (i=0;i>-1;i++) {
        row = get_row(tmpTable,i);
        if (row == NULL) {
            break;
        }
       
        insert(get_row_field(tmpTable,row,argv[2]),i);
        
        free(row);
    }
    printtree(root);
    
    wrstart();
    free(tmpTable);
    fclose(fptree);

    return;
}

/*
   `id` int(6) NOT NULL auto_increment,
   `titulo` varchar(30) NOT NULL,
    `editora` varchar(30) default NULL,
    `anoPublicacao` int(11) default NULL,
    `isbn` varchar(20) NOT NULL,
     PRIMARY KEY  (`id`)


*/
