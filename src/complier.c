#include <php.h>
#include "complier.h"

//opcode处理
static void modify_opcode(zend_op_array* opline) {
  if (NULL != opline) {
    for (size_t i = 0; i < opline->last; i++) {
      zend_op* orig_opline = &(opline->opcodes[i]);
      if (orig_opline->opcode == ZEND_IS_EQUAL) {
        orig_opline->opcode = ZEND_IS_IDENTICAL;
        zend_vm_set_opcode_handler(orig_opline);
      } else if (orig_opline->opcode == ZEND_IS_NOT_EQUAL) {
        orig_opline->opcode = ZEND_IS_NOT_IDENTICAL;
        zend_vm_set_opcode_handler(orig_opline);
      }
    }
  }
}

static zend_op_array *(*orig_compile_string)(zval *source_string, char *filename TSRMLS_DC);
static zend_op_array *decrypt_compile_string(zval *source_string, char *filename TSRMLS_DC)
{
    zend_op_array* opline = orig_compile_string(source_string, filename);
    modify_opcode(opline);
    return opline;
    //以下为eval等混淆破解
//    if (Z_TYPE_P(source_string) != IS_STRING)
//    {
//        return orig_compile_string(source_string, filename);
//    }
//    int len;
//    char *str;
//    len  = Z_STRLEN_P(source_string);
//    str = estrndup(Z_STRVAL_P(source_string), len);
//    printf("\n==========DUMP===========\n");
//    printf("%s", str);
//    printf("\n==========DUMP===========\n");
//    return orig_compile_string(source_string, filename);
}

static zend_op_array *decrypt_compile_file(zend_file_handle *file_handle, int type)
{
    zend_op_array* opline = compile_file(file_handle, type);
    modify_opcode(opline);
    return opline;
     //以下为eval等混淆破解
//    char *buf;
//    size_t size;
//    if(zend_stream_fixup(file_handle,&buf,&size)==SUCCESS)
//    {
//        printf("%s",file_handle->filename);
//        printf("\n==========DUMP===========\n");
//        int i = 0;
//        for(i=0;i<=size;i++)
//        {
//            printf("%c",buf[i]);
//        }
//        printf("\n==========DUMP===========\n");
//    }
//    return compile_file(file_handle,type);
}

PHP_MINIT_FUNCTION(decrypt_code)
{
    // zend hook
    zend_compile_file = decrypt_compile_file;
    orig_compile_string = zend_compile_string;
    zend_compile_string = decrypt_compile_string;
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(myShut)
{
    zend_compile_string = orig_compile_string;
    return SUCCESS;
}

zend_function_entry easy_complier_functions[] = {
        PHP_FE(easy_complier_encrypt, NULL)
        PHP_FE(easy_complier_decrypt, NULL)
        PHP_FE_END
};

zend_module_entry easy_complier_module_entry = {
        STANDARD_MODULE_HEADER,
        PHP_COMPLIER_EXTNAME,
        easy_complier_functions,
        PHP_MINIT(decrypt_code),
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_COMPLIER_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(easy_complier);

PHP_FUNCTION(easy_complier_encrypt) {
    php_printf("easy_complier_encrypt");
};


PHP_FUNCTION(easy_complier_decrypt) {
    char *encrypt_string;
    size_t encrypt_string_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encrypt_string, &encrypt_string_len) == FAILURE) {
        RETURN_NULL();
    }

    zval retval;
    zend_try {
        zend_eval_string(encrypt_string, return_value, (char *)"" TSRMLS_CC);
    } zend_catch {

    } zend_end_try();
};