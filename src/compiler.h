#define PHP_compiler_EXTNAME "easy_compiler"
#define PHP_compiler_VERSION "0.0.1"
PHP_FUNCTION(easy_compiler_encrypt);
PHP_FUNCTION(easy_compiler_decrypt);
ZEND_BEGIN_MODULE_GLOBALS(easy_compiler)
char *compile_string_check;
ZEND_END_MODULE_GLOBALS(easy_compiler)
