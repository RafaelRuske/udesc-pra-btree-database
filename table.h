typedef struct {
    char name[30];
    int startIndex;
    int fieldSize;
} table_field;

typedef struct {
    char name[30];
    int numFields;
    int totalSize;
    table_field *fields;
} table;

typedef struct {
	char *data;
} table_row;

void write_header(FILE *fp, char *tableName, int n_args, char **argv);
void write_header_field(char *fieldName, int fieldSize, int startIndex,FILE *fp);
void read_header (FILE *fp, table **Table);
table_field *get_field(table *table, char *field_name);
table_row *init_row(table *table);
int set_row_field(table *table, table_row *Row, char *field_name, char *value);
char *get_row_field(table *table, table_row *Row, char *field_name);
int save_row(table *table, table_row *Row);
table_row* get_row(table *table, long rowIdx);
void delete_row(table *Table, long rowIdx);
void update_row(table *Table, long rowIdx, table_row *Row);