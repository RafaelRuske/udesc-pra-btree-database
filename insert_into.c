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

    
    int i;
    char campo[30];

    char *nome_campo;

    FILE *fp;
    if (argc < 2) {   //
        printf("Usage: ./insert_into <tablename> <ValueX> <ValueY> <ValueZ> ...\n");
        printf("OR\n");
        printf("Usage: ./insert_into <tablename>\n");
    }

    table *tmpTable; // = (table*) malloc(sizeof(table));
    //table_field tmpField;

    fp = fopen(argv[1], "rb");
    read_header(fp,&tmpTable);
    fclose(fp);

    if (argc-2 != tmpTable->numFields || argc == 2) {   //
        printf("[TABLE: %s; numFields: %i]\n",tmpTable->name, tmpTable->numFields);
        for (i=0;i<tmpTable->numFields;i++) {
            printf("[FIELD: %s; fieldSize: %i]\n",(tmpTable->fields[i]).name, (tmpTable->fields[i]).fieldSize);
        }
        free(tmpTable);
        return;
    }
    
    table_row *row;
    row = init_row(tmpTable);

    for (i=2;i<argc;i++) {
        set_row_field(tmpTable,row,(tmpTable->fields[i-2]).name,argv[i]);
    }
    save_row(tmpTable,row);
    

    free(tmpTable);
    free(row);

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
