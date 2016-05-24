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

    if (argc < 3) {
        printf("Usage: ./create_table <tablename> <fieldNameX> <fieldSizeX> <fieldNameY> <fieldSizeY> ...");
    }

    fp = fopen(argv[1], "wb");

    //return;
    write_header(fp,argv[1],(argc-1),argv);

    fclose(fp);

    fp = NULL;

    table *tmpTable; // = (table*) malloc(sizeof(table));
    //table_field tmpField;

    fp = fopen(argv[1], "rb");
    read_header(fp,&tmpTable);
    printf("[TABLE: %s; numFields: %i; totalSize: %i]\n",tmpTable->name, tmpTable->numFields, tmpTable->totalSize);
    int x;
    for (x=0;x<tmpTable->numFields;x++) {
        printf("[FIELD: %s; startIndex: %i, fieldSize: %i]\n",(tmpTable->fields[x]).name, (tmpTable->fields[x]).startIndex,(tmpTable->fields[x]).fieldSize);
    }
    fclose(fp);

}
