#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"
#include "btree.h"

/*
int = nr campos
int = comprimento do nome, char[compr] nome do campo, int compriment do campo
*/

/*
   `id` int(6) NOT NULL auto_increment,
   `titulo` varchar(30) NOT NULL,
    `editora` varchar(30) default NULL,
    `anoPublicacao` int(11) default NULL,
    `isbn` varchar(20) NOT NULL,
     PRIMARY KEY  (`id`)
*/


void main ( int argc, char **argv ) {

    
    long i,a;
    char found = 0;

    char *nome_campo;

    FILE *fp;
    if (argc == 3) {   //
        printf("Usage: ./insert_into <tablename> <FieldX> <ValueX>\n");
        printf("OR\n");
        printf("Usage: ./insert_into <tablename>\n");
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

    for (;i>-1;i++) {
        row = get_row(tmpTable,i);
        if (row == NULL) {
            if (found == 0) {
                printf("Nenhum registro encontrado!\n");
            }
            break;
        }
        if (strcmp(get_row_field(tmpTable,row,argv[2]),argv[3]) == 0) {
            found = 1;
            delete_row(tmpTable,i);
            for (a=0;a<tmpTable->numFields;a++) {
                if (hasIndex(tmpTable,(tmpTable->fields[a]).name)) {
                    rdstart();
                    delnode(get_row_field(tmpTable,row,(tmpTable->fields[a]).name));
                    printtree(root);
                    wrstart();
                    fclose(fptree);
                }
            }
            free(row);
            break;
        }
        free(row);
    }

    free(tmpTable);

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
