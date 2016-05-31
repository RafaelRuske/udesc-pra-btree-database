#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "table.h"
#include "btree.h"
#include <sys/stat.h>

char* get_db_filename(char *tableName) {
    int sz = strlen(tableName) + 4;
    char *ret = (char*) malloc(sizeof(char) * sz);
    
    strcpy(ret,tableName);
    strcat(ret,".dbU");
    
    return ret;
}

char* get_index_filename(table *Table, char *fieldName) {
    int sz = strlen(Table->name) + strlen(fieldName) + 5;
    char *ret = (char*) malloc(sizeof(char) * sz);
    
    strcpy(ret,Table->name);
    strcat(ret,".");
    strcat(ret,fieldName);
    strcat(ret,".dbX");
    
    return ret;
}

char hasIndex(table *Table, char *fieldName) {
    char *indexFilename = get_index_filename(Table, fieldName);
	fptree = fopen(indexFilename, "r+b");
    //colocar free?
	if (fptree == NULL) {
        return 0;
    }
    return 1;
}

void write_header(char *tableName, int n_args, char **argv) {
        FILE *fp;
        int i;
        int fieldSize, startIndex;
        table Table;
        char *fieldName;
        
        char *dbFilename = get_db_filename(tableName);
        fp = fopen(dbFilename, "wb");
        free(dbFilename);
        
        strcpy(Table.name, tableName);
        Table.numFields = (n_args-1) / 2;
        Table.totalSize = 0;
        for (i=2;i<n_args;i+=2) {
            Table.totalSize += atoi(argv[i+1]);
        }
        fwrite(&Table, sizeof(table), 1, fp);

        startIndex = 0;

        for (i=2;i<n_args;i+=2) {
            fieldName = argv[i];
            fieldSize = atoi(argv[i+1]);
            write_header_field(fieldName, fieldSize, startIndex, fp);
            startIndex += (fieldSize + 1);
        }
        
        fclose(fp);
}

void write_header_field(char *fieldName, int fieldSize, int startIndex,FILE *fp) {
    table_field tableField;

    strcpy(tableField.name,fieldName);
    tableField.fieldSize = fieldSize;
    tableField.startIndex = startIndex;
    fwrite(&tableField, sizeof(table_field), 1, fp);
}

void read_header (char *tableName, table **Table) {
    int x;
    FILE *fp;
    char *dbFilename = get_db_filename(tableName);
    fp = fopen(dbFilename, "rb");
    free(dbFilename);
    
    (*Table) = (table*) malloc(sizeof(table));

    fread((*Table),sizeof(table),1,fp);    
    (*Table)->fields = (table_field*) malloc(sizeof(table_field) * (*Table)->numFields);
    
    for (x=0;x<(*Table)->numFields;x++) {
        fread(&((*Table)->fields[x]),sizeof(table_field),1,fp);    
    }
    fclose(fp);
}

table_field *get_field(table *Table, char *field_name) {
    int i;
    for (i=0;i<Table->numFields;i++) {
        if (strcmp((Table->fields[i]).name,field_name) == 0) {
            return &(Table->fields[i]);
        }
    }
    return NULL;
}

table_row *init_row(table *table) {
    table_row *tmpRow = (table_row*) malloc(sizeof(table_row));
    tmpRow->data = (char*) calloc(table->totalSize,sizeof(char));
}

int set_row_field(table *table, table_row *Row, char *field_name, char *value) {
    table_field *tmpField = get_field(table,field_name);
    
    if (strlen(value) > tmpField->fieldSize) {
        return -1;
    }
    memcpy(&((*Row).data[tmpField->startIndex]),value,strlen(value) * sizeof(char));
}

char *get_row_field(table *table, table_row *Row, char *field_name) {
    table_field *tmpField = get_field(table,field_name);
    if (tmpField != NULL) {
        return &((*Row).data[tmpField->startIndex]);
    }
    return NULL;
}

int save_row(table *Table, table_row *Row) {
    FILE *fp;

    char *dbFilename = get_db_filename((*Table).name);
    fp = fopen(dbFilename, "a+");
    free(dbFilename);

    fwrite(Row->data, (*Table).totalSize * sizeof(char), 1, fp);
    fclose(fp);
}

table_row* get_row(table *Table, long rowIdx) {
    FILE *fp;
    table_row *tmpRow = (table_row*) malloc(sizeof(table_row));
    (*tmpRow).data = (char*) calloc(Table->totalSize,sizeof(char));
    
    char *dbFilename = get_db_filename((*Table).name);
    fp = fopen(dbFilename, "r");
    free(dbFilename);

    fseek(fp, sizeof(table) + (sizeof(table_field) * Table->numFields), SEEK_SET);
    fseek(fp, rowIdx * (*Table).totalSize * sizeof(char), SEEK_CUR);
    size_t _read = fread(tmpRow->data,(*Table).totalSize * sizeof(char),1,fp);
    if (_read == 0) {
        free(tmpRow);
        tmpRow = NULL;
    }

    fclose(fp);

    return tmpRow;
}

void delete_row(table *Table, long rowIdx) {
    FILE *fp;
    table_row *tmpRow = init_row(Table);
    
    char *dbFilename = get_db_filename((*Table).name);
    fp = fopen(dbFilename, "r+");
    free(dbFilename);

    fseek(fp, sizeof(table) + (sizeof(table_field) * Table->numFields), SEEK_SET);
    fseek(fp, rowIdx * (*Table).totalSize * sizeof(char), SEEK_CUR);
    fwrite(tmpRow->data, (*Table).totalSize * sizeof(char), 1, fp);
    fclose(fp);

    free(tmpRow);
}

void update_row(table *Table, long rowIdx, table_row *Row) {
    FILE *fp;
    
    char *dbFilename = get_db_filename((*Table).name);
    fp = fopen(dbFilename, "r+");
    free(dbFilename);

    fseek(fp, sizeof(table) + (sizeof(table_field) * Table->numFields), SEEK_SET);
    fseek(fp, rowIdx * (*Table).totalSize * sizeof(char), SEEK_CUR);
    fwrite(Row->data, (*Table).totalSize * sizeof(char), 1, fp);
    fclose(fp);
}

long get_num_rows(table *Table) {
    char *dbFilename = get_db_filename((*Table).name);

    struct stat st;
    stat(dbFilename, &st);
    free(dbFilename);
    return (st.st_size - (sizeof(table) + (sizeof(table_field) * Table->numFields)))/((*Table).totalSize * sizeof(char));
}
