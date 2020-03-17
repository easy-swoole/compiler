#include <php.h>
#include <zend_string.h>
#include <zend_exceptions.h>
//#include <zend_execute.h>
#include <zend_types.h>
#include <zend_operators.h>
//#include <zend_generators.h>
//#include <zend_vm_execute.h>
#include <stdlib.h>
#include <ext/standard/base64.h>
#include <stdio.h>
#include "compiler.h"


//空实现
static unsigned char* encrypt_str(unsigned char *raw,int raw_size, int *decode_size)
{
    unsigned char *res;
    res = malloc(sizeof(unsigned char)*raw_size);
    *decode_size = raw_size;
    memcpy(res,raw,raw_size);
    return res;
}
//空实现
static unsigned char* decrypt_str(unsigned char *raw,int raw_size, int *decode_size)
{
    unsigned char *res;
    res = malloc(sizeof(unsigned char)*raw_size);
    *decode_size = raw_size;
    memcpy(res,raw,raw_size);
    return res;
}


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
//    printf("%s", filename);
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
        PHP_FE(easy_compiler_include, NULL)
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
    unsigned char *encrypt_string;
    unsigned char *raw_string;
    size_t raw_string_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &raw_string, &raw_string_len) == FAILURE) {
        RETURN_NULL();
    }
    size_t encrypt_len;
    encrypt_string = encrypt_str(raw_string,raw_string_len,&encrypt_len);
    zend_string *zend_encode_string;
    zend_string *base64;
    zend_encode_string = zend_string_init(encrypt_string,encrypt_len,0);
    base64 = php_base64_encode(zend_encode_string,encrypt_len);
    zval z_str;
    ZVAL_STR(return_value,base64);
    zend_string_release(base64);
    zend_string_release(zend_encode_string);
    efree(zend_encode_string);
    efree(base64);
    free(encrypt_string);
    efree(raw_string);
};

PHP_FUNCTION(easy_compiler_decrypt) {
    int i = 0;
    unsigned char *base64;
    unsigned char *decrypt_string;
    size_t base64_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &base64, &base64_len) == FAILURE) {
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
    zend_string *base64_decode;
    base64_decode = php_base64_decode(base64,base64_len);
    efree(base64);
    //得到原始长度和原始字符串
    base64_len = ZSTR_LEN(base64_decode);
    int decrypt_len = NULL;
    decrypt_string = decrypt_str(ZSTR_VAL(base64_decode),base64_len,&decrypt_len);
    zend_string_release(base64_decode);
    efree(base64_decode);

    zend_string *eval_string;
    zval z_str;
    eval_string = zend_string_init(decrypt_string,decrypt_len,0);
    ZVAL_STR(&z_str,eval_string);

    zend_op_array *new_op_array;
    char *filename = zend_get_executed_filename(TSRMLS_C);
    new_op_array =  compile_string(&z_str, filename TSRMLS_C);
    if(new_op_array){
        zend_try {
            // exec new op code
            zend_execute(new_op_array,return_value);
            //zend_eval_stringl(decrypt_string,strlen(decrypt_string), return_value, (char *)"" TSRMLS_CC);
        } zend_catch {

        } zend_end_try();
        destroy_op_array(new_op_array);
        efree(new_op_array);
    }
    zval_ptr_dtor(&z_str);
    free(decrypt_string);
    decrypt_string = NULL;
    efree(filename);
    filename = NULL;
};


PHP_FUNCTION(easy_compiler_include){
    unsigned char *file;
    size_t file_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &file, &file_len) == FAILURE) {
        RETURN_NULL();
    }
    zend_file_handle file_handle;
    int ret = zend_stream_open(file, &file_handle);
    if (ret != SUCCESS)
    {
        RETURN_NULL();
    }
    zend_string *opened_path;
    if (!file_handle.opened_path)
    {
        file_handle.opened_path = zend_string_init(file, strlen(file), 0);
    }
    opened_path = zend_string_copy(file_handle.opened_path);
    zval dummy;
    zval retval;
    zend_op_array *new_op_array;
    ZVAL_NULL(&dummy);
    if (zend_hash_add(&EG(included_files), opened_path, &dummy))
    {
        new_op_array = compile_file(&file_handle, ZEND_REQUIRE);
        zend_destroy_file_handle(&file_handle);
    }
    else
    {
        new_op_array = NULL;
        zend_file_handle_dtor(&file_handle);
    }
    zend_string_release(opened_path);
    if (!new_op_array)
    {
        RETURN_NULL();
    }
    zend_execute(new_op_array, return_value);
    destroy_op_array(new_op_array);
    efree(new_op_array);
    efree(file);
};