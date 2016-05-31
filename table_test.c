#include "CUnit/Basic.h"
#include "table.h"
#include <malloc.h>


void test_get_db_filename(void) {
   CU_ASSERT_STRING_EQUAL(get_db_filename("teste"), "teste.dbU");
}

void test_get_index_filename(void) {
   table tmpTable;
   strcpy(tmpTable.name,"teste");

   CU_ASSERT_STRING_EQUAL(get_index_filename(&tmpTable,"t1"), "teste.t1.dbX");
}

void test_init_row() {
   table tmpTable;
   strcpy(tmpTable.name,"teste");
   tmpTable.numFields = 1;
   tmpTable.totalSize = 10;
   table_field tmpField;
   strcpy(tmpField.name,"field_teste");
   tmpField.startIndex = 0;
   tmpField.fieldSize = 10;
   tmpTable.fields = &tmpField;

   table_row *row = init_row(&tmpTable);
   CU_ASSERT_PTR_NOT_NULL(row);
}

void test_set_get_row_field() {
   table tmpTable;
   strcpy(tmpTable.name,"teste");
   tmpTable.numFields = 1;
   tmpTable.totalSize = 10;
   tmpTable.fields = (table_field*) malloc(sizeof(table_field));

   strcpy(tmpTable.fields[0].name,"field_teste");
   tmpTable.fields[0].startIndex = 0;
   tmpTable.fields[0].fieldSize = 10;


   table_row *row = init_row(&tmpTable);

   set_row_field(&tmpTable, row, "field_teste", "teste123");
   CU_ASSERT_STRING_EQUAL(get_row_field(&tmpTable,row,"field_teste"), "teste123");
}

void test_save_get_get_field_row() {
   char **params;
   int i;
   
   params = (char**) malloc(sizeof(char*) * 4);

   for (i=0;i<4;i++) {
      params[i] = (char*) malloc(sizeof(char) * 50);
   }

   strcpy(params[0],"teste_programa");
   strcpy(params[1],"teste_tabela");
   strcpy(params[2],"campo");
   strcpy(params[3],"10");


   write_header("teste_tabela", 3, params);

   for (i=0;i<4;i++) {
      free(params[i]);
   }
   free(params);

   table *tmpTable;
   read_header("teste_tabela",&tmpTable);

   table_row *row = init_row(tmpTable);
   set_row_field(tmpTable, row, "campo", "teste123");
   save_row(tmpTable,row);
   free(row);

   row = get_row(tmpTable,0);
   CU_ASSERT_STRING_EQUAL(get_row_field(tmpTable,row,"campo"), "teste123");
   table_field *field = get_field(tmpTable,"campo");
   CU_ASSERT_STRING_EQUAL(field->name, "campo");
   CU_ASSERT(get_num_rows(tmpTable) == 1);
   
   set_row_field(tmpTable, row, "campo", "teste456");
   update_row(tmpTable,0,row);
   free(row);
   row = get_row(tmpTable,0);
   CU_ASSERT_STRING_EQUAL(get_row_field(tmpTable,row,"campo"), "teste456");
   
   delete_row(tmpTable,0);
   free(row);
   row = get_row(tmpTable,0);
   CU_ASSERT_STRING_EQUAL(row->data, "");

   CU_ASSERT(hasIndex(tmpTable,"campo") == 0);
   FILE *fp = fopen("teste_tabela.campo.dbX","w");
   fprintf(fp,"aaaaa");
   fclose(fp);
   CU_ASSERT(hasIndex(tmpTable,"campo") == 1);
   unlink("teste_tabela.campo.dbX");

   free(row);
   free(tmpTable);
}

void test_write_and_read_header() {
   char **params;
   int i;
   
   params = (char**) malloc(sizeof(char*) * 4);

   for (i=0;i<4;i++) {
      params[i] = (char*) malloc(sizeof(char) * 50);
   }

   strcpy(params[0],"teste_programa");
   strcpy(params[1],"teste_tabela");
   strcpy(params[2],"campo");
   strcpy(params[3],"10");


   write_header("teste_tabela", 3, params);

   for (i=0;i<4;i++) {
      free(params[i]);
   }
   free(params);

   table *tmpTable;
   read_header("teste_tabela",&tmpTable);

   CU_ASSERT_STRING_EQUAL(tmpTable->name, "teste_tabela");
   CU_ASSERT(tmpTable->numFields == 1);
   CU_ASSERT(tmpTable->totalSize == 10);

   free(tmpTable);
}

int main()
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", NULL, NULL);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   if (
         (NULL == CU_add_test(pSuite, "test_save_get_get_field_row", test_save_get_get_field_row))||
         (NULL == CU_add_test(pSuite, "test_set_get_row_field", test_set_get_row_field))||
         (NULL == CU_add_test(pSuite, "test_init_row", test_init_row))||
         (NULL == CU_add_test(pSuite, "test_write_and_read_header", test_write_and_read_header))||
   		(NULL == CU_add_test(pSuite, "test_get_db_filename", test_get_db_filename))||
   		(NULL == CU_add_test(pSuite, "test_get_index_filename", test_get_index_filename))
   		)
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

