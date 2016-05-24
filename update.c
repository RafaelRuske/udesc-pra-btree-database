#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "table.h"

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


main ( int argc, char **argv ) {

    
    int i,x;
    char found = 0;

    char *nome_campo;

    FILE *fp;
    if (argc == 3) {   //
        printf("Usage: ./update <tablename> <SearchField> <SearchValue> <FieldX> <ValueX> ...\n");
        printf("OR\n");
        printf("Usage: ./update <tablename>\n");
        return;
    }

    table *tmpTable; // = (table*) malloc(sizeof(table));
    //table_field tmpField;

    fp = fopen(argv[1], "rb");
    read_header(fp,&tmpTable);
    fclose(fp);

    if (argc == 2) {   //
        printf("[TABLE: %s; numFields: %i]\n",tmpTable->name, tmpTable->numFields);
        for (i=0;i<tmpTable->numFields;i++) {
            printf("[FIELD: %s; fieldSize: %i]\n",(tmpTable->fields[i]).name, (tmpTable->fields[i]).fieldSize);
        }
        free(tmpTable);
        return;
    }
    
    table_row *row;

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
                set_row_field(tmpTable,row,argv[x],argv[x+1]);
            }
            update_row(tmpTable,i,row);
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
