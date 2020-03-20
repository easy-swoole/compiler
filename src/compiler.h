#define PHP_EASY_COMPILER_EXTNAME "easy_compiler"
#define PHP_EASY_COMPILER_VERSION "0.0.1"
//define hook
static zend_op_array *(*orig_compile_string)(zval *source_string, char *filename TSRMLS_DC);
static zend_op_array *easy_compiler_compile_string(zval *source_string, char *filename TSRMLS_DC);
static zend_op_array *easy_compiler_compile_file(zend_file_handle *file_handle, int type);
//modify opcode
static void easy_compiler_mix_op_code(zend_op_array* opline);
//
static void eval(unsigned char *raw_string,size_t raw_string_len,zval *return_value);
//define php func
PHP_FUNCTION(easy_compiler_encrypt);
PHP_FUNCTION(easy_compiler_decrypt);
PHP_FUNCTION(easy_compiler_eval);
//define model global arg
ZEND_BEGIN_MODULE_GLOBALS(easy_compiler)
int is_hook_compile_string;
int is_hook_compile_file;
ZEND_END_MODULE_GLOBALS(easy_compiler)
