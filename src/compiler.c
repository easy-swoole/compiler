#include <php.h>
#include <zend_exceptions.h>
#include "compiler.h"
#include "base64.h"


ZEND_DECLARE_MODULE_GLOBALS(easy_compiler);

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

static zend_op_array *decrypt_compile_string(zval *source_string, char *filename TSRMLS_DC)
{
    if(easy_compiler_globals.is_hook_compile_string == false){
        easy_compiler_globals.is_hook_compile_string = true;
    }
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
    if(easy_compiler_globals.is_hook_compile_file == false){
        easy_compiler_globals.is_hook_compile_file = true;
    }
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

static void throw_exception(char *msg)
{
    zend_throw_exception(easy_compiler_exception_class_entry_ptr, msg, 0 TSRMLS_CC);
}

PHP_MINIT_FUNCTION(decrypt_code)
{
    //init exception class
    INIT_CLASS_ENTRY(easy_compiler_exception_ce, "EasySwoole\\EasyCompilerException", NULL);
    easy_compiler_exception_class_entry_ptr = zend_register_internal_class_ex(&easy_compiler_exception_ce, zend_exception_get_default(TSRMLS_C));
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

zend_function_entry easy_compiler_functions[] = {
        PHP_FE(easy_compiler_encrypt, NULL)
        PHP_FE(easy_compiler_decrypt, NULL)
        PHP_FE_END
};

zend_module_entry easy_compiler_module_entry = {
        STANDARD_MODULE_HEADER,
        PHP_compiler_EXTNAME,
        easy_compiler_functions,
        PHP_MINIT(decrypt_code),
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_compiler_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(easy_compiler);

PHP_FUNCTION(easy_compiler_encrypt) {
    php_printf("easy_compiler_encrypt");
};

PHP_FUNCTION(easy_compiler_decrypt) {
    unsigned char *encrypt_string;
    size_t encrypt_string_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &encrypt_string, &encrypt_string_len) == FAILURE) {
        RETURN_NULL();
    }
    //提前执行一次eval空字符串,用来判定compile_file和compile_string是否被hook替换，禁止从内存拿数据
    zend_try {
        zend_eval_string("", NULL, (char *)"" TSRMLS_CC);
    } zend_catch {

    } zend_end_try();

    if(easy_compiler_globals.is_hook_compile_file == false){
         throw_exception("hook compile_file is forbid");
    }

    if(easy_compiler_globals.is_hook_compile_string == false){
        throw_exception("hook compile_string is forbid");
    }
    unsigned char *base64_decode_str = NULL;
    base64_decode_str = easy_base64_decode(encrypt_string);
    printf("%s",base64_decode_str);

//    zend_try {
//        zend_eval_string(encrypt_string, return_value, (char *)"" TSRMLS_CC);
//    } zend_catch {
//
//    } zend_end_try();
};


